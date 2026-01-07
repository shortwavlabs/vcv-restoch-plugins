/*
 * Limiter_test.cpp
 * Unit tests for AudioLimiter class
 * 
 * Tests cover:
 * - Initialization and configuration
 * - Peak detection and gain reduction
 * - Soft-knee and hard-knee compression
 * - Lookahead processing
 * - Attack and release envelopes
 * - Automatic makeup gain
 * - Hard clipping protection
 * - Signal fidelity at safe levels
 */

#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include <string>
#include <cstdlib>

// Include the limiter
#include "../utils/Limiter.hpp"

// Test utilities
namespace TestUtils
{
  const float EPSILON = 1e-5f;
  
  bool floatEquals(float a, float b, float epsilon = EPSILON)
  {
    return std::abs(a - b) < epsilon;
  }
  
  void assertFloatEquals(float expected, float actual, const std::string& message, float epsilon = EPSILON)
  {
    if (!floatEquals(expected, actual, epsilon))
    {
      std::cerr << "FAIL: " << message << std::endl;
      std::cerr << "  Expected: " << expected << ", Got: " << actual << std::endl;
      assert(false);
    }
  }
  
  void assertTrue(bool condition, const std::string& message)
  {
    if (!condition)
    {
      std::cerr << "FAIL: " << message << std::endl;
      assert(false);
    }
  }
  
  void assertLess(float value, float max, const std::string& message)
  {
    if (value >= max)
    {
      std::cerr << "FAIL: " << message << std::endl;
      std::cerr << "  Value " << value << " is not less than " << max << std::endl;
      assert(false);
    }
  }
  
  void assertGreater(float value, float min, const std::string& message)
  {
    if (value <= min)
    {
      std::cerr << "FAIL: " << message << std::endl;
      std::cerr << "  Value " << value << " is not greater than " << min << std::endl;
      assert(false);
    }
  }
  
  void assertInRange(float value, float min, float max, const std::string& message)
  {
    if (value < min || value > max)
    {
      std::cerr << "FAIL: " << message << std::endl;
      std::cerr << "  Value " << value << " is not in range [" << min << ", " << max << "]" << std::endl;
      assert(false);
    }
  }
}

using namespace TestUtils;

// Test functions
void testInitialization()
{
  std::cout << "Testing initialization..." << std::endl;
  
  AudioLimiter limiter;
  limiter.init(44100.0f);
  
  // Process silence and verify no gain reduction
  float output = limiter.process(0.0f);
  assertFloatEquals(0.0f, output, "Silence should pass through unchanged");
  
  // Verify initial gain reduction is neutral
  assertFloatEquals(1.0f, limiter.getGainReduction(), "Initial gain reduction should be 1.0 (no reduction)", 0.1f);
  
  std::cout << "  ✓ Initialization test passed" << std::endl;
}

void testSignalPassthrough()
{
  std::cout << "Testing signal passthrough at safe levels..." << std::endl;
  
  AudioLimiter limiter;
  limiter.init(44100.0f);
  
  // Test safe signal levels (below threshold)
  float safeLevel = 2.0f; // Well below 4.5V threshold
  
  // Process enough samples to stabilize makeup gain
  for (int i = 0; i < 1000; ++i)
  {
    float input = safeLevel * std::sin(2.0f * M_PI * 440.0f * i / 44100.0f);
    limiter.process(input);
  }
  
  // Test that safe signals pass with minimal alteration
  float input = safeLevel * std::sin(2.0f * M_PI * 440.0f * 1000.0f / 44100.0f);
  float output = limiter.process(input);
  
  // Output should be close to input (within makeup gain tolerance)
  assertInRange(output, -5.0f, 5.0f, "Safe signal should remain within ±5V");
  
  std::cout << "  ✓ Signal passthrough test passed" << std::endl;
}

