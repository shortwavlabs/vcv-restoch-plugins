# RestocK Documentation Index

Complete documentation for the RestocK VCV Rack plugin and ReGrandy module.

## 📚 Documentation Structure

```
docs/
├── README.md              ⭐ Main overview and quick start
├── API.md                 🔧 Complete API reference
├── QUICKSTART.md          🚀 Step-by-step tutorials
├── ADVANCED.md            🎓 Deep dive and advanced techniques
├── FAQ.md                 ❓ Frequently asked questions
├── CHANGELOG.md           📋 Version history
├── CONTRIBUTING.md        🤝 Developer guidelines
└── examples/              💡 Real-world patches and presets
    ├── README.md          📖 Examples index
    ├── basic/             🟢 Beginner examples
    ├── intermediate/      🟡 Intermediate examples
    ├── advanced/          🔴 Advanced examples
    ├── generative/        🔵 Generative patches
    ├── performance/       🟣 Performance patches
    └── sound-effects/     🟠 Sound design examples
```

---

## 🎯 Quick Navigation

### New Users Start Here

1. **[Main README](README.md)** - Overview, features, installation
2. **[QuickStart Guide](QUICKSTART.md)** - Your first sounds in minutes
3. **[Example 01: First Sound](examples/basic/01-first-sound.md)** - Hands-on tutorial
4. **[FAQ](FAQ.md)** - Common questions answered

### Intermediate Users

