# Example 09: Xenakis-Style Orchestral Cloud

**Difficulty**: 🔴 Advanced  
**Category**: Sound Design  
**Time**: ~30 minutes  
**Modules Required**: ReGrandy x3, Mixer, VCF, Reverb, LFO

## Description

Create dense, evolving textural clouds inspired by Iannis Xenakis's orchestral works. This advanced patch uses multiple ReGrandy instances at different pitch ranges to create massive, evolving soundscapes.

## Learning Goals

- Layer multiple ReGrandy instances effectively
- Use different probability distributions for variety
- Create frequency-balanced complex textures
- Apply processing for orchestral depth
- Understand long-form evolution techniques

## The Patch

```
┌──────────┐
│  LFO     │ (0.05 Hz - very slow)
│  Sine    │
└─┬──┬──┬──┘
  │  │  │
  │  │  └─────────┐
  │  └────────┐   │
  │           │   │
  ▼           ▼   ▼
┌────────┐ ┌────────┐ ┌────────┐
│ReGrandy│ │ReGrandy│ │ReGrandy│
│  LOW   │ │  MID   │ │  HIGH  │
└───┬────┘ └───┬────┘ └───┬────┘
    │          │          │
    └──────┬───┴───┬──────┘
           ▼       ▼
        ┌─────────────┐
        │   Mixer     │
        │ 40/40/20%   │
        └──────┬──────┘
               ▼
        ┌─────────────┐
        │  HP Filter  │
        │   80 Hz     │
        └──────┬──────┘
               ▼
        ┌─────────────┐
        │  LP Filter  │
        │   8 kHz     │
        └──────┬──────┘
               ▼
        ┌─────────────┐
        │   Reverb    │
        │  6s decay   │
        └──────┬──────┘
               ▼
        ┌─────────────┐
        │   Audio     │
        │   Output    │
        └─────────────┘
```

## Settings

### ReGrandy 1 (LOW Range)

**Parameters:**
```
FREQ: -3.0       (sub-bass, ~33 Hz)
BPTS: 25         (high complexity)
ASTP: 0.4        (moderate variation)
DSTP: 0.3        (moderate speed)
GRAT: -2.0       (slow grain movement)

PDST: CAUCHY     (center - occasional surprises)
FMTR: DOWN       (FM off)
MIRR: UP         (mirroring)
ENVS: 3          (Hann)

CV Inputs:
- FREQ CV: LFO → attenuator 0.2
```

### ReGrandy 2 (MID Range)

**Parameters:**
```
FREQ: 0.0        (mid range, ~262 Hz)
BPTS: 30         (very high complexity)
ASTP: 0.5        (higher variation)
DSTP: 0.4        (faster changes)
GRAT: 0.0        (neutral grain speed)

PDST: LINEAR     (left - smooth evolution)
FMTR: DOWN       (FM off)
MIRR: UP         (mirroring)
ENVS: 3          (Hann)

CV Inputs:
- FREQ CV: LFO → attenuator 0.3
- ASTP CV: LFO → attenuator 0.2
```

### ReGrandy 3 (HIGH Range)

**Parameters:**
```
FREQ: +2.0       (high range, ~1046 Hz)
BPTS: 35         (highest complexity)
ASTP: 0.6        (high variation)
DSTP: 0.5        (fast changes)
GRAT: +1.0       (faster grains)

PDST: ARCSINE    (right - extreme values)
FMTR: DOWN       (FM off)
MIRR: DOWN       (wrapping for jumps)
ENVS: 4          (Tukey)

CV Inputs:
- FREQ CV: LFO → attenuator 0.25
- GRAT CV: LFO → attenuator 0.15
```

### LFO Settings

```
Waveform: Sine
Frequency: 0.05 Hz (20 second cycle)
Outputs: Connect to multiple ReGrandy CV inputs
```

### Mixer

```
Channel 1 (LOW): 40%
Channel 2 (MID): 40%
Channel 3 (HIGH): 20%

VU meters should show balanced activity
```

### Filters

**High-Pass:**
```
Type: 24dB/oct
Cutoff: 80 Hz
Resonance: 0.1
```

**Low-Pass:**
```
Type: 12dB/oct or 24dB/oct
Cutoff: 8000 Hz
Resonance: 0.3
```

### Reverb

```
Size/Room: Large (hall)
Decay Time: 6-8 seconds
Pre-delay: 20-40ms
Damping: Medium
Mix: 30-50% (taste)
```