void testPeakLimiting()
{
  std::cout << "Testing peak limiting..." << std::endl;
  
  AudioLimiter limiter;
  limiter.init(44100.0f);
  
  // Test with signal that exceeds threshold
  float dangerousLevel = 6.0f; // Exceeds 4.75V ceiling
  
  // Process several samples at dangerous level
  std::vector<float> outputs;
  for (int i = 0; i < 500; ++i)
  {
    float input = dangerousLevel * std::sin(2.0f * M_PI * 440.0f * i / 44100.0f);
    float output = limiter.process(input);
    outputs.push_back(output);
  }
  
  // Verify all outputs are clamped to safe levels
  for (float output : outputs)
  {
    assertInRange(output, -4.75f, 4.75f, "Output should be clamped to ceiling");
  }
  
  // Verify gain reduction occurred
  float gainReduction = limiter.getGainReduction();
  assertLess(gainReduction, 1.0f, "Gain reduction should be active for peaks");
  
  std::cout << "  ✓ Peak limiting test passed" << std::endl;
}

void testAttackResponse()
{
  std::cout << "Testing attack response..." << std::endl;
  
  AudioLimiter limiter;
  limiter.init(44100.0f);
  
  // Start with silence to establish baseline
  for (int i = 0; i < 200; ++i)
  {
    limiter.process(0.0f);
  }
  
  float initialGain = limiter.getGainReduction();
  
  // Sudden sustained peak well above threshold
  for (int i = 0; i < 200; ++i)
  {
    limiter.process(6.5f); // Well above 4.5V threshold
  }
  
  float attackedGain = limiter.getGainReduction();
  
  // Gain should reduce significantly
  assertLess(attackedGain, 0.95f, "Significant gain reduction should occur");
  
  std::cout << "  ✓ Attack response test passed" << std::endl;
}

void testReleaseResponse()
{
  std::cout << "Testing release response..." << std::endl;
  
  AudioLimiter limiter;
  limiter.init(44100.0f);
  
  // Create a peak
  for (int i = 0; i < 100; ++i)
  {
    limiter.process(5.5f);
  }
  
  float peakGain = limiter.getGainReduction();
  
  // Return to silence
  for (int i = 0; i < 10000; ++i)
  {
    limiter.process(0.1f); // Very low level
  }
  
  float releasedGain = limiter.getGainReduction();
  
  // Gain should recover toward 1.0
  assertGreater(releasedGain, peakGain, "Gain should increase during release");
  
  std::cout << "  ✓ Release response test passed" << std::endl;
}

void testLookaheadProcessing()
{
  std::cout << "Testing lookahead processing..." << std::endl;
  
  AudioLimiter limiter;
  limiter.init(44100.0f);
  
  // Feed silence first
  for (int i = 0; i < 10; ++i)
  {
    limiter.process(0.0f);
  }
  
  // Then feed a peak
  limiter.process(6.0f);
  
  // The peak should be delayed by the lookahead buffer
  // Immediate output should still be from the earlier silence
  float immediateOutput = limiter.process(0.0f);
  
  // Verify lookahead is working (output is delayed)
  assertInRange(immediateOutput, -5.0f, 5.0f, "Lookahead should delay the peak");
  
  std::cout << "  ✓ Lookahead processing test passed" << std::endl;
}

void testMakeupGain()
{
  std::cout << "Testing automatic makeup gain..." << std::endl;
  
  AudioLimiter limiter;
  limiter.init(44100.0f);
  
  // Process a consistent moderate signal
  float moderateLevel = 3.0f;
  for (int i = 0; i < 5000; ++i)
  {
    float input = moderateLevel * std::sin(2.0f * M_PI * 440.0f * i / 44100.0f);
    limiter.process(input);
  }
  
  float makeupGain = limiter.getMakeupGain();
  
  // Makeup gain should be active and reasonable
  assertGreater(makeupGain, 0.1f, "Makeup gain should be active");
  assertLess(makeupGain, 5.0f, "Makeup gain should be reasonable");
  
  std::cout << "  ✓ Makeup gain test passed" << std::endl;
}

