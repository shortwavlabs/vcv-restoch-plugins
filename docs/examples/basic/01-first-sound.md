# Example 01: First Sound

**Difficulty**: 🟢 Beginner  
**Category**: Basic  
**Time**: ~5 minutes  
**Modules Required**: ReGrandy, VCV Audio (or similar output)

## Description

Your first ReGrandy patch! This example gets you making sound in under 5 minutes and introduces the core parameters.

## Learning Goals

- Connect ReGrandy to audio output
- Understand the main frequency control
- Learn about breakpoints (BPTS)
- Hear stochastic synthesis in action

## The Patch

```
┌──────────┐     ┌───────────┐
│ ReGrandy │────▶│ VCV Audio │
│          │     │           │
└──────────┘     └───────────┘
```

Minimal setup: Just ReGrandy and an audio output.

## Settings

### ReGrandy Parameters

```
FREQ: 0.0        (C4, ~262 Hz)
BPTS: 12         (moderate complexity)
ASTP: 0.2        (gentle amplitude variation)
DSTP: 0.2        (gentle duration variation)
GRAT: 0.0        (grain rate matches frequency)
PDST: LINEAR     (left position)
FMTR: DOWN       (FM off)
MIRR: UP         (mirroring on)
ENVS: 4          (Tukey envelope)

All CV attenuators: 0.0 (no modulation)
```

### Audio Output

- Connect to your speakers/headphones
- Set volume to moderate level (~50%)

## Step-by-Step

### Step 1: Add Modules

1. Right-click in empty space
2. Add **RestocK → ReGrandy**
3. Add **Audio → Audio-8** (or your audio interface)
4. Position them side by side

### Step 2: Connect Audio

1. Click and drag from **ReGrandy SINE OUTPUT** (bottom left)
2. Connect to **Audio-8 input 1**
3. Make sure audio engine is running (green power button)

### Step 3: Initial Settings

1. **Initialize ReGrandy**: Right-click module → Initialize
2. Set **FREQ** knob to middle position (0.0)
3. Set **BPTS** to 12 (count from minimum)
4. You should hear a tone!

### Step 4: Add Variation

1. Slowly turn **ASTP** to 0.2 (about 20% up)
   - Notice the amplitude starting to vary
2. Turn **DSTP** to 0.2
   - Notice the timing starting to vary
3. The sound is now "alive" - it's not repeating exactly!

### Step 5: Experiment

Try these changes one at a time:

1. **Increase BPTS to 20**
   - More complex waveform
   - Richer harmonics

2. **Increase ASTP to 0.5**
   - More dramatic amplitude changes
   - More "character"

3. **Change FREQ**
   - Turn left: lower pitch
   - Turn right: higher pitch
   - Range is about 7 octaves!

4. **Try GRAT**
   - Turn to -2: slower grain movement
   - Turn to +1: faster, denser grains

## Expected Result

**What You Should Hear:**
- A clear tone at the frequency you set
- Gentle variations in volume and timbre
- The sound slowly evolving, never exactly repeating
- Organic, "living" quality

**What You Should NOT Hear:**
- Complete silence (check connections!)
- Harsh digital noise (lower ASTP/DSTP)
- Clicking or popping (use Hann or Tukey envelope)
- Pure sine wave (increase ASTP/DSTP)

## Variations

### Variation 1: More Chaos

```
BPTS: 30
ASTP: 0.6
DSTP: 0.5
PDST: CAUCHY (center position)
```

Result: More unpredictable, dramatic changes.

### Variation 2: Smoother

```
BPTS: 8
ASTP: 0.1
DSTP: 0.1
GRAT: -3
MIRR: UP
```

Result: Gentle, subtle variations.

### Variation 3: Faster Evolution

```
BPTS: 15
ASTP: 0.3
DSTP: 0.4
GRAT: +1
```

Result: Faster-changing, busier texture.

## Understanding What's Happening

### The Science

ReGrandy is creating a waveform from **breakpoints** (controlled by BPTS):

```
Amplitude
    ↑
 1.0│    ●───●
    │   /     \
 0.0│  ●       ●───●
    │               \
-1.0│                ●
    └────────────────────▶ Time
        Breakpoints randomly "walk"
```

Each breakpoint randomly moves:
- **ASTP**: How much amplitude can change per step
- **DSTP**: How much duration can change per step

### The Result

This creates a waveform that:
- Has a fundamental pitch (FREQ)
- Evolves organically (never repeats exactly)
- Has complex harmonic content (many breakpoints)
- Sounds "alive" (stochastic process)

## Tips

### Starting Tips

- **Too quiet?** Increase ASTP
- **Too harsh?** Decrease ASTP and DSTP
- **Want more pitch?** Decrease BPTS to 6-8
- **Want texture?** Increase BPTS to 20+

### Listening Tips

- Close your eyes and listen
- Notice how it never quite repeats
- Hear the "breathing" quality
- Compare to a regular oscillator (add VCO-1 to compare!)

### Next Steps

Once comfortable:
- Try the PDST switch (probability distributions)
- Experiment with MIRR (wrapping vs mirroring)
- Change ENVS (envelope types)
- Move to Example 02 for CV modulation!

## Common Issues

### No Sound

**Check:**
- ✅ Cable connected from SINE OUTPUT
- ✅ Audio engine running (green button)
- ✅ Audio output module configured
- ✅ Volume not at zero
- ✅ FREQ not at extreme low value

**Fix:**
- Right-click ReGrandy → Initialize
- Check VCV Rack audio settings

### Sound Too Harsh

**Fix:**
- Lower ASTP to 0.1
- Lower DSTP to 0.1
- Change ENVS to 3 (Hann)
- Lower BPTS to 8

### Sound Too Boring

**Fix:**
- Increase ASTP to 0.3-0.4
- Increase DSTP to 0.3
- Increase BPTS to 20+
- Try PDST = Cauchy

## Congratulations!

You've created your first stochastic synthesis patch!

**What You Learned:**
- ✅ How to connect ReGrandy
- ✅ Basic parameter functions
- ✅ Creating evolving sounds
- ✅ Adjusting for taste

**Next Steps:**
- [Example 02: Smooth Pad](02-smooth-pad.md)
- [QuickStart Guide](../../QUICKSTART.md)
- [API Reference](../../API.md)

---

**Created by**: Shortwav Labs  
**Difficulty**: Beginner  
**Version**: RestocK 2.0.1
**Last Updated**: 2024-12-28
