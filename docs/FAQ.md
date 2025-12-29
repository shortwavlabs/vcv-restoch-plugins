# Frequently Asked Questions (FAQ)

Common questions and solutions for ReGrandy users.

## Table of Contents

1. [General Questions](#general-questions)
2. [Getting Started](#getting-started)
3. [Parameter Questions](#parameter-questions)
4. [Sound Issues](#sound-issues)
5. [Performance & CPU](#performance--cpu)
6. [Technical Questions](#technical-questions)
7. [Patching & Routing](#patching--routing)
8. [Comparison Questions](#comparison-questions)
9. [Troubleshooting](#troubleshooting)
10. [Advanced Usage](#advanced-usage)

---

## General Questions

### What is ReGrandy?

ReGrandy is a **Granular Dynamic Stochastic Synthesis** module for VCV Rack. It combines:
- **Dynamic Stochastic Synthesis** (invented by Iannis Xenakis)
- **Granular Synthesis** (window-based processing)
- **FM Synthesis** (optional frequency modulation)

Result: Evolving, organic, never-repeating textures.

### What does "stochastic" mean?

**Stochastic** = using probability and randomness. In ReGrandy, parameters evolve through controlled random walks, creating organic variation.

### Is ReGrandy an oscillator?

Yes and no. It generates audio like an oscillator, but its behavior is more like a **texture generator**. It's designed for evolving timbres rather than static waveforms.

### What is RestocK?

**RestocK** is the plugin collection that contains ReGrandy. It's a port of the original StochKit plugins by Sam Laing, updated for VCV Rack 2.x by Shortwav Labs.

### Is ReGrandy free?

Yes! RestocK is open-source under GPL-3.0-or-later license. Free to use, modify, and share.

---

## Getting Started

### How do I install ReGrandy?

**Method 1 (Recommended):**
1. Open VCV Rack
2. Library → Search "RestocK"
3. Install
4. Restart VCV Rack

**Method 2 (Manual):**
See [Installation Guide](README.md#installation)

### I'm new to synthesis. Can I use ReGrandy?

Yes! Start with:
1. [QuickStart Guide](QUICKSTART.md)
2. [Example 01: First Sound](examples/basic/01-first-sound.md)
3. Experiment with simple settings
4. Gradually increase complexity

### What other modules do I need?

**Minimum:**
- Audio output module (VCV Audio-8 or similar)

**Recommended:**
- VCF (filter)
- Reverb
- LFO (for modulation)
- Mixer (for multiple instances)

### Where should I start?

Follow this path:
1. Read [QuickStart Guide](QUICKSTART.md) - 15 minutes
2. Try [Example 01](examples/basic/01-first-sound.md) - 5 minutes
3. Experiment with parameters - 30 minutes
4. Explore [other examples](examples/README.md)

---

## Parameter Questions

### What does BPTS do?

**BPTS** (Breakpoints) controls the **number of points** in the stochastic waveform.

- **Low (3-8)**: Simple waveforms, clear pitch
- **Medium (10-20)**: Balanced complexity
- **High (25-50)**: Dense, noise-like textures

**Think of it as**: Waveform complexity or harmonic density.

### What's the difference between ASTP and DSTP?

- **ASTP** (Amplitude Step): How much the **volume** varies randomly
- **DSTP** (Duration Step): How much the **timing** varies randomly

**Rule of thumb**: Start both at 0.2, adjust to taste.

### What does GRAT control?

**GRAT** (Granulation Rate) controls the **speed of the grain envelope**.

- **Low (-6 to -2)**: Slow wobbles, sweeping effects
- **Medium (-1 to +1)**: Natural grain texture
- **High (+2 to +3)**: Dense, smooth, almost noise

**Think of it as**: Grain density or texture speed.

### What are the three PDST positions?

**PDST** (Probability Distribution):

| Position | Type | Character |
|----------|------|-----------|
| **Left** | Linear | Smooth, predictable |
| **Center** | Cauchy | Occasional surprises |
| **Right** | Arcsine | Extreme values, bouncy |

**Start with Left** (Linear) for most musical applications.

### What does MIRR do?

**MIRR** (Mirror mode) controls **boundary behavior**:

- **Down (Wrap)**: Hard boundaries, sudden jumps
- **Up (Mirror)**: Soft boundaries, smooth reflections

**Recommendation**: Use Mirror (Up) for smooth sounds, Wrap (Down) for glitchy effects.

### What are the ENVS settings?

**ENVS** (Envelope Type) controls **grain window shape**:

| Setting | Type | Character |
|---------|------|-----------|
| **1** | Sine | General purpose |
| **2** | Triangle | Sharper, brighter |
| **3** | Hann | Smoothest (no clicks) |
| **4** | Tukey | Balanced |

**Start with 3 (Hann)** for smoothest results.

---

## Sound Issues

### Why is there no sound?

**Checklist:**
- ✅ Cable connected from SINE OUTPUT?
- ✅ Audio engine running (green power button)?
- ✅ Audio output module configured?
- ✅ Volume/level not at zero?
- ✅ FREQ not at extreme low (-4)?

**Quick fix**: Right-click ReGrandy → Initialize

### The sound is too quiet

**Solutions:**
1. Increase ASTP to 0.3-0.5
2. Add VCA or amplifier module
3. Check GRAT isn't too low (< -4)
4. Add multiple instances and mix

### The sound is too harsh/noisy

**Solutions:**
1. Lower ASTP to 0.1-0.2
2. Lower DSTP to 0.1-0.2
3. Decrease BPTS to 8-12
4. Change ENVS to 3 (Hann)
5. Switch PDST to Linear (left)
6. Add lowpass filter (~2-3 kHz)

### I hear clicking or popping

**Causes:**
- BPTS changing too fast
- Wrong envelope type
- Extreme parameter values

**Solutions:**
1. Use ENVS = 3 (Hann)
2. Slow down CV modulation of BPTS
3. Lower DSTP
4. Enable MIRR (mirror mode)

### The pitch is wrong/unstable

**Remember**: ReGrandy is **stochastic**, so pitch naturally varies!

**For more stable pitch:**
1. Lower ASTP to 0.1
2. Lower DSTP to 0.1
3. Decrease BPTS to 4-6
4. Use PDST = Linear
5. Enable MIRR mode

**For precise pitch**: Use a traditional oscillator instead. ReGrandy is designed for evolving textures.

### Sound keeps repeating

With low randomness, ReGrandy can sound periodic. This is normal!

**To increase variation:**
1. Increase ASTP to 0.3-0.5
2. Increase DSTP to 0.3-0.4
3. Increase BPTS to 20+
4. Try Cauchy or Arcsine distribution
5. Add slow CV modulation

---

## Performance & CPU

### How much CPU does ReGrandy use?

**Typical usage:**
- Default settings (BPTS=12, FM Off): **2-3%**
- High BPTS (40, FM Off): **6-8%**
- FM Mode (BPTS=12): **4-5%**
- Maximum (BPTS=50, FM On): **10-12%**

*(Percentage of single CPU core at 44.1kHz)*

### How can I reduce CPU usage?

**Optimization tips:**
1. **Lower BPTS**: Use 8-15 instead of 40+
2. **Disable FM**: Turn off FMTR when not needed
3. **Use Linear distribution**: Most efficient
4. **Increase buffer size**: VCV settings → larger blocks
5. **Use fewer instances**: Limit to 2-3 maximum

See [Performance Optimization](ADVANCED.md#performance-optimization) for details.

### Can I use multiple ReGrandy modules?

Yes! Many patches use 2-3 instances at different ranges.

**Tips:**
- Layer at different octaves
- Mix at 40/40/20 or similar
- Monitor CPU usage
- Consider rendering to audio if CPU-limited

### Does sample rate affect CPU?

Yes! Higher sample rates = more CPU.

- **44.1 kHz**: Standard, efficient
- **48 kHz**: Slightly higher CPU
- **96 kHz**: ~2x CPU usage

**Recommendation**: Use 44.1 or 48 kHz unless you specifically need higher.

---

## Technical Questions

### Is ReGrandy polyphonic?

**No**, currently monophonic only.

**Workaround**: Use multiple instances for polyphony (CPU-intensive).

**Future**: Polyphonic support planned for version 2.x.

### What's the frequency range?

**FREQ parameter range**: -4 to +3 octaves (relative to C4)

**Actual frequencies**: ~16 Hz to ~2093 Hz

**Full calculation**:
$$
\text{Frequency} = 261.626 \times 2^{\text{FREQ}}
$$

### What's the maximum number of breakpoints?

**Maximum**: 50 (defined by MAX_BPTS constant)

**Practical**: 8-30 for most applications

**CPU impact**: Linear (50 breakpoints ≈ 2x CPU of 25)

### Can I load custom wavetables?

**No**, not in current version. Source is always:
- Sine wave (FM mode off)
- FM synthesis (FM mode on)

**Future**: Custom wavetable loading planned.

### Is the output deterministic?

**Sort of**. ReGrandy uses VCV Rack's random number generator, which is:
- **Deterministic**: Same patch + same seed = same output
- **Not cryptographically secure**: Patterns may emerge
- **Good enough**: For audio applications, it's excellent

**For truly random**: Patch external noise source to CV inputs.

---

## Patching & Routing

### What should I modulate with CV?

**Best CV targets:**

| Parameter | Modulation Source | Effect |
|-----------|-------------------|--------|
| **FREQ** | LFO (slow) | Pitch drift |
| **FREQ** | Sequencer | Melodic patterns |
| **ASTP** | LFO | Breathing dynamics |
| **GRAT** | LFO | Texture evolution |
| **IMOD** | Envelope | Dynamic FM |

**Start with**: One or two CV sources, low attenuator values (0.2-0.3).

### Can I use ReGrandy as a modulation source?

**Yes!** The output works great for modulation:

```
ReGrandy SINE_OUTPUT
  ↓
Slew Limiter (smooth)
  ↓
Offset/Scale (adjust range)
  ↓
Other module's CV input
```

**Use cases:**
- Filter cutoff modulation
- Other oscillator FM input
- VCA modulation
- Effect parameter control

### Should I use SINE or INV output?

**SINE_OUTPUT**: Normal output (use this 99% of the time)

**INV_OUTPUT**: Inverted (phase-flipped) output

**Use INV for:**
- Stereo patches (opposite phase creates width)
- Modulation (inverted CV signals)
- Phase cancellation effects

### How do I make stereo patches?

**Method 1: Dual Instance**
```
ReGrandy 1 (FREQ=0.0) → Left
ReGrandy 2 (FREQ=0.01) → Right
(slight detuning creates width)
```

**Method 2: Processing**
```
ReGrandy → Stereo Reverb/Delay → Left & Right
```

**Method 3: Inverter**
```
ReGrandy SINE → Left
ReGrandy INV → Right
(careful: can cancel in mono!)
```

---

## Comparison Questions

### ReGrandy vs. Regular Oscillator?

**Regular Oscillator:**
- Static waveforms
- Precise pitch control
- Efficient (low CPU)
- Repeating patterns

**ReGrandy:**
- Evolving timbres
- Organic variation
- Higher CPU
- Never repeats exactly

**Use ReGrandy when**: You want evolution and texture.
**Use Regular VCO when**: You want stable, precise pitch.

### ReGrandy vs. Granular Sampler?

**Granular Sampler:**
- Processes audio files
- Controllable grain parameters
- Grain position, pitch, etc.

**ReGrandy:**
- Generates from scratch (no samples)
- Stochastic evolution
- Synthesis-based

**Different tools**: Sampler for working with audio, ReGrandy for synthesis.

### ReGrandy vs. Noise Module?

**Noise Module:**
- Pure random values
- No structure
- White/pink/other colors

**ReGrandy:**
- Structured randomness
- Pitch-based foundation
- Evolving patterns
- Harmonic content

**ReGrandy is**: "Controlled chaos" not pure noise.

### Is ReGrandy like Mutable Instruments Clouds?

**Similarities:**
- Both do granular processing
- Both create textures

**Differences:**
- **Clouds**: Processes audio input, reverb/delay
- **ReGrandy**: Generates audio, stochastic synthesis

**Closer to**: Plaits' granular mode or Rings, but with stochastic evolution.

---

## Troubleshooting

### Patch doesn't sound like the example

**Possible causes:**
1. Different VCV Rack version
2. Different plugin versions
3. Missing modules
4. Parameter differences

**Solutions:**
- Check all parameter values carefully
- Initialize module and start fresh
- Verify all required modules installed
- Check VCV Rack audio settings

### Parameters don't seem to do anything

**Check:**
- CV attenuators set too low?
- Parameter already at extreme?
- Audio level too quiet to hear?
- Wrong output connected?

**Test:**
- Set all CV attenuators to 0
- Set parameters to middle positions
- Increase volume
- Connect SINE_OUTPUT

### Module crashes VCV Rack

**This shouldn't happen!** Please report:
1. Go to [GitHub Issues](https://github.com/shortwavlabs/restock/issues)
2. Click "New Issue"
3. Include:
   - VCV Rack version
   - RestocK version
   - Operating system
   - Steps to reproduce
   - Console log (Help → Show Console)

### Strange sounds after VCV Rack update

After VCV Rack updates:
1. Reinstall RestocK
2. Rebuild plugin (if from source)
3. Check for RestocK updates
4. Initialize modules in old patches

---

## Advanced Usage

### How do I create generative patches?

**Basic approach:**
```
ReGrandy output
  ↓
Process (quantize, S&H, etc.)
  ↓
Back to ReGrandy CV input
(Self-modulation)
```

**See**: [Example 15: Self-Patching](examples/generative/15-self-patching.md)

### Can I sync to tempo?

**Not directly**, but you can:
1. Clock → Envelope → CV inputs
2. Tempo-synced LFO → Parameters
3. Trigger ReGrandy parameter changes on beat

**For rhythmic sync**: Modulate parameters with clock-derived signals.

### How do I create smooth transitions?

**Techniques:**
1. Use slow modulation (LFO < 0.2 Hz)
2. Enable Mirror mode
3. Use Hann envelope
4. Keep DSTP low (< 0.3)
5. Add slew limiters to CV

### Can I use ReGrandy for percussion?

**Yes**, with limitations:

**Setup:**
```
Trigger → Envelope → VCA CV
ReGrandy → VCA → Output
```

**Settings:**
- Low BPTS (4-8) for pitch
- High ASTP (0.5+) for dynamics
- Short envelope (< 500ms)
- FM mode ON for inharmonics

**Better for**: Textural percussion, not clean drums.

### How do I save my favorite settings?

**Method 1: Presets**
1. Right-click module
2. Preset → Save As
3. Name your preset

**Method 2: Full Patches**
- Save entire VCV patch
- Name descriptively

**Organization**: Create preset folders by category (pads, leads, fx, etc.)

---

## Still Have Questions?

### Resources

- **[QuickStart Guide](QUICKSTART.md)**: Basic usage
- **[Advanced Guide](ADVANCED.md)**: Deep techniques
- **[API Reference](API.md)**: Technical details
- **[Examples](examples/README.md)**: Real patches

### Community

- **[VCV Rack Forum](https://community.vcvrack.com/)**: Ask questions
- **[GitHub Issues](https://github.com/shortwavlabs/restock/issues)**: Report bugs
- **Email**: contact@shortwavlabs.com

### Can't find your question?

**Open a GitHub Discussion:**
1. Go to repository
2. Discussions → New Discussion
3. Ask your question!

We'll add popular questions to this FAQ.

---

**Last Updated**: 2024-12-28  
**Version**: RestocK 2.0.0

**Found an error in the FAQ?** [Open an issue](https://github.com/shortwavlabs/restock/issues) or submit a PR!
