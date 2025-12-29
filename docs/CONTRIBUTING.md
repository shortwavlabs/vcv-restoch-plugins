# Contributing to RestocK

Thank you for your interest in contributing to RestocK! This document provides guidelines and instructions for contributing to the project.

## Table of Contents

1. [Code of Conduct](#code-of-conduct)
2. [Getting Started](#getting-started)
3. [Development Setup](#development-setup)
4. [How to Contribute](#how-to-contribute)
5. [Coding Standards](#coding-standards)
6. [Commit Guidelines](#commit-guidelines)
7. [Pull Request Process](#pull-request-process)
8. [Testing](#testing)
9. [Documentation](#documentation)
10. [Community](#community)

---

## Code of Conduct

### Our Pledge

We are committed to providing a welcoming and inclusive experience for everyone. We pledge to:

- Be respectful and considerate
- Welcome diverse perspectives and ideas
- Accept constructive criticism gracefully
- Focus on what's best for the community
- Show empathy toward other community members

### Unacceptable Behavior

- Harassment, trolling, or discriminatory language
- Personal attacks or insults
- Publishing others' private information
- Any conduct that could reasonably be considered inappropriate

### Enforcement

Violations of this code of conduct may result in:
1. Warning
2. Temporary ban
3. Permanent ban

Report issues to: contact@shortwavlabs.com

---

## Getting Started

### Prerequisites

Before contributing, ensure you have:

- ✅ Git installed and configured
- ✅ C++ development environment
- ✅ VCV Rack 2.x SDK installed
- ✅ Basic knowledge of DSP and VCV Rack module development
- ✅ Familiarity with the RestocK plugin

### First Steps

1. **Fork the repository** on GitHub
2. **Clone your fork** locally:
   ```bash
   git clone https://github.com/YOUR_USERNAME/restock.git
   cd restock
   ```
3. **Add upstream remote**:
   ```bash
   git remote add upstream https://github.com/shortwavlabs/restock.git
   ```
4. **Create a branch** for your work:
   ```bash
   git checkout -b feature/your-feature-name
   ```

---

## Development Setup

### Installing VCV Rack SDK

1. Download the VCV Rack SDK for your platform from [VCV Rack](https://vcvrack.com/)
2. Extract to a suitable location (e.g., `/opt/Rack-SDK` or `C:\Rack-SDK`)
3. Set the `RACK_DIR` environment variable:
   ```bash
   export RACK_DIR=/path/to/Rack-SDK
   ```

### Building RestocK

```bash
# Install dependencies (if needed)
./install.sh

# Build the plugin
make

# Clean build artifacts
make clean

# Install to VCV Rack plugins folder
make install
```

### Development Environment

**Recommended IDEs:**
- Visual Studio Code with C++ extensions
- CLion
- Visual Studio (Windows)
- Xcode (macOS)

**Recommended Extensions (VS Code):**
- C/C++ (Microsoft)
- CMake Tools
- GitLens
- Markdown All in One

---

## How to Contribute

### Ways to Contribute

1. **Report Bugs**: Create detailed issue reports
2. **Request Features**: Propose new features or improvements
3. **Fix Bugs**: Submit pull requests for known issues
4. **Add Features**: Implement new functionality
5. **Improve Documentation**: Enhance or fix documentation
6. **Share Presets**: Contribute example patches
7. **Test**: Help test new releases and report findings

### Reporting Bugs

When reporting bugs, include:

**Template:**
```markdown
**Description**
[Clear description of the bug]

**Steps to Reproduce**
1. [First step]
2. [Second step]
3. [...]

**Expected Behavior**
[What you expected to happen]

**Actual Behavior**
[What actually happened]

**Environment**
- VCV Rack Version: [e.g., 2.5.0]
- RestocK Version: [e.g., 2.0.0]
- OS: [e.g., macOS 14.1, Windows 11, Ubuntu 22.04]
- CPU: [e.g., Intel i7, Apple M1]

**Additional Context**
[Screenshots, audio recordings, patch files, etc.]
```

**Good Bug Report Example:**
```markdown
**Description**
ReGrandy crashes when BPTS is rapidly modulated by audio-rate signal

**Steps to Reproduce**
1. Add ReGrandy to patch
2. Connect VCO (100Hz) to BPTS CV input
3. Set BPTS CV attenuator to 1.0
4. Start audio engine

**Expected Behavior**
Module should handle audio-rate modulation or limit input rate

**Actual Behavior**
VCV Rack crashes after 2-3 seconds

**Environment**
- VCV Rack Version: 2.5.0
- RestocK Version: 2.0.0
- OS: macOS 14.1
- CPU: Apple M2

**Additional Context**
Console shows: [error message]
Patch file: crash-test.vcv
```

### Requesting Features

When requesting features, include:

**Template:**
```markdown
**Feature Summary**
[Brief one-line description]

**Motivation**
[Why is this feature needed? What problem does it solve?]

**Proposed Solution**
[How should this feature work?]

**Alternatives Considered**
[What other approaches did you consider?]

**Additional Context**
[Mockups, examples from other modules, etc.]
```

---

## Coding Standards

### C++ Style Guide

Follow these conventions:

#### Naming

```cpp
// Classes: PascalCase
class ReGrandy : Module { };

// Functions/Methods: camelCase
void processModulationInputs();

// Variables: snake_case for locals, camelCase for members
float local_variable = 0.0f;
float memberVariable = 0.0f;

// Constants: UPPER_SNAKE_CASE
constexpr float MAX_FREQ = 3000.0f;

// Enums: PascalCase for type, UPPER_SNAKE_CASE for values
enum EnvType {
    SIN,
    TRI,
    HANN
};
```

#### Formatting

```cpp
// Indentation: 2 spaces (no tabs)
void process(const ProcessArgs &args) {
  if (condition) {
    doSomething();
  }
}

// Braces: K&R style
if (x > 0) {
  // code
}
else {
  // code
}

// Line length: 100 characters max (flexible)

// Pointer/Reference alignment: with type
float* ptr;
const ProcessArgs& args;
```

#### Comments

```cpp
// Single-line comments for brief explanations
float freq = 440.0f;  // A4 frequency

/* Multi-line comments for longer explanations
 * that need multiple lines to describe
 * complex logic or algorithms
 */

/**
 * Documentation comments for public API
 * @param args Process arguments containing sample time
 * @return void
 */
void process(const ProcessArgs &args);
```

#### Best Practices

```cpp
// Use const correctness
void process(const ProcessArgs &args) const;

// Use explicit types
float value = 1.0f;  // Not: auto value = 1.0f;

// Avoid magic numbers
constexpr float DEFAULT_FREQ = 440.0f;
osc.freq = DEFAULT_FREQ;  // Not: osc.freq = 440.0f;

// Use namespaces appropriately
namespace rack {
  // module code
}

// Prefer early returns
if (!isValid) {
  return;
}
// ... rest of function

// Initialize in constructors
ReGrandy() : Module(), memberVar(0.0f) {
  config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
}
```

### File Organization

```
src/
├── plugin.hpp           # Plugin-wide declarations
├── plugin.cpp           # Plugin initialization
├── ModuleName.hpp       # Module header
├── ModuleName.cpp       # Module implementation
└── utils/
    ├── UtilityName.hpp  # Utility headers
    └── UtilityName.cpp  # Utility implementations (if needed)
```

### Header Guards

```cpp
#ifndef __MODULE_NAME_HPP__
#define __MODULE_NAME_HPP__

// ... content ...

#endif
```

---

## Commit Guidelines

### Commit Message Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

#### Types

- **feat**: New feature
- **fix**: Bug fix
- **docs**: Documentation changes
- **style**: Code style changes (formatting, no logic change)
- **refactor**: Code refactoring
- **perf**: Performance improvements
- **test**: Adding or updating tests
- **chore**: Build process, dependencies, etc.

#### Examples

**Good Commits:**
```
feat(ReGrandy): add polyphonic support

Implement polyphonic processing for up to 16 channels.
Each channel maintains independent oscillator state.

Closes #123
```

```
fix(ReGrandy): prevent crash on rapid BPTS changes

Add rate limiting to BPTS parameter updates to prevent
zipper noise and potential crashes.

Fixes #456
```

```
docs(API): update ReGrandy parameter ranges

Clarify GRAT parameter range and add examples for
grain density calculations.
```

**Bad Commits:**
```
fixed bug          # Too vague
Update stuff       # Not descriptive
WIP                # Should not be in main branch
```

### Commit Best Practices

- ✅ One logical change per commit
- ✅ Write clear, descriptive messages
- ✅ Reference issues when applicable
- ✅ Keep commits focused and atomic
- ❌ Don't commit commented-out code
- ❌ Don't commit debug statements
- ❌ Don't mix refactoring with feature changes

---

## Pull Request Process

### Before Submitting

Checklist:

- [ ] Code follows style guidelines
- [ ] All tests pass (if applicable)
- [ ] Documentation updated (if needed)
- [ ] Commit messages are clear
- [ ] Branch is up-to-date with main
- [ ] No merge conflicts
- [ ] Self-review completed

### Creating a Pull Request

1. **Push your branch** to your fork:
   ```bash
   git push origin feature/your-feature-name
   ```

2. **Open PR** on GitHub:
   - Go to original repository
   - Click "New Pull Request"
   - Select your branch
   - Fill out PR template

### PR Template

```markdown
## Description
[Clear description of changes]

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Motivation
[Why is this change needed?]

## Changes Made
- [List specific changes]
- [...]

## Testing
[How was this tested?]

## Screenshots/Recordings
[If applicable]

## Checklist
- [ ] Code follows style guidelines
- [ ] Self-review completed
- [ ] Documentation updated
- [ ] No new warnings
- [ ] Tests pass

## Related Issues
Closes #[issue number]
```

### Review Process

1. **Automated Checks**: CI runs (if configured)
2. **Maintainer Review**: Code review by maintainers
3. **Feedback**: Address review comments
4. **Approval**: PR approved by maintainer
5. **Merge**: PR merged into main branch

### After Merge

- Delete your feature branch (optional)
- Update your fork's main branch:
  ```bash
  git checkout main
  git pull upstream main
  git push origin main
  ```

---

## Testing

### Manual Testing

Before submitting PR:

1. **Build and Run**
   ```bash
   make clean
   make
   make install
   ```

2. **Test Basic Functionality**
   - Add module to VCV Rack
   - Test all parameters
   - Test all CV inputs
   - Check outputs

3. **Test Edge Cases**
   - Extreme parameter values
   - Rapid parameter changes
   - High CPU load scenarios
   - Long-running patches

4. **Test Integration**
   - Patch with other modules
   - Different sample rates
   - Different buffer sizes

### Automated Testing

(When implemented)

```bash
# Run unit tests
make test

# Run with sanitizers (Linux/macOS)
make clean
make CFLAGS="-fsanitize=address -g"
```

### Performance Testing

Test CPU usage:

```bash
# Launch VCV Rack
# Open performance monitor
# Add ReGrandy
# Check CPU % in VCV Rack's meters
```

**Target**: < 5% CPU on reference system (i7-8700K @ 3.7GHz)

---

## Documentation

### What to Document

Document:
- Public APIs
- Complex algorithms
- Non-obvious behavior
- Parameter ranges and effects
- Usage examples

Don't over-document:
- Obvious code
- Standard patterns
- Auto-generated content

### Documentation Locations

- **Code Comments**: In `.cpp` and `.hpp` files
- **API Reference**: `docs/API.md`
- **User Guides**: `docs/QUICKSTART.md`, `docs/ADVANCED.md`
- **README**: `docs/README.md`
- **Changelog**: `docs/CHANGELOG.md`

### Documentation Standards

**Use Markdown**:
- Headers for structure
- Code blocks with language tags
- Tables for comparisons
- Links for cross-references

**Be Clear**:
- Write for your audience (users vs developers)
- Use examples
- Explain "why" not just "what"
- Keep it concise

**Keep Updated**:
- Update docs with code changes
- Remove outdated information
- Add version tags when needed

---

## Community

### Communication Channels

- **GitHub Issues**: Bug reports and feature requests
- **GitHub Discussions**: General questions and discussions
- **VCV Rack Forum**: Community support and sharing
- **Email**: contact@shortwavlabs.com

### Getting Help

If you need help:

1. Check existing documentation
2. Search GitHub issues
3. Ask in Discussions
4. Post on VCV Rack forum
5. Email maintainers (for private matters)

### Recognition

Contributors will be:
- Listed in CHANGELOG.md
- Credited in release notes
- Acknowledged in README.md (for significant contributions)

---

## License

By contributing to RestocK, you agree that your contributions will be licensed under the **GNU General Public License v3.0 or later**.

See [LICENSE.md](../LICENSE.md) for full license text.

---

## Questions?

If you have questions about contributing:

- **Email**: contact@shortwavlabs.com
- **GitHub Discussions**: [Ask a question](https://github.com/shortwavlabs/restock/discussions)
- **VCV Forum**: [RestocK thread](https://community.vcvrack.com/)

---

## Thank You!

Your contributions make RestocK better for everyone. We appreciate your time and effort! 🎉

**Happy coding!** 🎛️✨

---

**Maintained by**: Shortwav Labs  
**Last Updated**: 2024-12-28
