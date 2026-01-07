/*
 * Limiter.hpp
 *
 * Dynamic audio limiter and anti-clipping system with lookahead processing
 * Provides automatic gain reduction to prevent distortion and protect speakers
 */

#pragma once

#include <cmath>
#include <algorithm>
#include <vector>

namespace
{
  // Limiter constants
  constexpr float LIMITER_THRESHOLD = 4.5f;      // Threshold in volts (below VCV's ±5V range)
  constexpr float LIMITER_CEILING = 4.75f;       // Maximum output level in volts
  constexpr float LIMITER_KNEE_WIDTH = 0.5f;     // Soft knee width in dB
  constexpr float ATTACK_TIME_MS = 0.1f;         // Very fast attack for transients
  constexpr float RELEASE_TIME_MS = 50.0f;       // Moderate release to avoid pumping
  constexpr float LOOKAHEAD_TIME_MS = 5.0f;      // Lookahead buffer time
  constexpr float ENVELOPE_FOLLOWER_TAU = 1.0f;  // Envelope detector time constant
  constexpr float MIN_GAIN_REDUCTION = 0.01f;    // Minimum gain (prevents total silence)
  constexpr float AUTO_MAKEUP_RATIO = 0.8f;      // Automatic makeup gain compensation
}

class AudioLimiter
{
private:
  // Lookahead delay line
  std::vector<float> delayBuffer;
  size_t delayBufferSize;
  size_t writeIndex;
  
  // Envelope detection
  float envelopeLevel;
  float gainReduction;
  
  // Time constants (sample-rate dependent)
  float attackCoeff;
  float releaseCoeff;
  float envelopeCoeff;
  
  // Sample rate
  float sampleRate;
  
  // Auto gain staging
  float makeupGain;
  float peakHistory;
  
  /**
   * Convert time in milliseconds to coefficient for exponential filter
   */
  float timeToCoeff(float timeMs) const
  {
    if (timeMs <= 0.0f)
      return 0.0f;
    return std::exp(-1.0f / (timeMs * 0.001f * sampleRate));
  }
  
  /**
   * Convert linear amplitude to dB
   */
  float linearToDb(float linear) const
  {
    return 20.0f * std::log10(std::max(linear, 1e-6f));
  }
  
  /**
   * Convert dB to linear amplitude
   */
  float dbToLinear(float db) const
  {
    return std::pow(10.0f, db / 20.0f);
  }
  
  /**
   * Soft-knee compression curve
   * Provides smooth transition into gain reduction
   */
  float softKneeCompression(float inputDb, float thresholdDb, float kneeWidthDb) const
  {
    if (inputDb < (thresholdDb - kneeWidthDb / 2.0f))
    {
      // Below knee - no compression
      return inputDb;
    }
    else if (inputDb > (thresholdDb + kneeWidthDb / 2.0f))
    {
      // Above knee - full compression
      return thresholdDb;
    }
    else
    {
      // Within knee - smooth transition using quadratic curve
      float delta = inputDb - (thresholdDb - kneeWidthDb / 2.0f);
      float deltaSquared = delta * delta;
      return inputDb - (deltaSquared / (2.0f * kneeWidthDb));
    }
  }
  
  /**
   * Hard-knee compression curve
   * Aggressive limiting for peak protection
   */
  float hardKneeCompression(float inputDb, float thresholdDb) const
  {
    return std::min(inputDb, thresholdDb);
  }
  
  /**
   * Detect peak level in lookahead buffer
   */
  float detectPeakLevel() const
  {
    float peak = 0.0f;
    for (size_t i = 0; i < delayBufferSize; ++i)
    {
      float abs_sample = std::abs(delayBuffer[i]);
      if (abs_sample > peak)
        peak = abs_sample;
    }
    return peak;
  }
  
  /**
   * Calculate required gain reduction based on detected level
   */
  float calculateGainReduction(float detectedLevel)
  {
    if (detectedLevel < 1e-6f)
      return 1.0f; // No reduction needed for silence
    
    float levelDb = linearToDb(detectedLevel);
    float thresholdDb = linearToDb(LIMITER_THRESHOLD);
    float ceilingDb = linearToDb(LIMITER_CEILING);
    
    // Use soft-knee for most cases, hard-knee as safety net
    float softKneeDb = softKneeCompression(levelDb, thresholdDb, LIMITER_KNEE_WIDTH);
    float hardKneeDb = hardKneeCompression(levelDb, ceilingDb);
    
    // Choose the more conservative of the two
    float targetDb = std::min(softKneeDb, hardKneeDb);
    
    // Calculate gain reduction
    float reductionDb = targetDb - levelDb;
    float reductionLinear = dbToLinear(reductionDb);
    
    // Clamp to safe range
    return std::max(reductionLinear, MIN_GAIN_REDUCTION);
  }
  