## Step-by-Step

### Phase 1: Basic Setup (10 minutes)

1. **Add three ReGrandy modules**
   - Label them LOW, MID, HIGH (right-click → rename)

2. **Initialize all modules**
   - Right-click each → Initialize

3. **Set frequency ranges**
   - LOW: FREQ = -3
   - MID: FREQ = 0
   - HIGH: FREQ = +2

4. **Add basic mixer**
   - 3+ channel mixer
   - Connect all three outputs

### Phase 2: Configure Complexity (10 minutes)

5. **Set breakpoints (BPTS)**
   - LOW: 25
   - MID: 30
   - HIGH: 35

6. **Set randomness (ASTP/DSTP)**
   - LOW: ASTP=0.4, DSTP=0.3
   - MID: ASTP=0.5, DSTP=0.4
   - HIGH: ASTP=0.6, DSTP=0.5

7. **Set grain rates (GRAT)**
   - LOW: -2
   - MID: 0
   - HIGH: +1

8. **Listen and balance**
   - Adjust mixer levels
   - Aim for 40/40/20 balance

### Phase 3: Add Character (5 minutes)

9. **Set probability distributions**
   - LOW: Cauchy (surprise elements)
   - MID: Linear (smooth foundation)
   - HIGH: Arcsine (extreme variations)

10. **Set mirror modes**
    - LOW: Mirror (smooth)
    - MID: Mirror (smooth)
    - HIGH: Wrap (dramatic)

11. **Set envelopes**
    - LOW: Hann (smooth)
    - MID: Hann (smooth)
    - HIGH: Tukey (present)

### Phase 4: Add Modulation (5 minutes)

12. **Add slow LFO**
    - Sine wave, 0.05 Hz

13. **Connect modulation**
    - LFO → LOW FREQ CV (0.2)
    - LFO → MID FREQ CV (0.3)
    - LFO → MID ASTP CV (0.2)
    - LFO → HIGH FREQ CV (0.25)
    - LFO → HIGH GRAT CV (0.15)

### Phase 5: Processing (5 minutes)

14. **Add high-pass filter**
    - 80 Hz cutoff
    - Remove rumble

15. **Add low-pass filter**
    - 8 kHz cutoff
    - Tame highs

16. **Add reverb**
    - Large hall
    - 6s decay
    - 30-40% mix

17. **Final balance**
    - Adjust mix levels
    - Set output volume

## Expected Result

**What You Should Hear:**

**First 30 seconds:**
- Dense, complex texture emerging
- Multiple layers blending
- No clear individual pitches
- Oceanic, massive quality

**After 1 minute:**
- Slow evolution becoming apparent
- LFO causing gentle frequency shifts
- Different layers moving independently
- Breathing, living quality

**After 3 minutes:**
- Long-form structure emerging
- Cauchy surprises in low end
- Arcsine extremes in high end
- Never repeating exactly

**Overall Character:**
- Orchestral density (like massed strings/winds)
- Xenakis-like stochastic cloud
- Organic, not electronic
- Suitable for ambient or film scoring

**What You Should NOT Hear:**
- Individual grain clicks
- Harsh digital artifacts
- Pure periodic repetition
- Identifiable pitches

## Variations

### Variation 1: Darker Cloud

```
All FREQ: -1 octave lower
Mixer: 50/40/10 (more low)
LP Filter: 4 kHz (darker)
Reverb: 8s decay, more damping
```

### Variation 2: Brighter, More Active

```
All FREQ: +1 octave higher
All GRAT: +1 faster
All ASTP: +0.1 (more variation)
LP Filter: Remove or 12 kHz
```

### Variation 3: Rhythmic Pulses

```
Replace LFO with slow square wave (0.1 Hz)
All ASTP CV: attenuator 0.8
Result: Breathing/pulsing texture
```

### Variation 4: Maximum Chaos

```
All PDST: Cauchy
All MIRR: Wrap
All ASTP: 0.8
All DSTP: 0.7
Result: Wild, unpredictable cloud
```

## Advanced Techniques

### Dynamic Density

Add envelope follower to control reverb size:
```
Mixer Output → Envelope Follower → Reverb Size CV
```

### Frequency-Dependent Modulation

Modulate each range differently:
```
LFO 1 (0.05 Hz) → LOW
LFO 2 (0.08 Hz) → MID
LFO 3 (0.12 Hz) → HIGH
```