1. **[QuickStart Tutorials](QUICKSTART.md#tutorial-1-smooth-pad)** - Four complete tutorials
2. **[Examples Directory](examples/README.md)** - 24+ example patches
3. **[FAQ](FAQ.md)** - Tips and tricks

### Advanced Users

1. **[Advanced Usage Guide](ADVANCED.md)** - Synthesis theory and techniques
2. **[Advanced Examples](examples/README.md#-advanced-experienced-users)** - Complex patches
3. **[API Reference](API.md)** - Technical documentation

### Developers

1. **[Contributing Guidelines](CONTRIBUTING.md)** - How to contribute
2. **[API Reference](API.md)** - Code documentation
3. **[Changelog](CHANGELOG.md)** - Version history

---

## 📖 Documentation by Topic

### Installation & Setup

- [Installation Instructions](README.md#installation)
- [Building from Source](CONTRIBUTING.md#development-setup)
- [System Requirements](README.md#technical-specifications)

### Learning ReGrandy

- [What is Dynamic Stochastic Synthesis?](README.md#what-is-dynamic-stochastic-synthesis)
- [Understanding Parameters](QUICKSTART.md#understanding-the-parameters)
- [First Sound Tutorial](examples/basic/01-first-sound.md)
- [Parameter Deep Dive](ADVANCED.md#probability-distributions-deep-dive)

### Sound Design

- [Sound Design Tips](README.md#sound-design-tips)
- [Sound Design Recipes](ADVANCED.md#sound-design-recipes)
- [Example Patches](examples/README.md)
- [Preset Collection](examples/README.md#preset-collection)

### Technical Reference

- [Module Class](API.md#regrandy-module-class)
- [GendyOscillator Class](API.md#gendyoscillator-class)
- [Wavetable Class](API.md#wavetable-class)
- [Enumerations](API.md#enumerations)
- [Constants](API.md#constants)

### Patching & Routing

- [CV Modulation](QUICKSTART.md#working-with-cv-modulation)
- [Advanced Patching](ADVANCED.md#advanced-patching-techniques)
- [Integration with Other Modules](ADVANCED.md#integration-with-other-modules)
- [Patching FAQ](FAQ.md#patching--routing)

### Troubleshooting

- [Troubleshooting Guide](QUICKSTART.md#troubleshooting)
- [FAQ - Sound Issues](FAQ.md#sound-issues)
- [Performance Optimization](ADVANCED.md#performance-optimization)
- [Known Limitations](ADVANCED.md#known-limitations)

---

## 🎓 Learning Paths

### Path 1: Complete Beginner (2-3 hours)

```
1. Read Main README (15 min)
   ↓
2. QuickStart: First Sound (10 min)
   ↓
3. Example 01 (10 min)
   ↓
4. QuickStart Tutorial 1 (20 min)
   ↓
5. Experiment! (60+ min)
   ↓
6. FAQ as needed
```

### Path 2: Experienced Synthesist (1-2 hours)

```
1. Skim Main README (5 min)
   ↓
2. Read Parameter Descriptions (10 min)
   ↓
3. Try Examples 02-04 (30 min)
   ↓
4. Read Advanced Guide (30 min)
   ↓
5. Try Advanced Examples (30+ min)
```

### Path 3: Developer (2-4 hours)

```
1. Read Contributing Guidelines (20 min)
   ↓
2. Setup Development Environment (30 min)
   ↓
3. Read API Reference (45 min)
   ↓
4. Study Source Code (60 min)
   ↓
5. Build & Test (30 min)
```

---

## 📝 Document Descriptions

### [README.md](README.md)
**Main overview document**
- Plugin description and features
- Installation instructions
- Quick start guide
- Panel controls reference
- Sound design tips
- Credits and license

**Read this first!**

### [API.md](API.md)
**Complete technical reference**
- Module class documentation
- Function signatures and parameters
- Constants and enumerations
- Code examples
- Performance considerations

**For developers and advanced users**

### [QUICKSTART.md](QUICKSTART.md)
**Hands-on learning guide**
- Your first sound (5 minutes)
- Four complete tutorials
- CV modulation guide
- Troubleshooting
- Quick reference card

**Best for beginners**

### [ADVANCED.md](ADVANCED.md)
**Deep dive into synthesis**
- Dynamic Stochastic Synthesis theory
- Probability distribution mathematics
- Granular synthesis concepts
- FM synthesis techniques
- Advanced patching
- Sound design recipes
- Performance optimization

**For experienced users**

### [FAQ.md](FAQ.md)
**Common questions and answers**
- General questions
- Parameter explanations
- Sound issues
- Performance optimization
- Patching tips
- Comparisons with other modules

**Quick reference for everyone**

### [CHANGELOG.md](CHANGELOG.md)
**Version history**
- Release notes
- Bug fixes
- New features
- Breaking changes
- Migration guides

**Track updates**

### [CONTRIBUTING.md](CONTRIBUTING.md)
**Developer guidelines**
- Code of conduct
- Development setup
- Coding standards
- Commit guidelines
- Pull request process
- Testing procedures

**For contributors**

### [examples/README.md](examples/README.md)
**Example patches index**
- 24+ categorized examples
- Beginner to advanced
- Multiple genres
- Step-by-step instructions
- Variations and tips

**Learn by doing**

---

## 🔍 Search by Need

### "I want to..."

**...get started quickly**
→ [QuickStart Guide](QUICKSTART.md)

**...understand a specific parameter**
→ [Parameter Reference](API.md#parameters-paramids)

**...learn synthesis theory**
→ [Advanced Guide - Theory](ADVANCED.md#understanding-dynamic-stochastic-synthesis)

**...create ambient pads**
→ [Tutorial 1: Smooth Pad](QUICKSTART.md#tutorial-1-smooth-pad)

**...make chaotic textures**
→ [Tutorial 4: Chaotic Noise](QUICKSTART.md#tutorial-4-chaotic-noise)

**...use FM synthesis**
→ [FM Synthesis in ReGrandy](ADVANCED.md#fm-synthesis-in-regrandy)

**...optimize CPU usage**
→ [Performance Optimization](ADVANCED.md#performance-optimization)

**...report a bug**
→ [Contributing - Bug Reports](CONTRIBUTING.md#reporting-bugs)

**...contribute code**
→ [Contributing Guidelines](CONTRIBUTING.md)

**...find example patches**
→ [Examples Directory](examples/README.md)

---

## 📊 Documentation Statistics

- **Total Pages**: 9 major documents
- **Example Patches**: 24+ detailed examples
- **Code Examples**: 50+ snippets
- **Tutorials**: 4 complete walkthroughs
- **Word Count**: ~50,000+ words
- **Estimated Reading Time**: 6-8 hours (complete)

---

## 🌐 External Resources

### VCV Rack

- [VCV Rack Official Site](https://vcvrack.com/)
- [VCV Rack Manual](https://vcvrack.com/manual/)
- [VCV Community Forum](https://community.vcvrack.com/)

### Synthesis Theory

- [Dynamic Stochastic Synthesis - Wikipedia](https://en.wikipedia.org/wiki/Stochastic_synthesis)
- [Granular Synthesis - Wikipedia](https://en.wikipedia.org/wiki/Granular_synthesis)
- [FM Synthesis - Wikipedia](https://en.wikipedia.org/wiki/Frequency_modulation_synthesis)

### Academic References

- Xenakis, I. (1992). *Formalized Music: Thought and Mathematics in Composition*
- Roads, C. (2001). *Microsound*
- Chowning, J. (1973). "The Synthesis of Complex Audio Spectra by Means of Frequency Modulation"

---

## 📥 Download & Print

All documentation is available as:
- **Markdown** (GitHub)
- **Web HTML** (rendered on GitHub)
- **PDF** (coming soon)

**To save offline:**
```bash
git clone https://github.com/shortwavlabs/restock.git
cd restock/docs
# Open in your favorite markdown viewer
```

---

## 🆘 Getting Help

### Can't find what you need?

1. **Check FAQ**: [FAQ.md](FAQ.md)
2. **Search Issues**: [GitHub Issues](https://github.com/shortwavlabs/restock/issues)
3. **Ask Community**: [VCV Forum](https://community.vcvrack.com/)
4. **Email Support**: contact@shortwavlabs.com

### Found an error in documentation?

1. [Open an issue](https://github.com/shortwavlabs/restock/issues)
2. Or submit a pull request (see [Contributing](CONTRIBUTING.md))

---

## 📜 License

All documentation is licensed under **Creative Commons Attribution-ShareAlike 4.0 International** (CC BY-SA 4.0).

Code and plugin are licensed under **GPL-3.0-or-later**.

See [LICENSE.md](../LICENSE.md) for details.

---

## 🙏 Credits

**Documentation by**: Shortwav Labs  
**Original Plugin**: Sam Laing (StochKit)  
**Theory**: Iannis Xenakis (Dynamic Stochastic Synthesis)

**Contributors**: [See CHANGELOG.md](CHANGELOG.md#credits)

---

## 📅 Last Updated

**Date**: 2024-12-28  
**Version**: RestocK 2.0.0  
**Status**: Current

---

**Happy learning! 🎛️✨**

*Navigate using the links above or browse the docs folder directly.*
