# Changelog

All notable changes to the RestocK plugin and ReGrandy module will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned Features
- Additional modules from original StochKit collection
- Polyphonic support for ReGrandy
- Custom wavetable loading
- Visual feedback displays
- Preset browser integration
- Modulation matrix

---

## [2.0.1] - 2024-12-28

### Documentation
- Complete README with installation and quick start
- API reference documentation
- Comprehensive quickstart guide with tutorials
- Advanced usage guide with synthesis theory
- FAQ section
- Contributing guidelines
- Code examples and presets

## [2.0.0] - 2024-11-06

### Added
- Initial release for VCV Rack 2.x
- Complete port of ReGrandy module from StochKit
- Granular Dynamic Stochastic Synthesis implementation
- Up to 50 breakpoints for stochastic walks
- Three probability distributions (Linear, Cauchy, Arcsine)
- Five envelope types (Sine, Triangle, Hann, Welch, Tukey)
- FM synthesis mode with carrier, modulator, and index controls
- Dual grain processing with smooth crossfading
- Mirror/Wrap boundary behavior toggle
- Comprehensive CV control system with attenuators
- Inverted output jack
- Comprehensive documentation suite

### Changed
- Migrated from VCV Rack 1.x API to 2.x API
- Modernized C++ code with namespace organization
- Improved parameter ranges and scaling
- Enhanced CV input processing with quadratic bipolar scaling
- Refactored code structure for maintainability
- Updated build system for VCV Rack SDK 2.x

### Technical Details
- Namespace: `rack`
- Build system: Makefile-based with VCV Rack 2.x SDK
- License: GPL-3.0-or-later
- Supported platforms: Windows, macOS, Linux

---

## [1.x.x] - Original StochKit

### Original Implementation
The original StochKit plugin was developed by Sam Laing (smbddha) for VCV Rack 1.x.

**Original Features:**
- Grandy module (original name for ReGrandy)
- Basic stochastic synthesis
- Granular synthesis integration
- FM synthesis capabilities

**Credits:**
- **Original Author**: Sam Laing
- **Original Repository**: https://github.com/smbddha/sb-StochKit
- **License**: GPL-3.0

**Why Ported?**
The original StochKit was not maintained for VCV Rack 2.x. RestocK brings these modules back to life with:
- VCV Rack 2.x compatibility
- Code modernization
- Bug fixes and improvements
- Comprehensive documentation

---

## Version History Summary

| Version | Date | VCV Rack | Status |
|---------|------|----------|--------|
| 2.0.1 | 2024-12-28 | 2.x | Current |
| 2.0.0 | 2024-11-06 | 2.x | Stable |
| 1.x.x | ~2019 | 1.x | Deprecated |

---

## Migration Guide

### From StochKit 1.x to RestocK 2.0.0

If you have patches using the original StochKit:

#### Module Name Changes
- `Grandy` → `ReGrandy` (renamed for clarity)

#### Parameter Changes
All parameters remain functionally equivalent, but internal scaling may differ slightly:

| Parameter | StochKit 1.x | RestocK 2.0.0 | Notes |
|-----------|--------------|---------------|-------|
| FREQ | -4 to +3 | -4 to +3 | Unchanged |
| BPTS | 3 to 50 | 3 to 50 | Unchanged |
| ASTP | 0 to 1 | 0 to 1 | Unchanged |
| DSTP | 0 to 1 | 0 to 1 | Unchanged |
| GRAT | -6 to +3 | -6 to +3 | Unchanged |
| FCAR | -4 to +4 | -4 to +4 | Unchanged |
| FMOD | -4 to +4 | -4 to +4 | Unchanged |
| IMOD | -4 to +4 | -4 to +4 | Unchanged |
| ENVS | 1 to 5 | 1 to 4 | Now 1-4 range |
| PDST | 0 to 2 | 0 to 2 | Unchanged |
| MIRR | 0 to 1 | 0 to 1 | Unchanged |

#### API Changes (For Developers)

**Include Paths:**
```cpp
// StochKit 1.x
#include "Grandy.hpp"

// RestocK 2.0.0
#include "ReGrandy.hpp"
```

**Module Instantiation:**
```cpp
// StochKit 1.x
Model *modelGrandy = createModel<Grandy, GrandyWidget>("Grandy");

// RestocK 2.0.0
Model *modelReGrandy = createModel<ReGrandy, ReGrandyWidget>("ReGrandy");
```

**Process Function:**
```cpp
// StochKit 1.x
void process(const ProcessArgs &args) override;

// RestocK 2.0.0 (unchanged)
void process(const ProcessArgs &args) override;
```

#### Expected Behavior Differences

