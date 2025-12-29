# QuickStart Guide

Get started with ReGrandy in minutes! This guide will walk you through your first patches and introduce you to the core concepts of Granular Dynamic Stochastic Synthesis.

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Your First Sound](#your-first-sound)
3. [Understanding the Parameters](#understanding-the-parameters)
4. [Tutorial 1: Smooth Pad](#tutorial-1-smooth-pad)
5. [Tutorial 2: Evolving Texture](#tutorial-2-evolving-texture)
6. [Tutorial 3: FM Bell Tones](#tutorial-3-fm-bell-tones)
7. [Tutorial 4: Chaotic Noise](#tutorial-4-chaotic-noise)
8. [Working with CV Modulation](#working-with-cv-modulation)
9. [Troubleshooting](#troubleshooting)
10. [Next Steps](#next-steps)

---

## Prerequisites

Before you begin:
- ✅ VCV Rack 2.x installed
- ✅ RestocK plugin installed (see [Installation](README.md#installation))
- ✅ Basic familiarity with VCV Rack interface
- ✅ Audio output configured in VCV Rack

---

## Your First Sound

Let's create your first sound in under 60 seconds!

### Step 1: Add the Module

1. Right-click in an empty area of your patch
2. Navigate to **RestocK → ReGrandy**
3. The module will appear in your patch

### Step 2: Connect Audio Output

1. Click and drag from ReGrandy's **SINE OUTPUT** (bottom left jack)
2. Connect to your audio output module (usually VCV Audio-8 or similar)
3. Make sure your audio module is configured and unmuted

### Step 3: Make Some Noise!

1. **Turn up FREQ** (top left large knob) - you should hear a tone
2. **Increase BPTS** (second large knob) to ~12
3. **Add randomness** - slowly turn up ASTP and DSTP to 0.3
4. **Add texture** - turn up GRAT (bottom left) to 0

🎉 **Congratulations!** You're now generating stochastic synthesis!

---

## Understanding the Parameters

Before diving into tutorials, let's understand what each control does:

### The Big Four

| Parameter | What It Does | Start With |
|-----------|-------------|------------|
| **FREQ** | Base pitch of the sound | Middle position (C4) |
| **BPTS** | Complexity of the waveform | 8-15 |
| **ASTP** | How much the volume varies randomly | 0.2 |
| **DSTP** | How fast the variations happen | 0.2 |

### Grain Control

| Parameter | What It Does | Start With |
|-----------|-------------|------------|
| **GRAT** | Speed of the grain envelope | -3 to 0 for smooth |

### FM Synthesis

| Parameter | What It Does | Start With |
|-----------|-------------|------------|
| **FCAR** | FM carrier pitch | Middle |
| **FMOD** | FM modulator pitch | -2 (lower) |
| **IMOD** | FM intensity | 0.5 |

### Switches

| Switch | What It Does | Try |
|--------|-------------|-----|
| **PDST** | Random distribution type | Start with LEFT (Linear) |
| **FMTR** | Enable FM mode | Start DOWN (FM off) |
| **MIRR** | Boundary behavior | Start DOWN (Wrap) |

---

## Tutorial 1: Smooth Pad

**Goal**: Create a smooth, evolving pad sound perfect for ambient music.

### The Patch

![Patch Diagram]
```
ReGrandy → VCF (Lowpass) → Reverb → Audio Out
```

### Settings

1. **Reset to defaults** - Right-click ReGrandy → Initialize
2. **Set main parameters:**
   - FREQ: **0** (C4, 261Hz)
   - BPTS: **12**
   - ASTP: **0.15** (subtle variation)
   - DSTP: **0.1** (slow changes)
   - GRAT: **-3** (slow grain movement)

3. **Set switches:**
   - PDST: **LEFT** (Linear)
   - FMTR: **DOWN** (FM off)
   - MIRR: **UP** (Mirroring on)

4. **Optional envelope:**
   - ENVS: **4** (Tukey window)

### Expected Result

A smooth, slowly evolving pad with gentle variations. The sound should be organic but not chaotic.

### Tweaking Tips

- **Brighter**: Increase GRAT to -1 or 0
- **More movement**: Increase ASTP to 0.25
- **Thicker**: Increase BPTS to 18-20
- **Slower evolution**: Decrease DSTP to 0.05

### Audio Processing

Add these modules for best results:
- **VCF-1**: Lowpass filter with cutoff ~1000Hz
- **Plateau** or similar: Reverb with 3-5 second decay
- Optional: Chorus for width

---

## Tutorial 2: Evolving Texture

**Goal**: Create a complex, ever-changing texture with character.

### The Patch

```
ReGrandy → VCA (with envelope) → Delay → Audio Out
          ↑
       LFO (slow)
```

### Settings

1. **Main parameters:**
   - FREQ: **-1** (lower, around 130Hz)
   - BPTS: **25** (high complexity)
   - ASTP: **0.4** (moderate variation)
   - DSTP: **0.35** (moderate speed)
   - GRAT: **0** (neutral grain speed)

2. **Switches:**
   - PDST: **CENTER** (Cauchy distribution)
   - FMTR: **DOWN** (FM off)
   - MIRR: **DOWN** (Wrapping)

3. **Envelope:**
   - ENVS: **2** (Triangle)

### Modulation Setup

1. Add an **LFO** (like Fundamental VCO-1)
2. Set LFO frequency very slow (~0.1 Hz)
3. Connect LFO output to:
   - FREQ CV input (set attenuator to 0.3)
   - ASTP CV input (set attenuator to 0.5)

### Expected Result

A rich, organic texture that slowly morphs over time. The Cauchy distribution creates occasional surprising jumps, keeping the sound interesting.

### Exploration Ideas

- **Switch PDST** to Arcsine (RIGHT) for bimodal behavior
- **Increase DSTP** to 0.6 for faster changes
- **Try ENVS 3** (Hann) for smoother grains
- **Patch BPTS CV** with another slow LFO

---

## Tutorial 3: FM Bell Tones

**Goal**: Create bell-like, metallic tones using FM synthesis.

### The Patch

```
Trigger → ReGrandy → VCA → Reverb → Audio Out
           ↓
        Envelope
```

### Settings

1. **Main parameters:**
   - FREQ: **+1** (higher, ~520Hz)
   - BPTS: **6** (low for clearer pitch)
   - ASTP: **0.5** (allows dynamics)
   - DSTP: **0.15** (some variation)
   - GRAT: **+1** (faster grains)

2. **FM parameters:**
   - FCAR: **0** (carrier at base frequency)
   - FMOD: **+3** (high modulator, inharmonic)
   - IMOD: **2.5** (heavy modulation)

3. **Switches:**
   - PDST: **LEFT** (Linear)
   - **FMTR: UP** (FM ON!)
   - MIRR: **UP** (Mirroring)

4. **Envelope:**
   - ENVS: **3** (Hann window)

### Envelope Setup

1. Add an **ADSR** envelope
2. Settings:
   - Attack: 1ms
   - Decay: 1-3 seconds
   - Sustain: 0
   - Release: 2 seconds
3. Trigger with clock or sequencer
4. Patch envelope output to VCA

### Expected Result

Struck bell or gong-like tones with complex inharmonic partials. Each trigger creates a unique variation.

### Tuning the Bell

- **Higher pitch**: Increase FREQ or FCAR
- **More metallic**: Increase IMOD
- **Softer**: Decrease IMOD to 1.0
- **Different harmonics**: Change FMOD ratio
- **More variation**: Increase ASTP

### Harmonic Ratios to Try

| FCAR | FMOD | Character |
|------|------|-----------|
| 0 | +3 | Bright, inharmonic |
| 0 | -2 | Dark, gong-like |
| +1 | +4 | Very bright, gamelan |
| -1 | +0.5 | Warm, tubular |

---

## Tutorial 4: Chaotic Noise

**Goal**: Create wild, unpredictable noise textures for experimental music or sound effects.

### The Patch

```
ReGrandy → Waveshaper → Filter (HP+LP) → Audio Out
```

### Settings

1. **Main parameters:**
   - FREQ: **+2** (high, ~1000Hz)
   - BPTS: **40** (very high)
   - ASTP: **0.85** (extreme variation)
   - DSTP: **0.75** (fast changes)
   - GRAT: **+2.5** (very fast grains)

2. **Switches:**
   - PDST: **RIGHT** (Arcsine)
   - FMTR: **DOWN** (FM off, or try UP!)
   - MIRR: **DOWN** (Wrapping)

3. **Envelope:**
   - ENVS: **1** (Sine) or **2** (Triangle)

### Expected Result

Dense, noisy, chaotic texture with unpredictable variations. Great for:
- Sci-fi sound effects
- Drone music backgrounds
- Noise layers in electronic music
- "Broken transmission" effects

### Control the Chaos

Even chaos can be shaped:
- **Less harsh**: Lower ASTP to 0.6
- **Rhythmic chaos**: Reduce GRAT to +1
- **Pitched chaos**: Lower BPTS to 20-25
- **Tame it**: Switch PDST to Linear

### Processing Ideas

- **Waveshaper**: Add harmonics and saturation
- **HP Filter**: Remove rumble, focus on high noise
- **LP Filter**: Tame harsh highs
- **Bit crusher**: Add digital artifacts
- **Granular delay**: Layer the chaos

---

## Working with CV Modulation

CV modulation brings ReGrandy to life! Here's how to use it effectively.

### Basic CV Setup

Every major parameter has:
1. **CV Input jack** (bottom)
2. **Attenuator knob** (middle small knob)

**How to use:**
1. Patch a modulation source to CV input
2. Adjust attenuator to control modulation depth (0 = none, 1 = full)

### Modulation Sources to Try

| Source | Best For | Patch To |
|--------|----------|----------|
| **Slow LFO** (0.1-1 Hz) | Evolving textures | FREQ, ASTP, GRAT |
| **Fast LFO** (5-20 Hz) | Vibrato, tremolo | FREQ, GRAT |
| **Sample & Hold** | Stepped changes | BPTS, any parameter |
| **Envelope** | Shaped dynamics | ASTP, IMOD |
| **Sequencer** | Melodic patterns | FREQ |
| **Random** | Controlled chaos | DSTP, ASTP |

### Example CV Patch: Breathing Texture

```
Slow Sine LFO (0.2 Hz)
  ├─→ FREQ CV (attenuator: 0.3)
  ├─→ GRAT CV (attenuator: 0.5)
  └─→ ASTP CV (attenuator: 0.4)

Random CV (1 Hz)
  └─→ BPTS CV (attenuator: 0.2)
```

**Result**: Slow breathing motion with subtle random variations.

### CV Modulation Tips

✅ **Do:**
- Start with low attenuator values (0.2-0.3)
- Use slow modulation first
- Modulate multiple related parameters
- Experiment with inverted signals (use offset/inverter)

❌ **Don't:**
- Max out all attenuators (sounds chaotic)
- Modulate BPTS too fast (causes zipper noise)
- Use very fast modulation on FREQ without musical intent

---

## Troubleshooting

### No Sound

**Check:**
- ✅ Audio cable connected from SINE OUTPUT
- ✅ FREQ knob not at extreme positions
- ✅ VCV Rack audio engine running
- ✅ Audio output module configured
- ✅ Volume/gain not at zero

**Try:**
- Turn all CV attenuators to zero
- Initialize the module (right-click → Initialize)
- Check FMTR switch (try both positions)

### Sound is Too Quiet

**Solutions:**
- Increase ASTP (adds amplitude variation)
- Add a VCA or amplifier after ReGrandy
- Check that GRAT isn't at extreme negative values
- Try different ENVS settings

### Sound is Too Harsh/Noisy

**Solutions:**
- Reduce BPTS to 8-12
- Lower ASTP to 0.2 or less
- Lower GRAT to -2 or -3
- Switch PDST to Linear (left position)
- Add a lowpass filter
- Use ENVS 3 (Hann) for smoother grains

### Clicks or Pops

**Causes & Solutions:**
- **BPTS changing too fast**: Reduce BPTS CV modulation
- **DSTP too high**: Lower to 0.1-0.3
- **Envelope mismatch**: Try ENVS 3 (Hann) or 4 (Tukey)
- **FM with extreme IMOD**: Reduce IMOD

### Sound Keeps Repeating

This is expected! Stochastic synthesis creates evolving patterns, but with low randomness settings it can be periodic.

**To increase variation:**
- Increase ASTP to 0.3-0.5
- Increase DSTP to 0.2-0.4
- Increase BPTS to 15-25
- Switch PDST to Cauchy or Arcsine
- Add slow CV modulation

---

## Next Steps

### Explore Further

Now that you've mastered the basics:

1. **📖 Read the [Advanced Usage Guide](ADVANCED.md)**
   - Deep dive into synthesis theory
   - Advanced patching techniques
   - Performance optimization

2. **🔧 Check the [API Reference](API.md)**
   - Technical details
   - Parameter ranges
   - Algorithm explanations

3. **💡 Browse [Examples](examples/)**
   - Ready-to-use patches
   - Genre-specific presets
   - Creative techniques

4. **❓ Read the [FAQ](FAQ.md)**
   - Common questions
   - Tips and tricks
   - Known limitations

### Practice Exercises

To build your skills:

1. **Recreate a preset sound** from the [presets folder](../presets/ReGrandy/)
2. **Create 5 patches** using only ReGrandy + one filter
3. **Make a sequence** using CV to control FREQ
4. **Design a sound effect** for a sci-fi movie
5. **Build a generative patch** that evolves for 5 minutes

### Join the Community

- Share your patches on the [VCV Rack Community Forum](https://community.vcvrack.com/)
- Report issues on [GitHub](https://github.com/shortwavlabs/restock/issues)
- Tag your tracks with #ReGrandy

---

## Quick Reference Card

### Sweet Spot Settings

| Sound Type | FREQ | BPTS | ASTP | DSTP | GRAT | PDST | FM |
|------------|------|------|------|------|------|------|----|
| **Smooth Pad** | -1 to +1 | 10-15 | 0.1-0.2 | 0.1-0.2 | -3 to -1 | Linear | Off |
| **Evolving Texture** | -2 to 0 | 20-30 | 0.3-0.5 | 0.3-0.4 | -1 to +1 | Cauchy | Off |
| **Bell Tone** | 0 to +2 | 4-8 | 0.4-0.6 | 0.1-0.2 | 0 to +2 | Linear | On |
| **Noise** | +1 to +3 | 30-50 | 0.7-1.0 | 0.6-0.9 | +2 to +3 | Arcsine | Any |
| **Bass Drone** | -4 to -2 | 8-12 | 0.2-0.3 | 0.2-0.3 | -4 to -2 | Linear | Off |

### Processing Chain Suggestions

```
Ambient:     ReGrandy → LP Filter → Chorus → Reverb
Aggressive:  ReGrandy → Waveshaper → HP Filter → Delay
Clean:       ReGrandy → Envelope VCA → Light Reverb
Experimental: ReGrandy → Granular FX → Bit Crusher → Reverb
```

---

**Happy patching! 🎛️**

*Questions? See the [FAQ](FAQ.md) or [open an issue](https://github.com/shortwavlabs/restock/issues).*
