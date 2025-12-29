# ReGrandy Examples

This directory contains example patches and presets demonstrating various uses of the ReGrandy module.

## Directory Structure

```
examples/
├── README.md                    # This file
├── basic/                       # Simple starter patches
├── intermediate/                # More complex techniques
├── advanced/                    # Advanced sound design
├── generative/                  # Generative/algorithmic patches
├── performance/                 # Performance-ready patches
└── sound-effects/              # Sound design and FX
```

## Quick Start

1. Copy desired `.vcv` patch file to your VCV Rack patches folder
2. Open VCV Rack and load the patch
3. Read the accompanying `.md` file for notes and tips
4. Experiment with parameters!

## Example Categories

### Basic Examples

Perfect for learning ReGrandy fundamentals.

**Files:**
- `01-first-sound.md` - Your first ReGrandy patch
- `02-smooth-pad.md` - Creating ambient pads
- `03-fm-bells.md` - Bell-like tones with FM
- `04-controlled-noise.md` - Shaping chaotic textures

### Intermediate Examples

Explore CV modulation and routing techniques.

**Files:**
- `05-evolving-texture.md` - Slow modulation techniques
- `06-rhythmic-sequence.md` - Melodic generative sequences
- `07-multi-band-processing.md` - Frequency-split processing
- `08-dual-oscillator.md` - Two ReGrandy modules together

### Advanced Examples

Deep sound design and complex patches.

**Files:**
- `09-xenakis-cloud.md` - Orchestral stochastic clouds
- `10-granular-feedback.md` - Self-modulation patches
- `11-fm-complex.md` - Advanced FM techniques
- `12-spectral-freeze.md` - Frozen spectral textures

### Generative Examples

Algorithmic and self-evolving patches.

**Files:**
- `13-infinite-variation.md` - Never-repeating textures
- `14-probability-music.md` - Different distributions compared
- `15-self-patching.md` - Output modulating input
- `16-clock-sync.md` - Tempo-synchronized evolution

### Performance Examples

Ready-to-use performance patches.

**Files:**
- `17-ambient-bed.md` - Background ambient layer
- `18-lead-voice.md` - Melodic lead sounds
- `19-bass-drone.md` - Deep bass foundations
- `20-percussion-layer.md` - Rhythmic/percussive elements

### Sound Effects Examples

Sound design for film, games, and media.

**Files:**
- `21-sci-fi-transmission.md` - Alien communications
- `22-underwater.md` - Subaquatic ambience
- `23-wind-atmosphere.md` - Natural wind sounds
- `24-mechanical-robot.md` - Robotic/mechanical sounds

---

## Example Templates

Each example follows this format:

### Example Name

**Difficulty**: Beginner | Intermediate | Advanced  
**Category**: [Category name]  
**Time**: ~X minutes to complete  
**Modules Required**: ReGrandy + [other modules]

#### Description

Brief description of what this patch demonstrates.

#### Learning Goals

- Goal 1
- Goal 2
- Goal 3

#### The Patch

```
[ASCII diagram or description of patch]
```

#### Settings

**ReGrandy Parameters:**
```
FREQ: X
BPTS: X
ASTP: X
DSTP: X
GRAT: X
[etc.]
```

**Other Modules:**
```
[Settings for other modules]
```

#### Step-by-Step

1. Step one
2. Step two
3. [etc.]

#### Expected Result

What you should hear and see.

#### Variations

- Try this...
- Change that...
- Experiment with...

#### Tips

- Helpful tip 1
- Helpful tip 2

---

## Creating Your Own Examples

Want to contribute an example? Great!

### Guidelines

1. **Start Simple**: Build up complexity gradually
2. **Document Clearly**: Explain every step
3. **Include Settings**: List all parameter values
4. **Explain Why**: Not just what, but why
5. **Test Thoroughly**: Verify it works on clean VCV install

### Submission Process

1. Create your example following the template
2. Test on a fresh VCV Rack installation
3. Write clear documentation
4. Submit PR to repository

See [CONTRIBUTING.md](../CONTRIBUTING.md) for details.

---

## Example Index by Skill Level

### 🟢 Beginner (No prior experience needed)

| # | Name | Description |
|---|------|-------------|
| 01 | First Sound | Getting audio out of ReGrandy |
| 02 | Smooth Pad | Simple ambient textures |
| 03 | FM Bells | Basic FM synthesis |
| 04 | Controlled Noise | Shaping chaos |

### 🟡 Intermediate (Some VCV/synthesis experience)

| # | Name | Description |
|---|------|-------------|
| 05 | Evolving Texture | LFO modulation techniques |
| 06 | Rhythmic Sequence | Melodic sequencing |
| 07 | Multi-Band Processing | Frequency splitting |
| 08 | Dual Oscillator | Multiple instances |

