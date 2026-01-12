# ReGenEcho Module Documentation

## Overview

ReGenEcho is a granular stochastic sample manipulation module that uses generative echo techniques to alter and transform audio samples in real-time. Based on Iannis Xenakis's Gendy (Stochastic Synthesis) principles, ReGenEcho applies granular synthesis techniques to a continuously updated sample buffer.

## Features

- **Sample Buffer**: Real-time audio sampling with configurable buffer length
- **Granular Processing**: Break-point based granular synthesis with stochastic control
- **Envelope Shaping**: Multiple envelope types for grain windowing (SIN, TRI, HANN, WELCH)
- **Stochastic Control**: Three probability distributions (LINEAR, CAUCHY, ARCSINE)
- **Mirror/Wrap Modes**: Control how parameter changes wrap or mirror at boundaries
- **Accumulate Mode**: Accumulate amplitude changes over time for evolving textures
- **Built-in Limiter**: Integrated audio limiter for speaker protection and anti-clipping

## Parameters

### Sample Length (SLEN)
Controls the length of the sample buffer being processed.
- **Range**: 0.01 to 1.0 (10% to 100% of maximum buffer)
- **Default**: 1.0 (full buffer)

### Breakpoint Spacing (BPTS)
Determines the spacing between breakpoints in the sample buffer.
- **Range**: 0 to 2200 samples
- **Default**: 800 samples
- **CV Input**: Modulate spacing with external voltage
- **CV Amount**: 0.0 to 1.0

### Amplitude Step (ASTP)
Controls the maximum random step size for amplitude changes.
- **Range**: 0.0 to 0.6
- **Default**: 0.2
- **CV Input**: Modulate step size with external voltage
- **CV Amount**: 0.0 to 1.0

### Duration Step (DSTP)
Controls the maximum random step size for duration changes.
- **Range**: 0.0 to 0.2
- **Default**: 0.1
- **CV Input**: Modulate step size with external voltage
- **CV Amount**: 0.0 to 1.0

### Envelope Type (ENVS)
Selects the envelope shape for grain windowing.
- **1**: Sine wave envelope
- **2**: Triangle envelope
- **3**: Hann window
- **4**: Welch window
- **Default**: 2 (Triangle)

### Accumulate Mode (ACCM)
Toggle to accumulate amplitude changes over time.
- **Off**: Each breakpoint starts from zero
- **On**: Changes accumulate, creating evolving textures

### Mirror Mode (MIRR)
Controls how parameters wrap at boundaries.
- **Off**: Wrap around (modulo)
- **On**: Mirror/reflect at boundaries

### Probability Distribution (PDST)
Selects the probability distribution for random changes.
- **0**: Linear distribution
- **1**: Cauchy distribution (heavier tails)
- **2**: Arcsine distribution (bimodal)
- **Default**: 0 (Linear)

## Inputs

### AUDIO (WAV0)
Main audio input for sampling. This signal is continuously written to the buffer when the gate is active.

### GATE
Trigger to start recording new audio into the sample buffer. On each trigger:
- Resets the buffer write position
- Clears accumulated breakpoint values
- Begins fresh recording

### RESET (RSET)
Resets the sample buffer to its last recorded state and clears all accumulated changes.

### CV Inputs
- **BPTS**: Modulates breakpoint spacing
- **ASTP**: Modulates amplitude step size
- **DSTP**: Modulates duration step size

## Output

### OUT
Processed audio output with integrated limiter for speaker protection.

## Usage Tips

### Basic Echo Effect
1. Set BPTS to around 1000-1500 for clear echoes
2. Keep ASTP and DSTP relatively low (0.1-0.2)
3. Use Triangle or Hann envelope
4. Leave Accumulate mode OFF

### Evolving Textures
1. Enable Accumulate mode
2. Use longer sample buffer (SLEN = 0.8-1.0)
3. Moderate ASTP (0.2-0.3) and lower DSTP (0.05-0.15)
4. Experiment with Cauchy distribution for more dramatic changes

### Glitchy Effects
1. Short breakpoint spacing (BPTS = 200-600)
2. High amplitude and duration steps
3. Enable Mirror mode
4. Use Arcsine distribution

### Ambient Soundscapes
1. Long sample buffer
2. Low amplitude and duration steps
3. Enable Accumulate mode
4. Use Welch or Hann envelope
5. Linear or Cauchy distribution

## Signal Flow

```
Audio In → Sample Buffer → Granular Processing → Limiter → Audio Out
              ↑                      ↑
           Gate/Reset          Stochastic Control
```

## Technical Details

- **Sample Rate**: Follows VCV Rack engine sample rate
- **Buffer Size**: 44100 samples maximum (~1 second at 44.1kHz)
- **Limiter**: Automatic gain reduction with soft-knee compression
- **Processing**: Real-time granular resynthesis with envelope windowing

## Presets

Several factory presets are included:
- **Gentle Echo**: Subtle, musical echo effect
- **Chaotic Reflections**: Complex, evolving echoes
- **Ambient Texture**: Smooth, atmospheric processing
- **Granular Dreams**: Dreamy, accumulated textures
- **Glitchy Buffer**: Short, glitchy buffer manipulations

## Credits

Original GenEcho design by Samuel Laing (2019)
Adapted for VCV Rack as ReGenEcho