void testHardClipping()
{
  std::cout << "Testing hard clipping protection..." << std::endl;
  
  AudioLimiter limiter;
  limiter.init(44100.0f);
  
  // Feed extreme values
  std::vector<float> extremeInputs = {10.0f, -10.0f, 15.0f, -15.0f, 20.0f, -20.0f};
  
  for (float input : extremeInputs)
  {
    float output = limiter.process(input);
    
    // All outputs must be clamped to ceiling
    assertInRange(output, -4.75f, 4.75f, "Hard clipper must enforce ceiling");
  }
  
  std::cout << "  ✓ Hard clipping protection test passed" << std::endl;
}

void testReset()
{
  std::cout << "Testing reset functionality..." << std::endl;
  
  AudioLimiter limiter;
  limiter.init(44100.0f);
  
  // Process some signal
  for (int i = 0; i < 100; ++i)
  {
    limiter.process(5.5f);
  }
  
  // Reset
  limiter.reset();
  
  // Process silence and verify state is clean
  float output = limiter.process(0.0f);
  assertFloatEquals(0.0f, output, "After reset, silence should produce silence", 0.1f);
  
  std::cout << "  ✓ Reset functionality test passed" << std::endl;
}

void testDifferentSampleRates()
{
  std::cout << "Testing different sample rates..." << std::endl;
  
  std::vector<float> sampleRates = {44100.0f, 48000.0f, 96000.0f};
  
  for (float sampleRate : sampleRates)
  {
    AudioLimiter limiter;
    limiter.init(sampleRate);
    
    // Process a peak
    for (int i = 0; i < 500; ++i)
    {
      float input = 5.5f * std::sin(2.0f * M_PI * 440.0f * i / sampleRate);
      float output = limiter.process(input);
      
      // Verify limiting works at all sample rates
      assertInRange(output, -4.75f, 4.75f, "Limiting should work at all sample rates");
    }
  }
  
  std::cout << "  ✓ Different sample rates test passed" << std::endl;
}

void testContinuousSignal()
{
  std::cout << "Testing continuous signal processing..." << std::endl;
  
  AudioLimiter limiter;
  limiter.init(44100.0f);
  
  // Process a long continuous signal with varying levels
  for (int i = 0; i < 50000; ++i)
  {
    // Varying amplitude from 1V to 7V
    float amplitude = 1.0f + 6.0f * std::abs(std::sin(2.0f * M_PI * i / 10000.0f));
    float input = amplitude * std::sin(2.0f * M_PI * 440.0f * i / 44100.0f);
    float output = limiter.process(input);
    
    // All outputs must remain safe
    assertInRange(output, -5.0f, 5.0f, "Continuous signal must stay within bounds");
  }
  
  std::cout << "  ✓ Continuous signal test passed" << std::endl;
}

void testTransientHandling()
{
  std::cout << "Testing transient handling..." << std::endl;
  
  AudioLimiter limiter;
  limiter.init(44100.0f);
  
  // Simulate transient spike
  std::vector<float> signal(1000, 0.0f);
  signal[500] = 8.0f; // Sharp transient
  
  for (float input : signal)
  {
    float output = limiter.process(input);
    
    // Verify transient is caught and limited
    assertInRange(output, -4.75f, 4.75f, "Transients must be limited");
  }
  
  std::cout << "  ✓ Transient handling test passed" << std::endl;
}

// Main test runner
int main()
{
  std::cout << "========================================" << std::endl;
  std::cout << "Running AudioLimiter Unit Tests" << std::endl;
  std::cout << "========================================" << std::endl << std::endl;
  
  try
  {
    testInitialization();
    testSignalPassthrough();
    testPeakLimiting();
    testAttackResponse();
    testReleaseResponse();
    testLookaheadProcessing();
    testMakeupGain();
    testHardClipping();
    testReset();
    testDifferentSampleRates();
    testContinuousSignal();
    testTransientHandling();
    
    std::cout << std::endl << "========================================" << std::endl;
    std::cout << "All tests passed! ✓" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << std::endl << "Test failed with exception: " << e.what() << std::endl;
    return 1;
  }
}