### 🔴 Advanced (Experienced users)

| # | Name | Description |
|---|------|-------------|
| 09 | Xenakis Cloud | Complex orchestral textures |
| 10 | Granular Feedback | Self-modulation |
| 11 | FM Complex | Advanced FM techniques |
| 12 | Spectral Freeze | Frozen spectra |

### 🔵 Generative (Algorithmic composition)

| # | Name | Description |
|---|------|-------------|
| 13 | Infinite Variation | Never-repeating |
| 14 | Probability Music | Distribution comparison |
| 15 | Self-Patching | Feedback routing |
| 16 | Clock Sync | Tempo synchronization |

### 🟣 Performance (Stage-ready)

| # | Name | Description |
|---|------|-------------|
| 17 | Ambient Bed | Background layer |
| 18 | Lead Voice | Melodic leads |
| 19 | Bass Drone | Deep foundations |
| 20 | Percussion Layer | Rhythmic elements |

### 🟠 Sound Effects (Sound design)

| # | Name | Description |
|---|------|-------------|
| 21 | Sci-Fi Transmission | Alien sounds |
| 22 | Underwater | Aquatic ambience |
| 23 | Wind Atmosphere | Natural wind |
| 24 | Mechanical Robot | Robotic sounds |

---

## Example by Parameter Focus

### Focusing on BPTS (Breakpoints)

- Example 01: Low BPTS (simple waveforms)
- Example 09: High BPTS (complex textures)
- Example 14: Variable BPTS via CV

### Focusing on ASTP/DSTP (Randomness)

- Example 02: Low randomness (smooth)
- Example 04: High randomness (chaotic)
- Example 13: Modulated randomness

### Focusing on GRAT (Grain Rate)

- Example 03: Fast grains (bell-like)
- Example 12: Very fast grains (spectral)
- Example 19: Slow grains (wobbles)

### Focusing on FM Synthesis

- Example 03: Basic FM
- Example 11: Complex FM
- Example 24: FM for sound effects

### Focusing on Probability Distributions

- Example 02: Linear (smooth)
- Example 05: Cauchy (surprises)
- Example 04: Arcsine (extremes)

---

## Example by Musical Genre

### Ambient / Drone

- Example 02: Smooth Pad
- Example 12: Spectral Freeze
- Example 17: Ambient Bed
- Example 19: Bass Drone

### Experimental / Noise

- Example 04: Controlled Noise
- Example 09: Xenakis Cloud
- Example 21: Sci-Fi Transmission

### Melodic / Rhythmic

- Example 06: Rhythmic Sequence
- Example 18: Lead Voice
- Example 20: Percussion Layer

### Generative / Algorithmic

- Example 13: Infinite Variation
- Example 15: Self-Patching
- Example 16: Clock Sync

### Sound Design

- Example 21: Sci-Fi Transmission
- Example 22: Underwater
- Example 23: Wind Atmosphere
- Example 24: Mechanical Robot

---

## Preset Collection

In addition to full patches, the `presets/ReGrandy/` folder contains parameter presets:

**Factory Presets:**
- Abyss.vcvm
- Aftermath.vcvm
- Alarm.vcvm
- Drone.vcvm
- Emperor.vcvm
- Glass.vcvm
- Haunted.vcvm
- Interference.vcvm
- Noise Pollution.vcvm
- Ocean Waves.vcvm
- Planes.vcvm
- Razor Blade.vcvm
- Rumble.vcvm
- Soothe.vcvm
- UFO Transmission.vcvm

**Using Presets:**
1. Right-click ReGrandy module
2. Select "Preset"
3. Choose desired preset
4. Adjust to taste

---

## Video Tutorials

(To be added)

Links to video tutorials demonstrating these examples will be added here.

---

## Community Examples

Share your examples!

**Submission:**
1. Create example following template
2. Test thoroughly
3. Submit PR or post in discussions
4. Get featured here!

**Community Showcase:**
- [To be populated with community contributions]

---

## Troubleshooting Examples

If an example doesn't work:

1. **Check VCV Rack Version**: Examples are for 2.x
2. **Check RestocK Version**: Should match example version
3. **Check Module Availability**: All required modules installed?
4. **Reset Modules**: Right-click → Initialize
5. **Check Documentation**: Read the `.md` file carefully

---

## Resources

- [QuickStart Guide](../QUICKSTART.md) - Basic usage
- [Advanced Guide](../ADVANCED.md) - Deep techniques
- [API Reference](../API.md) - Technical details
- [FAQ](../FAQ.md) - Common questions

---

## License

All examples are provided under the same GPL-3.0-or-later license as RestocK.

Feel free to use, modify, and share!

---

**Happy patching!** 🎛️✨

*Examples maintained by Shortwav Labs*  
*Last Updated: 2024-12-28*
