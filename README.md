# ReGrandy - Granular Dynamic Stochastic Synthesis Module

![Version](https://img.shields.io/badge/version-2.0.0-blue.svg)
![License](https://img.shields.io/badge/license-GPL--3.0--or--later-green.svg)
![VCV Rack](https://img.shields.io/badge/VCV%20Rack-2.x-orange.svg)

## Overview

**ReGrandy** is a revolutionary stochastic synthesis generator for VCV Rack that implements **Granular Dynamic Stochastic Synthesis (GDSS)**—an extended version of Iannis Xenakis's Dynamic Stochastic Synthesis with a synchronous granular synthesis twist.

This module is part of the **RestocK** plugin collection by Shortwav Labs, bringing back to life the StochKit plugins originally created by Sam Laing for VCV Rack 2.x.

## Key Features

- **Xenakis-Inspired Stochastic Synthesis**: Implements Dynamic Stochastic Synthesis with up to 50 breakpoints
- **Granular Synthesis Integration**: Synchronous granular processing adds texture and complexity
- **FM Synthesis Mode**: Built-in frequency modulation for additional sonic possibilities
- **Flexible Probability Distributions**: Choose between Linear, Cauchy, and Arcsine distributions
- **Envelope Shaping**: Multiple envelope types (Sine, Triangle, Hann, Welch, Tukey) for grain shaping
- **Extensive CV Control**: All major parameters respond to ±5V CV modulation
- **Dual Outputs**: Normal and inverted outputs for expanded patching options

## What is Dynamic Stochastic Synthesis?

Dynamic Stochastic Synthesis (DSS) was invented by composer Iannis Xenakis in the 1970s. The technique generates sound by creating random "walks" through a series of breakpoints in both amplitude and time domains. These breakpoints are continuously mutated using probability distributions, creating evolving, organic textures that never repeat exactly.

ReGrandy extends this concept by:
1. Adding granular synthesis processing to each segment
2. Providing FM synthesis capabilities
3. Offering multiple envelope shapes for grain windowing
4. Implementing flexible wrapping/mirroring modes for boundary conditions

## Installation

### Method 1: VCV Rack Library (Recommended)

1. Open VCV Rack
2. Click on "Library" in the menu bar
3. Search for "RestocK" or "ReGrandy"
4. Click "Subscribe" or "Install"
5. Restart VCV Rack

### Method 2: Build from Source

```bash
# Clone the repository
git clone https://github.com/shortwavlabs/restock.git
cd restock

# Install dependencies (requires VCV Rack SDK)
./install.sh

# Build the plugin
make

# Install to your VCV Rack plugins folder
make install
```

## Quick Start

1. **Add the Module**: Right-click in VCV Rack and select "RestocK > ReGrandy"
2. **Basic Sound**: Turn the FREQ knob to set the base frequency
3. **Add Variation**: Increase BPTS (breakpoints) to 12-20 for more complex waveforms
4. **Introduce Randomness**: Slowly increase ASTP (amplitude step) and DSTP (duration step)
5. **Shape the Texture**: Adjust GRAT (granulation rate) to change the grain density
6. **Connect Output**: Patch the SINE_OUTPUT to your audio destination

For detailed usage instructions, see the [Quickstart Guide](QUICKSTART.md).

## Panel Controls

### Main Parameters

| Control | Description | Range |
|---------|-------------|-------|
| **FREQ** | Base frequency of the oscillator | -4 to +3 (octaves) |
| **BPTS** | Number of breakpoints in synthesis | 3 to 50 |
| **ASTP** | Maximum amplitude step size | 0.0 to 1.0 |
| **DSTP** | Maximum duration step size | 0.0 to 1.0 |
| **GRAT** | Granulation frequency | -6 to +3 (octaves) |

### FM Synthesis Controls

| Control | Description | Range |
|---------|-------------|-------|
| **FCAR** | FM carrier frequency | -4 to +4 (octaves) |
| **FMOD** | FM modulation frequency | -4 to +4 (octaves) |
| **IMOD** | FM modulation index | -4 to +4 |

### Switches

- **PDST** (3-position): Probability distribution (Linear, Cauchy, Arcsine)
- **FMTR** (2-position): Toggle FM synthesis mode
- **MIRR** (2-position): Toggle mirror/wrap boundary behavior

### CV Inputs

All major parameters have dedicated CV inputs with attenuators:
- FREQ, BPTS, ASTP, DSTP, GRAT, FMOD, IMOD

CV inputs accept ±5V signals. Use the small attenuator knobs to control CV modulation amount.

### Outputs

- **SINE_OUTPUT**: Main audio output (±5V)
- **INV_OUTPUT**: Inverted audio output (±5V)

## Sound Design Tips

### Smooth Pads
- Low ASTP (0.1-0.3)
- Low DSTP (0.1-0.3)
- Medium BPTS (8-15)
- Slow GRAT (-3 to 0)
- Use FM mode OFF

### Chaotic Textures
- High ASTP (0.6-1.0)
- High DSTP (0.6-1.0)
- High BPTS (25-50)
- Fast GRAT (0 to +2)
- Experiment with different PDST settings

### Bell-Like Tones
- Medium ASTP (0.4-0.6)
- Low DSTP (0.1-0.3)
- Low BPTS (3-6)
- FM mode ON with high IMOD
- Use Hann or Welch envelope

### Noise and Drone
- High BPTS (30+)
- Moderate ASTP and DSTP
- Very fast GRAT (+2 to +3)
- Cauchy or Arcsine distribution

## Documentation

- **[API Reference](./docs/API.md)**: Detailed technical documentation
- **[Quickstart Guide](./docs/QUICKSTART.md)**: Step-by-step tutorials
- **[Advanced Usage](./docs/ADVANCED.md)**: Deep-dive into synthesis techniques
- **[FAQ](./docs/FAQ.md)**: Common questions and solutions
- **[Examples](./docs/examples/)**: Preset patches and use cases
- **[Changelog](./docs/CHANGELOG.md)**: Version history and updates
- **[Contributing](./docs/CONTRIBUTING.md)**: Development guidelines

## Technical Specifications

- **CPU Usage**: Low to moderate (depends on BPTS and GRAT settings)
- **Sample Rate**: Follows VCV Rack engine (typically 44.1kHz or 48kHz)
- **Polyphony**: Monophonic
- **Latency**: Zero
- **Output Level**: ±5V (standard VCV Rack audio range)

## Credits

### Original StochKit
- **Original Author**: Sam Laing (smbddha)
- **Original Repository**: [sb-StochKit](https://github.com/smbddha/sb-StochKit)

### RestocK Port
- **Developer**: Stephane Pericat
- **Company**: Shortwav Labs
- **Email**: contact@shortwavlabs.com

### Theoretical Foundation
- **Dynamic Stochastic Synthesis**: Iannis Xenakis

## License

This plugin is licensed under the **GNU General Public License v3.0 or later** (GPL-3.0-or-later).

See [LICENSE.md](LICENSE.md) for full license text.

## Support

- **Issues**: [GitHub Issues](https://github.com/shortwavlabs/restock/issues)
- **Discussions**: [VCV Rack Community Forum](https://community.vcvrack.com/)
- **Email**: contact@shortwavlabs.com

## Related Resources

- [VCV Rack Official Site](https://vcvrack.com/)
- [Iannis Xenakis - Wikipedia](https://en.wikipedia.org/wiki/Iannis_Xenakis)
- [Dynamic Stochastic Synthesis](https://en.wikipedia.org/wiki/Stochastic_synthesis)
- [Granular Synthesis Introduction](https://en.wikipedia.org/wiki/Granular_synthesis)