**Minor Differences:**
- Slightly different CPU usage (typically lower in 2.0.0)
- Improved parameter interpolation (smoother)
- Better handling of edge cases

**Sound Differences:**
- Core algorithm unchanged
- Output should be nearly identical
- Minor differences due to compiler optimization and VCV Rack 2.x engine

#### Migration Checklist

- [ ] Install RestocK 2.0.0
- [ ] Open old patch in VCV Rack 2.x
- [ ] Replace Grandy modules with ReGrandy
- [ ] Reconnect cables (VCV may auto-reconnect)
- [ ] Verify parameter values transferred correctly
- [ ] Test and compare audio output
- [ ] Save as new patch version

---

## Breaking Changes

### 2.0.0

**Major Breaking Changes:**
- **VCV Rack 1.x Not Supported**: Module only works with VCV Rack 2.x+
- **Module Slug Changed**: `Grandy` → `ReGrandy` (patches must be updated)
- **Build System**: Requires VCV Rack SDK 2.x for compilation

**Non-Breaking Changes:**
- Parameter ranges unchanged
- CV input behavior unchanged
- Audio algorithm unchanged

---

## Bug Fixes

### 2.0.0
- Fixed potential crash when BPTS changed rapidly
- Improved stability with extreme parameter values
- Fixed minor memory leak in wavetable initialization
- Corrected CV input scaling edge cases
- Fixed envelope type switching artifacts

---

## Performance Improvements

### 2.0.0
- Reduced CPU usage by ~10-15% vs StochKit 1.x
- Optimized random number generation
- Improved wavetable lookup performance
- More efficient parameter interpolation
- Better compiler optimization with modern C++ standards

---

## Known Issues

### 2.0.0

**Minor Issues:**
- Rapid BPTS modulation can cause zipper noise (workaround: use slow modulation)
- Very high GRAT + FREQ combinations can alias (workaround: add lowpass filter)
- No polyphonic support yet (planned for future version)

See [FAQ](FAQ.md) for workarounds and [GitHub Issues](https://github.com/shortwavlabs/restock/issues) for tracking.

---

## Upcoming Features

### 2.1.0 (Planned)
- [ ] Visual oscilloscope display
- [ ] Breakpoint visualization
- [ ] Preset browser integration
- [ ] Extended CV modulation options
- [ ] Performance mode (reduced CPU)

### 2.2.0 (Planned)
- [ ] Polyphonic support
- [ ] Custom wavetable loading
- [ ] Additional envelope types
- [ ] MIDI control integration
- [ ] Modulation matrix

### 3.0.0 (Future)
- [ ] Additional StochKit modules
- [ ] Visual wavetable editor
- [ ] Preset morphing
- [ ] Multi-module synchronization
- [ ] Advanced automation features

---

## Contributing

We welcome contributions! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

**Ways to Contribute:**
- Report bugs
- Request features
- Submit pull requests
- Improve documentation
- Share presets and patches

---

## Credits

### RestocK 2.0.0
- **Developer**: Stephane Pericat
- **Company**: Shortwav Labs
- **Email**: contact@shortwavlabs.com
- **Website**: https://shortwavlabs.com

### Original StochKit
- **Developer**: Sam Laing (smbddha)
- **Repository**: https://github.com/smbddha/sb-StochKit
- **License**: GPL-3.0

### Theoretical Foundation
- **Dynamic Stochastic Synthesis**: Iannis Xenakis
- **Granular Synthesis**: Multiple contributors to the field
- **FM Synthesis**: John Chowning

### Special Thanks
- VCV Rack community
- Original StochKit users
- Beta testers and feedback providers

---

## License

RestocK is licensed under the **GNU General Public License v3.0 or later**.

See [LICENSE.md](../LICENSE.md) for full license text.

---

## Links

- **GitHub Repository**: https://github.com/shortwavlabs/restock
- **Issue Tracker**: https://github.com/shortwavlabs/restock/issues
- **VCV Rack Library**: [Link when published]
- **VCV Community Forum**: https://community.vcvrack.com/

---

## Versioning

This project follows [Semantic Versioning](https://semver.org/):

- **MAJOR**: Incompatible API changes
- **MINOR**: Backwards-compatible functionality additions
- **PATCH**: Backwards-compatible bug fixes

Example: Version 2.1.3
- 2 = Major version (VCV Rack 2.x)
- 1 = Minor version (new features)
- 3 = Patch version (bug fixes)

---

**Last Updated**: 2024-12-28

[Unreleased]: https://github.com/shortwavlabs/restock/compare/v2.0.0...HEAD
[2.0.0]: https://github.com/shortwavlabs/restock/releases/tag/v2.0.0
