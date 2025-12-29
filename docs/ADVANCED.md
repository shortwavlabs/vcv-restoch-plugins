# Advanced Usage Guide

Comprehensive guide to mastering ReGrandy's advanced features, synthesis techniques, and creative applications.

## Table of Contents

1. [Understanding Dynamic Stochastic Synthesis](#understanding-dynamic-stochastic-synthesis)
2. [Probability Distributions Deep Dive](#probability-distributions-deep-dive)
3. [Granular Synthesis Theory](#granular-synthesis-theory)
4. [FM Synthesis in ReGrandy](#fm-synthesis-in-regrandy)
5. [Envelope Types and Grain Shaping](#envelope-types-and-grain-shaping)
6. [Boundary Behavior: Wrap vs Mirror](#boundary-behavior-wrap-vs-mirror)
7. [Advanced Patching Techniques](#advanced-patching-techniques)
8. [Performance Optimization](#performance-optimization)
9. [Sound Design Recipes](#sound-design-recipes)
10. [Integration with Other Modules](#integration-with-other-modules)
11. [Known Limitations](#known-limitations)
12. [Best Practices](#best-practices)

---

## Understanding Dynamic Stochastic Synthesis

### Historical Context

Dynamic Stochastic Synthesis (DSS) was invented by Iannis Xenakis (1922-2001), a Greek-French composer, architect, and engineer. Xenakis pioneered the use of mathematical concepts in composition, particularly probability theory and set theory.

DSS was first implemented in the GENDY program on a mainframe computer in 1991, though Xenakis had been exploring stochastic processes in composition since the 1950s.

### The Algorithm

At its core, DSS works by:

1. **Creating Breakpoints**: A series of points defined by amplitude and duration
2. **Random Walks**: Each breakpoint randomly "walks" within bounds
3. **Interpolation**: Linear interpolation between current and next breakpoint
4. **Continuous Evolution**: The waveform never repeats exactly

#### Mathematical Representation

For breakpoint $i$ at time $t$:

$$
A_i(t+1) = \text{bound}(A_i(t) + \Delta A \cdot r_A)
$$

$$
D_i(t+1) = \text{bound}(D_i(t) + \Delta D \cdot r_D)
$$

Where:
- $A_i$ = amplitude of breakpoint $i$
- $D_i$ = duration of breakpoint $i$
- $\Delta A$ = maximum amplitude step (ASTP)
- $\Delta D$ = maximum duration step (DSTP)
- $r_A, r_D$ = random values from probability distribution
- $\text{bound}()$ = wrapping or mirroring function

### ReGrandy's Extension: GDSS

ReGrandy extends DSS by adding granular synthesis:

$$
\text{Output}(t) = \sum_{g=1}^{2} E_g(t) \cdot [A_g(t) + W_g(t)]
$$

Where:
- $E_g(t)$ = grain envelope function
- $A_g(t)$ = stochastic amplitude interpolation
- $W_g(t)$ = wavetable or FM oscillator sample
- $g$ = grain index (two simultaneous grains)

This creates a hybrid where:
- The **macro structure** comes from DSS (slow amplitude evolution)
- The **micro structure** comes from granular synthesis (fast grain texture)

---

## Probability Distributions Deep Dive

The PDST parameter fundamentally changes the character of random walks.

### Linear Distribution

**Mathematical Form**: Uniform distribution $U(-1, 1)$

$$
P(x) = \frac{1}{2} \text{ for } x \in [-1, 1]
$$

**Characteristics:**
- Equal probability across entire range
- Predictable, moderate variations
- No bias toward center or extremes

**Best For:**
- Smooth, controlled evolution
- Pitched material where tuning matters
- Beginner-friendly patches
- Rhythmic patterns

**Parameter Interaction:**
- Works well with any ASTP/DSTP values
- Most CPU-efficient option
- Predictable response to CV modulation

### Cauchy Distribution

**Mathematical Form**: Inverse Cauchy transform

$$
F^{-1}(u) = \frac{1}{a} \tan(c \cdot (2u - 1))
$$

Where $c = \arctan(10a)$ and $a = 0.5$

**Characteristics:**
- Heavy tails: rare but extreme jumps
- Most values cluster near center
- Creates "surprises" in otherwise smooth evolution

**Best For:**
- Evolving textures with occasional bursts
- Glitch effects and unexpected changes
- Keeping long drones interesting
- Experimental/avant-garde music

**Parameter Interaction:**
- High ASTP + Cauchy = dramatic jumps
- Low ASTP + Cauchy = subtle spikes
- Combines well with slow LFO modulation
- More variation with high BPTS

**CPU Note**: Slightly more expensive than Linear due to trigonometric functions.

### Arcsine Distribution

**Mathematical Form**: Inverse arcsine transform

$$
F^{-1}(u) = \frac{\sin(\pi(u - 0.5)a)}{c}
$$

Where $c = \sin(1.5707963 \cdot a)$ and $a = 0.5$

**Characteristics:**
- Bimodal: peaks at extremes (-1 and +1)
- Values tend toward boundaries
- Creates oscillating, bouncing behavior

**Best For:**
- Rhythmic, pulsing textures
- Maximum contrast between states
- "Bouncing ball" effect
- Noise generation with structure

**Parameter Interaction:**
- Works best with MIRR mode ON (prevents getting stuck at boundaries)
- High DSTP creates rapid oscillations
- Low BPTS shows bimodal nature more clearly
- Can create subharmonics when DSTP is low

**Sonic Signature**: Often sounds more "periodic" than other distributions due to tendency toward extremes.

---

## Granular Synthesis Theory

ReGrandy uses **synchronous granular synthesis** where grains are synchronized to the stochastic breakpoint timing.

### Grain Anatomy

Each grain consists of:

1. **Envelope** (ENVS): Window function that fades grain in/out
2. **Source**: Wavetable sample OR FM synthesis
3. **Rate** (GRAT): How fast the grain plays back
4. **Offset**: Starting position in wavetable (internally randomized)

### The Dual-Grain System

ReGrandy generates **two overlapping grains** continuously:

```
Grain 1: ████░░░░░░░░
Grain 2:      ████░░░░░░░░
         ├─────────────> Time
         Crossfade region
```

This creates smooth transitions without gaps or clicks.

### GRAT Parameter Explained

GRAT controls the grain playback rate:

$$
\text{Grain Frequency} = 261.626 \text{ Hz} \cdot 2^{\text{GRAT}}
$$

**GRAT values and effects:**

| GRAT | Frequency | Effect |
|------|-----------|--------|
| -6 | ~4 Hz | Very slow, sweeping motions |
| -3 | ~33 Hz | Slow wobble, almost pitched |
| 0 | ~262 Hz | Grain rate matches fundamental |
| +1 | ~523 Hz | Fast grain texture |
| +3 | ~2093 Hz | Dense, noise-like grains |

### Grain Density

The **perceived grain density** is:

$$
\text{Density} = \frac{\text{GRAT frequency}}{\text{FREQ frequency}} \times \text{BPTS}
$$

**Guidelines:**
- Density < 1: Sparse grains, gaps audible
- Density ≈ 1: Natural sounding
- Density > 10: Very dense, smooth texture
- Density > 100: Approaches white noise

### Wavetable Reading

When FM mode is OFF, grains read from a sine wavetable with randomized offsets:

```cpp
g_amp = amp + (env.get(g_idx) * sample.get(off));
```

The `off` parameter undergoes its own random walk, creating subtle timbral variations.

---

## FM Synthesis in ReGrandy

When FMTR switch is UP, ReGrandy switches to FM synthesis for the grain source.

### FM Algorithm

Classic two-operator FM:

$$
\text{Output}(t) = \sin(2\pi f_c t + I \sin(2\pi f_m t))
$$

Where:
- $f_c$ = carrier frequency (FCAR)
- $f_m$ = modulator frequency (FMOD)
- $I$ = modulation index (IMOD)

### ReGrandy's Implementation

Each grain gets its own FM synthesis:

```
Grain 1:
  Modulator → sin(phase_mod1)
       ↓ (scaled by IMOD)
  Carrier → sin(phase_car1 + modulation)
       ↓
  Envelope → multiply
  
Grain 2: (parallel)
  [Same structure]
```

The carrier frequency is **modulated continuously**, creating complex timbral evolution.

### Harmonic Relationships

The ratio $f_c : f_m$ determines harmonic content:

| Ratio | Type | Character |
|-------|------|-----------|
| 1:1 | Harmonic | Pure tone, vibrato effect |
| 2:1 | Harmonic | Octave relationship, rich |
| 3:2 | Harmonic | Perfect fifth, musical |
| 1.4:1 | Inharmonic | Bell-like, metallic |
| 8:1 | Inharmonic | Bright, clangy |

**To calculate ratio:**
$$
\text{Ratio} = \frac{2^{\text{FCAR}}}{2^{\text{FMOD}}}
$$

### IMOD and Sidebands

The modulation index determines the **number and amplitude of sidebands**:

$$
\text{Sidebands} = f_c \pm n \cdot f_m \text{ for } n = 0, 1, 2, ...
$$

**Approximate sideband count**: $I / f_m$

| IMOD | Effect |
|------|--------|
| 0-50 | Subtle, vibrato-like |
| 50-200 | Clear FM character, bell-like |
| 200-500 | Complex, many partials |
| 500-1000 | Very bright, aggressive |
| 1000+ | Noise-like, harsh |

### FM + Stochastic Interaction

The **magic** happens when DSS modulates FM parameters:

- **Stochastic amplitude** (from ASTP/DSTP) → dynamic envelope
- **Grain rate** (GRAT) → creates beating patterns with FM sidebands
- **Multiple BPTS** → each segment has different FM characteristics

This creates sounds **impossible with pure FM or pure DSS alone**.

---

## Envelope Types and Grain Shaping

The ENVS parameter selects the **window function** for grains.

### Sine (ENVS = 1)

$$
E(t) = \sin(\pi t) \text{ for } t \in [0, 1]
$$

**Properties:**
- Smooth attack and release
- Slight emphasis in middle
- Moderate frequency spread

**Use When:**
- General-purpose grain windowing
- Smooth textures desired
- Learning the module

### Triangle (ENVS = 2)

$$
E(t) = \begin{cases}
2t & \text{if } t < 0.5 \\
2(1-t) & \text{if } t \geq 0.5
\end{cases}
$$

**Properties:**
- Linear slopes
- Sharper attacks than sine
- Wider frequency spectrum (more harmonics)

**Use When:**
- Sharper, more defined grains
- Percussive elements
- Rhythmic textures

### Hann (ENVS = 3)

$$
E(t) = 0.5 \left(1 - \cos(2\pi t)\right)
$$

**Properties:**
- Smoothest attack/release
- Minimal spectral leakage
- Nearly continuous spectrum at grain transitions

**Use When:**
- Maximum smoothness needed
- Avoiding clicks/pops
- Dense grain clouds
- High GRAT settings

**Technical Note**: Hann window has excellent properties for signal processing - zero discontinuity at boundaries.

### Welch (ENVS = 4)

$$
E(t) = 1 - (2t - 1)^2
$$

**Properties:**
- Parabolic shape
- Broader frequency response
- Softer peaks than Hann

**Use When:**
- Natural, organic textures
- Moderate smoothness
- Alternative to Sine

### Tukey (ENVS = 5)

Tapered cosine window (specific taper parameters internal).

**Properties:**
- Flat top with tapered edges
- Good balance: smooth but present
- Minimal amplitude modulation

**Use When:**
- Sustained portions in grains
- Minimizing amplitude fluctuation
- Experimental sounds

### Envelope Comparison

| Envelope | Smoothness | Brightness | CPU | Best Use |
|----------|------------|------------|-----|----------|
| Sine | ●●●○○ | ●●●○○ | Low | General |
| Triangle | ●●○○○ | ●●●●○ | Lowest | Percussive |
| Hann | ●●●●● | ●●○○○ | Low | Smooth/Dense |
| Welch | ●●●●○ | ●●●○○ | Low | Natural |
| Tukey | ●●●○○ | ●●●○○ | Medium | Sustained |

### Practical Envelope Selection

**Decision Tree:**

```
Are clicks/pops a problem?
├─ Yes → Use Hann (ENVS = 3)
└─ No
   └─ Want smooth or sharp?
      ├─ Smooth → Sine (1) or Welch (4)
      └─ Sharp → Triangle (2)
         
Is it experimental?
└─ Try Tukey (5)
```

---

## Boundary Behavior: Wrap vs Mirror

The MIRR switch determines what happens when breakpoint values exceed bounds.

### Wrap Mode (MIRR = DOWN)

**Algorithm:**
```cpp
if (value > upper_bound) value = lower_bound;
if (value < lower_bound) value = upper_bound;
```

**Behavior:**
- Hard boundary: instant jump to opposite side
- Creates discontinuities
- Can produce sudden changes

**Visual:**
```
Amplitude over time:
 1.0 |     ___/
     |    /
 0.5 |___/
     |                  ___/
-0.5 |                 /
-1.0 |________________/
     └────────────────────> Time
          Wraps here ↑
```

**Use When:**
- Dramatic changes desired
- Glitch/digital aesthetics
- Maximum contrast
- Rhythmic, pulsing textures

**Parameter Interaction:**
- Works well with Arcsine distribution
- High ASTP + Wrap = sudden jumps
- Can create subharmonic effects at low FREQ

### Mirror Mode (MIRR = UP)

**Algorithm:**
```cpp
if (value > upper_bound) value = upper_bound - (value - upper_bound);
if (value < lower_bound) value = lower_bound - (value - lower_bound);
```

**Behavior:**
- Soft boundary: reflects back into range
- Maintains continuity
- Smoother evolution

**Visual:**
```
Amplitude over time:
 1.0 |     __/\_
     |    /     \
 0.5 |___/       \___
     |                \  /
-0.5 |                 \/
-1.0 |________________/
     └────────────────────> Time
        Mirrors here ↑
```

**Use When:**
- Smooth, continuous textures
- Musical pitched content
- Avoiding abrupt changes
- Ambient/pad sounds

**Parameter Interaction:**
- Essential with Arcsine distribution
- Works well with high BPTS
- More natural-sounding in most contexts

### Acoustic Impact

The boundary mode affects the **timbre** significantly:

| Aspect | Wrap | Mirror |
|--------|------|--------|
| **Harmonics** | More odd harmonics | Smoother spectrum |
| **Transients** | Sharp clicks possible | Soft transitions |
| **Predictability** | Less predictable | More predictable |
| **Spectral noise** | Higher | Lower |

### Recommendation

**Start with Mirror (UP)** for most musical applications. Switch to Wrap for experimental sound design.

---

## Advanced Patching Techniques

### Technique 1: Self-Modulation

**Concept**: Use ReGrandy's output to modulate its own parameters.

**Patch:**
```
ReGrandy SINE_OUTPUT
  ├─→ Slew Limiter → Offset/Scale → ASTP CV
  └─→ Sample & Hold → GRAT CV
```

**Result**: Self-evolving, feedback-like behavior where the sound shapes its own evolution.

**Tips:**
- Add slew limiting to prevent too-fast changes
- Use offset/scale to keep modulation in useful range
- Can create quasi-chaotic behavior

### Technique 2: Dual ReGrandy Synchronization

**Concept**: Two ReGrandy modules with linked but offset parameters.

**Patch:**
```
LFO → [Offset +0.5] ┬─→ ReGrandy 1 FREQ CV
                     └─→ ReGrandy 2 FREQ CV
                     
ReGrandy 1 → Left output
ReGrandy 2 → Right output
```

**Settings:**
- Identical BPTS, ASTP, DSTP
- Slightly different FREQ (detuning)
- Same PDST and MIRR
- Offset GRAT by 0.5-1.0 octaves

**Result**: Stereo spread, chorus-like effect, richer texture.

### Technique 3: Granular Quantizer

**Concept**: Quantize FREQ CV to create melodic sequences from chaos.

**Patch:**
```
ReGrandy 1 SINE_OUTPUT
  └─→ Quantizer (set to scale)
      └─→ ReGrandy 2 FREQ CV (attenuator 0.5)
      
ReGrandy 2 → Audio output
```

**Result**: ReGrandy 1 generates random-ish voltages, quantizer constrains to scale, ReGrandy 2 plays melodic but evolving sequence.

**Variations:**
- Use S&H before quantizer for stepped notes
- Add envelope to ReGrandy 2 for plucky sounds
- Mult the quantized CV to other oscillators

### Technique 4: Spectral Freezing

**Concept**: Very slow or stopped evolution with focus on grain texture.

**Settings:**
```
FREQ: Any
BPTS: 30-40 (high)
ASTP: 0.01 (very low!)
DSTP: 0.001 (almost frozen)
GRAT: +1 to +2 (fast grains)
```

**Result**: Nearly static spectrum with rich grain texture creating "spectral freeze" effect.

**Use:** Drone music, ambient, creating sustained textures.

### Technique 5: Rhythmic Patterns via CV

**Concept**: Use rhythmic CV modulation to create beat-synced textures.

**Patch:**
```
Clock → Trigger Delay (various times)
         ├─→ Envelope 1 → ASTP CV
         ├─→ Envelope 2 → GRAT CV
         └─→ Envelope 3 → IMOD CV (if FM on)
```

**Settings:**
- Different envelope shapes for each
- Synchronize to same clock
- Vary delay times for polyrhythms

**Result**: Rhythmically evolving textures that lock to tempo.

### Technique 6: FM Feedback Simulation

**Concept**: Simulate FM feedback using external processing.

**Patch:**
```
ReGrandy (FM ON)
  └─→ Wavefolder
      └─→ Slew → Offset → IMOD CV
```

**Result**: The wavefolder adds harmonics, which then modulate the FM index, creating complex feedback-like interactions.

### Technique 7: Multi-Band Stochastic Processing

**Concept**: Split ReGrandy into frequency bands, process separately.

**Patch:**
```
ReGrandy
  ├─→ LP Filter (300Hz) → Reverb → Mix
  ├─→ BP Filter (300-2kHz) → Delay → Mix
  └─→ HP Filter (2kHz+) → Distortion → Mix
```

**Result**: Different treatments for different frequency ranges, creating more complex textures.

---

## Performance Optimization

### CPU Usage Factors

ReGrandy's CPU usage depends on:

1. **BPTS** (Number of Breakpoints): **Linear impact**
2. **GRAT** (Grain Rate): **Minimal impact**
3. **PDST** (Distribution): **Slight impact** (Cauchy/Arcsine slightly higher)
4. **FM Mode**: **~20% increase** when ON
5. **Sample Rate**: **Linear impact** (higher = more CPU)

### Optimization Strategies

#### Strategy 1: Reduce BPTS

Most musical sounds work well with 8-15 breakpoints.

**Test:**
- Start at BPTS = 50
- Reduce by 10 until you hear quality loss
- Use the lowest value that sounds good

**Savings**: Each 10 breakpoints ≈ 5-10% CPU reduction

#### Strategy 2: Limit Active Instances

Run only as many ReGrandy modules as needed.

**Alternatives:**
- Use one ReGrandy + signal duplication for multiple voices
- Render complex patches to audio, then replay

#### Strategy 3: Increase Audio Buffer Size

In VCV Rack settings:
- Engine → Block size: Increase to 512 or 1024 samples
- **Tradeoff**: Increases latency, reduces CPU load

#### Strategy 4: Use Linear Distribution

PDST = Linear is most CPU-efficient.

**Savings**: ~2-5% compared to Cauchy/Arcsine

#### Strategy 5: Disable FM When Not Needed

FM mode adds significant CPU usage.

**Check**: If IMOD is zero or FMTR is down, ensure you're getting benefit from FM before enabling.

### Optimization Checklist

Before going on stage or recording:

- [ ] All BPTS values minimized for desired sound
- [ ] FM mode OFF on modules where not needed
- [ ] Unused CV inputs disconnected
- [ ] Sample rate set appropriately (44.1kHz vs 96kHz)
- [ ] Block size increased if latency acceptable
- [ ] CPU meter checked in VCV Rack

### Performance Benchmarks

Approximate CPU usage (% of single core, 44.1kHz, block size 256):

| Configuration | CPU % |
|---------------|-------|
| Default (BPTS=12, FM Off) | 2-3% |
| High BPTS (40, FM Off) | 6-8% |
| FM Mode (BPTS=12) | 4-5% |
| Full Complexity (BPTS=50, FM On) | 10-12% |

*Note: Values depend on system. Measure on your hardware.*

---

## Sound Design Recipes

### Recipe 1: Xenakis-Style Orchestral Cloud

**Goal**: Dense, evolving texture reminiscent of Xenakis's orchestral works.

**Modules:**
- ReGrandy x3 (different pitch ranges)
- Mixer
- Reverb
- HP/LP Filter

**Settings (ReGrandy 1 - Low):**
```
FREQ: -3 (sub-bass range)
BPTS: 25
ASTP: 0.4
DSTP: 0.3
GRAT: -2
PDST: Cauchy
MIRR: Up
```

**Settings (ReGrandy 2 - Mid):**
```
FREQ: 0 (mid range)
BPTS: 30
ASTP: 0.5
DSTP: 0.4
GRAT: 0
PDST: Linear
MIRR: Up
```

**Settings (ReGrandy 3 - High):**
```
FREQ: +2 (high range)
BPTS: 35
ASTP: 0.6
DSTP: 0.5
GRAT: +1
PDST: Arcsine
MIRR: Down
```

**Processing:**
- Mix: Low=40%, Mid=40%, High=20%
- HP Filter: 80Hz (remove rumble)
- LP Filter: 8kHz (tame highs)
- Reverb: 6s decay, 30% mix

**Modulation:**
- Very slow LFO (0.05 Hz) → All FREQ CVs (0.2 attenuators)

**Result**: Massive, orchestral texture with multiple evolving layers.

### Recipe 2: Sci-Fi Transmission

**Goal**: Broken radio transmission / alien communication.

**Modules:**
- ReGrandy
- Sample & Hold
- Waveshaper
- HP Filter
- Delay

**Settings:**
```
FREQ: +1
BPTS: 40
ASTP: 0.7
DSTP: 0.6
GRAT: +2
PDST: Arcsine
MIRR: Down
FM: OFF
ENVS: Triangle
```

**Modulation:**
```
Fast LFO (8Hz) → S&H (random) → BPTS CV (0.5)
Slow LFO (0.3Hz) → FREQ CV (0.4)
```

**Processing:**
- Waveshaper: Moderate drive
- HP Filter: 500Hz
- Delay: 1/8 note, 30% feedback, filtered

**Performance:**
- Manually sweep GRAT during performance
- Toggle MIRR switch for different "modes"

**Result**: Glitchy, digital, alien transmission sounds.

### Recipe 3: Underwater Ambience

**Goal**: Deep ocean, submarine ambience.

**Modules:**
- ReGrandy
- LP Filter (steep, 24dB)
- Chorus
- Reverb (large)
- Compressor

**Settings:**
```
FREQ: -2 (low)
BPTS: 15
ASTP: 0.2
DSTP: 0.15
GRAT: -4 (very slow!)
PDST: Linear
MIRR: Up
FM: OFF
ENVS: Hann
```

**Processing:**
- LP Filter: 400Hz cutoff, high resonance (~0.7)
- Chorus: Slow rate, medium depth
- Reverb: 10s+ decay, dark character, 50% mix
- Compressor: Slow attack, medium release

**Modulation:**
```
Ultra-slow LFO (0.02 Hz) → Filter Cutoff
Slow LFO (0.1 Hz) → ASTP CV (0.3)
```

**Result**: Deep, rumbling, mysterious underwater soundscape.

### Recipe 4: Rhythmic Generative Sequence

**Goal**: Self-generating melodic sequence with rhythm.

**Modules:**
- ReGrandy
- Quantizer (chromatic or scale)
- Clock Divider
- Envelope x2
- VCA
- Delay

**ReGrandy Settings:**
```
FREQ: 0 (this will be overridden)
BPTS: 20
ASTP: 0.4
DSTP: 0.3
GRAT: -1
PDST: Cauchy
MIRR: Up
```

**Patch:**
```
ReGrandy INV_OUTPUT → Quantizer → ReGrandy FREQ CV (1.0)
Clock (120 BPM) → Clock Divider
  ├─ /1 → Envelope 1 (short) → VCA CV
  └─ /4 → Envelope 2 (long) → ASTP CV (0.4)
  
ReGrandy SINE_OUTPUT → VCA → Delay → Output
```

**Result**: Melodic, quantized sequence that evolves based on its own output. Rhythm from clock, pitch from self-modulation.

### Recipe 5: Granular Pluck/Mallets

**Goal**: Pitched percussive sounds like marimbas, xylophones, or kalimbas.

**Settings:**
```
FREQ: 0 to +2 (pitched)
BPTS: 6 (low for clear pitch)
ASTP: 0.5
DSTP: 0.1
GRAT: +2 (fast grains)
PDST: Linear
MIRR: Up
FM: ON
FCAR: +1
FMOD: +4 (high, inharmonic)
IMOD: +2
ENVS: Hann
```

**Envelope:**
- External ADSR: A=1ms, D=500ms, S=0, R=200ms
- Gate from sequencer

**Processing:**
- Slight LP filter (~3kHz)
- Short plate reverb

**Tips:**
- Change FMOD for different timbres
- Lower IMOD for more wooden sound
- Higher IMOD for metallic

**Result**: Bell/mallet-like pitched percussion with evolving character.

---

## Integration with Other Modules

### With Oscillators

**Oscillator as Modulation Source:**
```
VCO Triangle → ReGrandy ASTP CV
```
Creates rhythmic amplitude modulation.

**ReGrandy as Modulation Source:**
```
ReGrandy SINE_OUTPUT → VCA → Another VCO FM input
```
Uses stochastic waveform for complex FM.

### With Filters

**Self-Tracking Filter:**
```
ReGrandy → Filter
ReGrandy INV_OUTPUT → Filter Cutoff CV
```
Filter tracks the signal's evolution.

**Multiple Filter Bands:**
```
ReGrandy → LP → HP (parallel) → Mix
```
Emphasize different frequency regions.

### With Reverb/Delay

**Feedback Integration:**
```
ReGrandy → Delay (with feedback) → Mix
            ↑
      Feedback Send → HP Filter
```
Delay feedback only sends highs, preventing mud.

**Modulated Reverb:**
```
ReGrandy → Reverb
ReGrandy INV → Reverb Size/Decay CV
```
Reverb evolves with source.

### With Sequencers

**Stochastic Sequencer Control:**
```
Clock → ReGrandy GRAT CV (via envelope)
ReGrandy SINE → S&H → Quantizer → Other modules
```
ReGrandy generates melodic content from clock.

### With Effects

**Dynamic Waveshaping:**
```
ReGrandy → Waveshaper
ReGrandy INV → Waveshaper Drive CV
```
Self-modulating distortion.

**Granular Delay:**
```
ReGrandy → Granular Delay → Output
ReGrandy INV → Grain Size CV
```
Nested granular processing.

---

## Known Limitations

### Current Limitations

1. **Monophonic Only**
   - No polyphonic support
   - Workaround: Use multiple instances

2. **No Direct Grain Count Control**
   - Grain density is indirect (via GRAT + FREQ + BPTS)
   - Workaround: See [Granular Synthesis Theory](#granular-synthesis-theory) for formulas

3. **No Stereo Output**
   - Single mono output (plus inverted)
   - Workaround: Use multiple instances with slight parameter offsets

4. **Fixed Wavetable**
   - Can't load custom wavetables
   - Wavetable source is always sine wave (when FM off)
   - Workaround: Use FM mode or external processing

5. **BPTS Changes Can Click**
   - Rapid BPTS modulation causes zipper noise
   - Workaround: Use slow modulation or avoid modulating BPTS

6. **No Visual Feedback**
   - No oscilloscope or waveform display
   - No visual indication of breakpoint positions
   - Workaround: Use external scope module

7. **No Preset Morphing**
   - Can't smoothly morph between stored states
   - Workaround: Use CV modulation and save patches

### Theoretical Limitations

**Nyquist Frequency:**
- Very high GRAT + FREQ can alias
- Sample rate dependent
- Usually not an issue with default settings

**Random Number Generation:**
- Uses VCV Rack's RNG (not cryptographically secure)
- Deterministic given same seed
- Good enough for audio

---

## Best Practices

### General Guidelines

1. **Start Simple**
   - Begin with BPTS = 10-15
   - Low ASTP/DSTP (0.2)
   - Build up complexity

2. **One Change at a Time**
   - Adjust parameters individually
   - Understand each parameter's effect
   - Document sweet spots

3. **Use CV Modulation Sparingly**
   - Start with one or two CV sources
   - Low attenuator values initially
   - Slowly increase complexity

4. **Save Variations**
   - Save patches at different stages
   - Name patches descriptively
   - Build your own preset library

### Parameter Ranges

**Conservative (Musical):**
- BPTS: 8-20
- ASTP: 0.1-0.4
- DSTP: 0.1-0.3
- GRAT: -3 to +1

**Aggressive (Experimental):**
- BPTS: 25-50
- ASTP: 0.5-1.0
- DSTP: 0.4-1.0
- GRAT: +1 to +3

### Avoiding Common Mistakes

❌ **Don't:**
- Max out all parameters at once
- Modulate BPTS with fast signals
- Ignore the ENVS parameter
- Forget about MIRR setting
- Use extreme GRAT without intention

✅ **Do:**
- Adjust parameters gradually
- Use appropriate envelope types
- Consider boundary behavior (MIRR)
- Match GRAT to desired density
- Use visual feedback (scope module)

### Mixing & Mastering

**EQ Recommendations:**
- **HP Filter**: 30-80 Hz (remove subsonic rumble)
- **Broad Cut**: 200-400 Hz (reduce muddiness if needed)
- **Presence Boost**: 2-4 kHz (if too dull)
- **Air**: Subtle boost at 10kHz+ (for sparkle)

**Dynamics:**
- Light compression (2-3:1) for glue
- Slow attack to preserve transients
- Medium-fast release

**Spatial:**
- Stereo width: Use multiple instances panned differently
- Reverb: Match to musical context
- Delay: Rhythmic delays work well with slower GRAT

---

## Further Reading

- **[API Reference](API.md)**: Technical details and function references
- **[FAQ](FAQ.md)**: Common questions and solutions
- **[Examples](examples/)**: Real-world patches and presets

### External Resources

- Xenakis, I. (1992). *Formalized Music: Thought and Mathematics in Composition*
- Roads, C. (2001). *Microsound*
- Chowning, J. (1973). "The Synthesis of Complex Audio Spectra by Means of Frequency Modulation"

---

**Experiment boldly! Granular Dynamic Stochastic Synthesis rewards exploration.** 🎛️✨
