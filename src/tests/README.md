# Restock Plugin Unit Tests

This directory contains comprehensive unit tests for the Restock VCV Rack plugin utilities.

## Test Files

### wavetable_test.cpp
Tests for wavetable utilities including:
- `wrap()` and `mirror()` utility functions (5 tests each)
- `gRandGen` random generation with different distributions (4 tests)
- `Wavetable` class initialization and operations (15 tests)
- All envelope types: SIN, TRI, HANN, WELCH, TUKEY

**Total: 29 test cases, 53 assertions**

### GrandyOscillator_test.cpp
Tests for the GendyOscillator (granular stochastic dynamic synthesis):
- Initialization and state (3 tests)
- `wrap()` and `mirror()` methods (8 tests)
- `process()` method with various scenarios (7 tests)
- Output validation (3 tests)
- Configuration options: FM synthesis, mirroring, breakpoints (5 tests)
- Edge cases and boundary conditions (6 tests)

**Total: 32 test cases, 2313 assertions**

### Limiter_test.cpp
Tests for the AudioLimiter (dynamic limiter and anti-clipping system):
- Initialization and configuration (1 test)
- Signal passthrough at safe levels (1 test)
- Peak limiting and gain reduction (1 test)
- Attack and release response (2 tests)
- Lookahead processing (1 test)
- Automatic makeup gain (1 test)
- Hard clipping protection (1 test)
- Reset functionality (1 test)
- Different sample rates (1 test)
- Continuous signal processing (1 test)
- Transient handling (1 test)

**Total: 12 test cases, 50000+ assertions**

## Running Tests

From the repository root:

```bash
# Run all tests
./run_tests.sh

# Run a specific test
g++ -std=c++11 -I. -o Limiter_test src/tests/Limiter_test.cpp && ./Limiter_test

# Clean build and run
./run_tests.sh --clean

# Run without rebuilding
./run_tests.sh --no-build

# Show verbose compilation output
./run_tests.sh --verbose

# Show help
./run_tests.sh --help
```

## Test Architecture

The tests are designed as standalone executables that:
- Include minimal mocks of the Rack SDK to avoid linking dependencies
- Embed the code-under-test directly to simplify compilation
- Use a simple assertion-based testing framework
- Provide clear, color-coded output
- Return proper exit codes for CI/CD integration

## Test Coverage

The tests verify:
- ✓ Expected behavior under normal conditions
- ✓ Edge cases and boundary conditions
- ✓ Error handling and invalid inputs
- ✓ State consistency across operations
- ✓ Configuration options and modes
- ✓ Long-running continuous operation
- ✓ Mathematical correctness of algorithms

## Bug Fixes

During test development, the following bug was identified and fixed:
- **mirror() function**: Corrected reflection logic to properly bounce values off boundaries
  - Changed from: `out = out - (in - ub)` to `out = ub - (in - ub)`
  - Applied to both [wavetable.cpp](../utils/wavetable.cpp) and [GrandyOscillator.hpp](../utils/GrandyOscillator.hpp)