  /**
   * Update automatic makeup gain based on signal history
   */
  void updateMakeupGain(float currentLevel)
  {
    // Track peak history with slow decay
    const float historyCoeff = 0.9999f;
    peakHistory = std::max(currentLevel, peakHistory * historyCoeff);
    
    // Calculate makeup gain to normalize output
    if (peakHistory > 1e-6f)
    {
      float targetGain = (LIMITER_THRESHOLD * AUTO_MAKEUP_RATIO) / peakHistory;
      makeupGain = std::min(targetGain, 2.0f); // Cap at +6dB makeup
    }
    else
    {
      makeupGain = 1.0f;
    }
  }

public:
  AudioLimiter()
    : delayBufferSize(0)
    , writeIndex(0)
    , envelopeLevel(0.0f)
    , gainReduction(1.0f)
    , attackCoeff(0.0f)
    , releaseCoeff(0.0f)
    , envelopeCoeff(0.0f)
    , sampleRate(44100.0f)
    , makeupGain(1.0f)
    , peakHistory(0.0f)
  {
  }
  
  /**
   * Initialize limiter with sample rate
   */
  void init(float sampleRate_)
  {
    sampleRate = sampleRate_;
    
    // Calculate lookahead buffer size
    delayBufferSize = static_cast<size_t>(LOOKAHEAD_TIME_MS * 0.001f * sampleRate);
    if (delayBufferSize < 1)
      delayBufferSize = 1;
    
    delayBuffer.resize(delayBufferSize, 0.0f);
    writeIndex = 0;
    
    // Calculate time constants
    attackCoeff = timeToCoeff(ATTACK_TIME_MS);
    releaseCoeff = timeToCoeff(RELEASE_TIME_MS);
    envelopeCoeff = timeToCoeff(ENVELOPE_FOLLOWER_TAU);
    
    // Reset state
    envelopeLevel = 0.0f;
    gainReduction = 1.0f;
    makeupGain = 1.0f;
    peakHistory = 0.0f;
  }
  
  /**
   * Process a single audio sample through the limiter
   * Returns the limited and gain-staged output
   */
  float process(float input)
  {
    // Write input to lookahead buffer
    delayBuffer[writeIndex] = input;
    
    // Detect peak in lookahead window
    float peakLevel = detectPeakLevel();
    
    // Update envelope follower
    float targetEnvelope = peakLevel;
    if (targetEnvelope > envelopeLevel)
      envelopeLevel = envelopeLevel + (1.0f - envelopeCoeff) * (targetEnvelope - envelopeLevel);
    else
      envelopeLevel = envelopeLevel * envelopeCoeff;
    
    // Calculate required gain reduction
    float targetGainReduction = calculateGainReduction(envelopeLevel);
    
    // Apply attack/release to gain reduction
    if (targetGainReduction < gainReduction)
    {
      // Attack - fast reduction for peak protection
      gainReduction = targetGainReduction + attackCoeff * (gainReduction - targetGainReduction);
    }
    else
    {
      // Release - slower recovery to avoid pumping
      gainReduction = targetGainReduction + releaseCoeff * (gainReduction - targetGainReduction);
    }
    
    // Read oldest sample from buffer (lookahead delay)
    size_t readIndex = (writeIndex + 1) % delayBufferSize;
    float delayedSample = delayBuffer[readIndex];
    
    // Update makeup gain
    updateMakeupGain(std::abs(delayedSample * gainReduction));
    
    // Apply gain reduction and makeup gain
    float output = delayedSample * gainReduction * makeupGain;
    
    // Safety hard clipper (should rarely engage)
    output = std::max(-LIMITER_CEILING, std::min(LIMITER_CEILING, output));
    
    // Advance write index
    writeIndex = (writeIndex + 1) % delayBufferSize;
    
    return output;
  }
  
  /**
   * Reset limiter state
   */
  void reset()
  {
    std::fill(delayBuffer.begin(), delayBuffer.end(), 0.0f);
    writeIndex = 0;
    envelopeLevel = 0.0f;
    gainReduction = 1.0f;
    peakHistory = 0.0f;
    makeupGain = 1.0f;
  }
  
  /**
   * Get current gain reduction amount (for metering)
   */
  float getGainReduction() const
  {
    return gainReduction;
  }
  
  /**
   * Get current makeup gain (for metering)
   */
  float getMakeupGain() const
  {
    return makeupGain;
  }
};