### Spectral Evolution

Slowly sweep filters over time:
```
LFO (0.02 Hz) → LP Filter Cutoff
Range: 2 kHz to 10 kHz
```

## Understanding the Design

### Why Three Layers?

**LOW (sub-bass):**
- Foundation, felt more than heard
- Slow movement (GRAT -2)
- Cauchy for occasional depth shifts

**MID (body):**
- Main tonal content
- Balanced evolution
- Linear distribution for smoothness

**HIGH (air):**
- Sparkle and shimmer
- Fast grains (GRAT +1)
- Arcsine for dramatic movement

### Why These Distributions?

- **Cauchy** in LOW: Adds unpredictable bass shifts
- **Linear** in MID: Stable foundation
- **Arcsine** in HIGH: Sparkle and drama

### Why These Settings?

**High BPTS (25-35):**
- Creates spectral density
- Many partials = orchestral quality
- Smooth, non-pitched quality

**High ASTP/DSTP:**
- Constant evolution
- Never static
- Organic character

**Slow GRAT in LOW:**
- Prevents "buzz" in bass
- Creates slow wobbles
- Sub-bass clarity

## Performance Tips

### Live Performance

**Manual Controls:**
- Master volume: Mixer output level
- Brightness: LP filter cutoff
- Depth: Reverb mix
- Intensity: MID ASTP knob

**Preset Variations:**
- Save 3-4 variations
- Morph between during performance
- Use MIDI controller for smooth transitions

### Recording Tips

- Record 5+ minute takes (long evolution)
- Capture multiple passes (never identical)
- Layer different takes for uber-density
- Use automation on filter cutoffs

### CPU Optimization

This patch is CPU-intensive. To optimize:
- Reduce BPTS if needed (25/25/30 still works)
- Use Linear distribution (most efficient)
- Increase buffer size in VCV settings
- Consider rendering sections to audio

**CPU Usage:**
- Expected: 15-25% of single core
- If higher: Reduce BPTS or use 2 instances instead of 3

## Xenakis Comparison

This technique is inspired by Xenakis's works like:
- **Pithoprakta** (1955-56) - String textures
- **Metastasis** (1953-54) - Glissandi clouds
- **Terretektorh** (1965-66) - Spatial masses

**Key Similarities:**
- Stochastic processes creating mass textures
- Many independent voices (breakpoints)
- Statistical control rather than individual notes
- Emergence of structure from randomness

**Differences:**
- Xenakis: Discrete instruments playing independently
- ReGrandy: Continuous synthesis with granular processing
- Both: Achieve similar sonic results!

## Troubleshooting

### Too Muddy

- Increase HP filter to 120 Hz
- Reduce LOW mixer level to 30%
- Lower BPTS in LOW and MID

### Too Harsh

- Decrease all ASTP by 0.1
- Lower LP filter to 6 kHz
- Change all envelopes to Hann
- Switch HIGH to Linear distribution

### Not Enough Movement

- Increase LFO frequency to 0.08 Hz
- Raise all CV attenuators by 0.1
- Increase all DSTP values
- Try different LFO waveforms

### CPU Too High

- Reduce BPTS: LOW=20, MID=25, HIGH=30
- Remove one ReGrandy (use 2 layers)
- Use simpler filter modules
- Increase VCV Rack buffer size

## Next Steps

**After mastering this:**
- Try 4+ layers (add ultra-low and ultra-high)
- Add spatial panning to each layer
- Use different reverbs per frequency band
- Automate more parameters over time
- Combine with external audio processing

**Related Examples:**
- [Example 12: Spectral Freeze](../advanced/12-spectral-freeze.md)
- [Example 15: Self-Patching](../generative/15-self-patching.md)
- [Example 22: Underwater](../sound-effects/22-underwater.md)

## Further Reading

- [Advanced Guide - Sound Design Recipes](../../ADVANCED.md#sound-design-recipes)
- [API Reference - GendyOscillator](../../API.md#gendyoscillator-class)
- Xenakis, I. *Formalized Music* (1992)

---

**Created by**: Shortwav Labs  
**Difficulty**: Advanced  
**Version**: RestocK 2.0.1
**Last Updated**: 2024-12-28

**Estimated CPU**: 15-25%  
**Recommended RAM**: 4GB+  
**Best for**: Ambient, Film Scoring, Experimental
