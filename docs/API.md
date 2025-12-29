# API Reference

Complete technical reference for the ReGrandy module.

## Table of Contents

1. [Module Overview](#module-overview)
2. [ReGrandy Module Class](#regrandy-module-class)
3. [GendyOscillator Class](#gendyoscillator-class)
4. [Wavetable Class](#wavetable-class)
5. [Enumerations](#enumerations)
6. [Constants](#constants)
7. [Utility Functions](#utility-functions)

---

## Module Overview

**Namespace**: `rack`  
**Header Files**: 
- `ReGrandy.hpp`
- `utils/GrandyOscillator.hpp`
- `utils/wavetable.hpp`

---

## ReGrandy Module Class

The main VCV Rack module class implementing the user interface and parameter processing.

### Class Definition

```cpp
struct ReGrandy : Module
```

### Parameters (ParamIds)

| Parameter ID | Name | Description | Range | Unit |
|--------------|------|-------------|-------|------|
| `FREQ_PARAM` | Frequency | Base oscillator frequency | -4.0 to 3.0 | Octaves (relative to C4) |
| `FREQCV_PARAM` | Frequency CV Amount | Attenuator for frequency CV input | 0.0 to 1.0 | Scale |
| `BPTS_PARAM` | Breakpoints | Number of stochastic breakpoints | 3 to 50 | Count |
| `BPTSCV_PARAM` | Breakpoints CV Amount | Attenuator for breakpoints CV | 0.0 to 1.0 | Scale |
| `ASTP_PARAM` | Amplitude Step | Maximum amplitude variation per step | 0.0 to 1.0 | Scale |
| `ASTPCV_PARAM` | Amplitude Step CV Amount | Attenuator for amplitude step CV | 0.0 to 1.0 | Scale |
| `DSTP_PARAM` | Duration Step | Maximum duration variation per step | 0.0 to 1.0 | Scale |
| `DSTPCV_PARAM` | Duration Step CV Amount | Attenuator for duration step CV | 0.0 to 1.0 | Scale |
| `GRAT_PARAM` | Granulation Rate | Frequency of grain envelope | -6.0 to 3.0 | Octaves (relative to C4) |
| `GRATCV_PARAM` | Granulation Rate CV Amount | Attenuator for grain rate CV | 0.0 to 1.0 | Scale |
| `FCAR_PARAM` | FM Carrier Frequency | Carrier frequency for FM synthesis | -4.0 to 4.0 | Octaves (relative to C4) |
| `FMOD_PARAM` | FM Modulator Frequency | Modulator frequency for FM synthesis | -4.0 to 4.0 | Octaves (relative to C4) |
| `FMODCV_PARAM` | FM Modulator CV Amount | Attenuator for FM mod frequency CV | 0.0 to 1.0 | Scale |
| `IMOD_PARAM` | FM Modulation Index | Amount of frequency modulation | -4.0 to 4.0 | Scale |
| `IMODCV_PARAM` | FM Index CV Amount | Attenuator for FM index CV | 0.0 to 1.0 | Scale |
| `ENVS_PARAM` | Envelope Type | Grain window shape selector | 1 to 4 | Enum |
| `PDST_PARAM` | Probability Distribution | Random distribution type | 0 to 2 | Enum |
| `MIRR_PARAM` | Mirror Mode | Boundary behavior toggle | 0 to 1 | Boolean |
| `FMTR_PARAM` | FM Toggle | Enable/disable FM synthesis | 0 to 1 | Boolean |

### Inputs (InputIds)

| Input ID | Name | Expected Range | Description |
|----------|------|----------------|-------------|
| `FREQ_INPUT` | Frequency CV | ±5V | Controls oscillator pitch |
| `BPTS_INPUT` | Breakpoints CV | ±5V | Modulates number of breakpoints |
| `ASTP_INPUT` | Amplitude Step CV | ±5V | Modulates amplitude randomness |
| `DSTP_INPUT` | Duration Step CV | ±5V | Modulates duration randomness |
| `GRAT_INPUT` | Granulation Rate CV | ±5V | Modulates grain frequency |
| `FMOD_INPUT` | FM Modulator CV | ±5V | Modulates FM frequency |
| `IMOD_INPUT` | FM Index CV | ±5V | Modulates FM depth |

### Outputs (OutputIds)

| Output ID | Name | Range | Description |
|-----------|------|-------|-------------|
| `SINE_OUTPUT` | Main Output | ±5V | Primary audio signal |
| `INV_OUTPUT` | Inverted Output | ±5V | Phase-inverted audio signal |

### Member Variables

```cpp
GendyOscillator go;           // Main synthesis engine
EnvType env;                  // Current envelope type
float freq_sig;               // Frequency modulation signal
float astp_sig;               // Amplitude step modulation signal
float dstp_sig;               // Duration step modulation signal
float grat_sig;               // Granulation rate modulation signal
float bpts_sig;               // Breakpoints modulation signal
float fmod_sig;               // FM modulator modulation signal
float imod_sig;               // FM index modulation signal
bool fm_is_on;                // FM synthesis state
```

### Methods

#### Constructor

```cpp
ReGrandy()
```

Initializes the module, configuring all parameters with their ranges and default values.

**Example:**
```cpp
// Module is automatically instantiated by VCV Rack
// User doesn't need to call constructor directly
```

#### process()

```cpp
void process(const ProcessArgs &args) override
```

Main audio processing function called every sample.

**Parameters:**
- `args`: ProcessArgs structure containing `sampleTime` (delta time per sample)

**Process Flow:**
1. Updates envelope type if changed
2. Processes all CV modulation inputs
3. Updates granular synthesis parameters
4. Updates FM synthesis parameters
5. Calls oscillator processing
6. Outputs audio to SINE_OUTPUT and INV_OUTPUT

**Example Usage (internal):**
```cpp
// Called automatically by VCV Rack engine
void ReGrandy::process(const ProcessArgs &args) {
    // Process one sample
    updateEnvelopeType(args);
    processModulationInputs();
    updateGranularParameters();
    updateFMParameters();
    go.process(args.sampleTime);
    outputs[SINE_OUTPUT].setVoltage(5.0f * go.out());
}
```

#### updateEnvelopeType()

```cpp
void updateEnvelopeType(const ProcessArgs &args)
```

Updates the grain envelope shape based on the ENVS parameter.

**Parameters:**
- `args`: ProcessArgs (not currently used but available for future expansion)

**Behavior:**
- Reads ENVS_PARAM value (1-4)
- Converts to EnvType enum
- Switches oscillator envelope if changed

#### processModulationInputs()

```cpp
void processModulationInputs()
```

Reads and scales all CV inputs according to their attenuator settings.

**Modulation Scaling:**
- CV inputs are normalized to ±1.0 range (dividing by 5V)
- Multiplied by corresponding CV attenuator parameter
- Uses `quadraticBipolar()` for exponential response on some parameters

#### updateGranularParameters()

```cpp
void updateGranularParameters()
```

Updates all stochastic synthesis parameters in the GendyOscillator.

**Parameters Updated:**
- `go.num_bpts`: Number of breakpoints
- `go.freq`: Main oscillator frequency
- `go.max_amp_step`: Maximum amplitude variation
- `go.max_dur_step`: Maximum duration variation
- `go.freq_mul`: Frequency multiplier
- `go.g_rate`: Grain envelope rate

#### updateFMParameters()

```cpp
void updateFMParameters()
```

Updates FM synthesis parameters in the GendyOscillator.

**Parameters Updated:**
- `go.is_fm_on`: FM enable state
- `go.f_car`: Carrier frequency
- `go.f_mod`: Modulator frequency
- `go.i_mod`: Modulation index

---

## GendyOscillator Class

Core synthesis engine implementing Granular Dynamic Stochastic Synthesis.

### Class Definition

```cpp
struct GendyOscillator
```

### Public Members

#### Control Variables

```cpp
bool GRAN_ON;              // Enable granular synthesis (currently always true)
bool is_fm_on;             // Enable FM synthesis mode
bool is_mirroring;         // Use mirroring vs wrapping for boundaries
int num_bpts;              // Number of active breakpoints (3-50)
float freq;                // Main oscillator frequency (Hz)
float g_rate;              // Grain envelope frequency (Hz)
float max_amp_step;        // Maximum amplitude random walk step (0.05-0.3)
float max_dur_step;        // Maximum duration random walk step (0.01-0.3)
DistType dt;               // Probability distribution type
```

#### FM Synthesis Parameters

```cpp
float f_car;               // FM carrier frequency (Hz)
float f_mod;               // FM modulator frequency (Hz)
float i_mod;               // FM modulation index (10-3000)
```

#### Internal State Arrays

```cpp
float amps[MAX_BPTS];      // Breakpoint amplitudes (-1.0 to 1.0)
float durs[MAX_BPTS];      // Breakpoint durations (0.5 to 1.5)
float offs[MAX_BPTS];      // Grain sample offsets (0.0 to 1.0)
float rats[MAX_BPTS];      // Grain playback rates (0.7 to 1.3)
```

### Methods

#### process()

```cpp
void process(float deltaTime)
```

Advances the synthesis engine by one sample.

**Parameters:**
- `deltaTime`: Time elapsed since last sample (1/sampleRate)

**Algorithm:**
1. Check if phase has wrapped (reached next breakpoint)
2. If wrapped:
   - Apply stochastic walk to current breakpoint values
   - Apply wrapping or mirroring to keep values in bounds
   - Update grain parameters
   - Calculate next segment speed
3. Interpolate between current and next breakpoint
4. Apply grain envelope and wavetable/FM oscillator
5. Advance all phases

**Example:**
```cpp
GendyOscillator osc;
osc.freq = 440.0f;
osc.num_bpts = 12;
osc.max_amp_step = 0.1f;
osc.max_dur_step = 0.1f;
osc.g_rate = 1000.0f;

// Process one sample at 44.1kHz
float deltaTime = 1.0f / 44100.0f;
osc.process(deltaTime);
float sample = osc.out();
```

#### out()

```cpp
float out()
```

Returns the current output sample value.

**Returns:**
- `float`: Audio sample in range approximately -1.0 to 1.0

**Example:**
```cpp
float audioSample = oscillator.out();
outputs[AUDIO_OUT].setVoltage(5.0f * audioSample);
```

#### wrap()

```cpp
float wrap(float in, float lb, float ub)
```

Wraps a value to stay within bounds (hard boundary).

**Parameters:**
- `in`: Input value
- `lb`: Lower bound
- `ub`: Upper bound

**Returns:**
- If `in > ub`, returns `lb`
- If `in < lb`, returns `ub`
- Otherwise returns `in`

**Example:**
```cpp
float wrapped = osc.wrap(1.2f, 0.0f, 1.0f);  // Returns 0.0f
```

#### mirror()

```cpp
float mirror(float in, float lb, float ub)
```

Mirrors a value at boundaries (soft boundary).

**Parameters:**
- `in`: Input value
- `lb`: Lower bound
- `ub`: Upper bound

**Returns:**
- If `in > ub`, returns `ub - (in - ub)`
- If `in < lb`, returns `lb - (in - lb)`
- Otherwise returns `in`

**Example:**
```cpp
float mirrored = osc.mirror(1.2f, 0.0f, 1.0f);  // Returns 0.8f
```

---

## Wavetable Class

Manages wavetables for grain windowing and FM oscillators.

### Class Definition

```cpp
struct Wavetable
```

### Constants

```cpp
#define TABLE_SIZE 2048
```

### Members

```cpp
float table[TABLE_SIZE];   // Wavetable buffer
EnvType et;                // Current envelope type
```

### Constructors

#### Default Constructor

```cpp
Wavetable()
```

Creates a wavetable with SIN type (one cycle of sine wave).

#### Parameterized Constructor

```cpp
Wavetable(EnvType e)
```

Creates a wavetable with specified envelope type.

**Parameters:**
- `e`: EnvType enum value (SIN, TRI, HANN, WELCH, TUKEY)

### Methods

#### init()

```cpp
void init(EnvType e)
```

Initializes the wavetable with the specified envelope shape.

**Parameters:**
- `e`: EnvType to generate

**Supported Types:**
- `SIN`: Full sine wave cycle
- `TRI`: Triangle wave
- `HANN`: Hann window (raised cosine)
- `WELCH`: Welch window (parabolic)
- `TUKEY`: Tukey window (tapered cosine)

#### get()

```cpp
float get(float phase)
```

Retrieves interpolated value from wavetable at normalized phase.

**Parameters:**
- `phase`: Normalized position (0.0 to 1.0)

**Returns:**
- Interpolated wavetable value

**Example:**
```cpp
Wavetable wt(HANN);
float value = wt.get(0.25f);  // Get value at 25% through table
```

#### switchEnvType()

```cpp
void switchEnvType(EnvType e)
```

Changes the envelope type and regenerates the table.

**Parameters:**
- `e`: New EnvType

---

## Enumerations

### EnvType

Envelope/window shapes for grain processing.

```cpp
enum EnvType {
    SIN,      // Sine wave (smooth periodic)
    TRI,      // Triangle (linear slopes)
    HANN,     // Hann window (raised cosine, no discontinuity)
    WELCH,    // Welch window (parabolic)
    TUKEY,    // Tukey window (tapered cosine)
    NUM_ENVS  // Count of envelope types
};
```

**Use Cases:**
- **SIN**: General purpose, smooth grain edges
- **TRI**: Sharper attacks and releases
- **HANN**: Smoothest grain windowing, minimal clicks
- **WELCH**: Broader frequency spectrum
- **TUKEY**: Adjustable taper, good for percussive grains

### DistType

Probability distributions for stochastic walks.

```cpp
enum DistType {
    LINEAR,   // Uniform distribution
    CAUCHY,   // Cauchy distribution (heavy tails)
    ARCSINE   // Arcsine distribution (peaks at extremes)
};
```

**Characteristics:**
- **LINEAR**: Even probability across range, predictable variation
- **CAUCHY**: Occasional large jumps, creates dramatic changes
- **ARCSINE**: Tends toward extreme values, bimodal behavior

---

## Constants

### Frequency Ranges

```cpp
constexpr float MIN_FREQ = 1.0f;       // Minimum oscillator frequency (Hz)
constexpr float MAX_FREQ = 3000.0f;    // Maximum oscillator frequency (Hz)
constexpr float MIN_G_RATE = 1e-6f;    // Minimum grain rate (Hz)
constexpr float MAX_G_RATE = 3000.0f;  // Maximum grain rate (Hz)
constexpr float MAX_F_CAR = 5000.0f;   // Maximum FM carrier frequency (Hz)
```

### Step Ranges

```cpp
constexpr float MIN_AMP_STEP = 0.05f;  // Minimum amplitude step
constexpr float MAX_AMP_STEP = 0.3f;   // Maximum amplitude step
constexpr float MIN_DUR_STEP = 0.01f;  // Minimum duration step
constexpr float MAX_DUR_STEP = 0.3f;   // Maximum duration step
```

### Modulation Ranges

```cpp
constexpr float MIN_I_MOD = 10.0f;     // Minimum FM index
constexpr float MAX_I_MOD = 3000.0f;   // Maximum FM index
constexpr float MIN_FREQ_MUL = 0.05f;  // Minimum frequency multiplier
constexpr float MAX_FREQ_MUL = 4.0f;   // Maximum frequency multiplier
```

### Breakpoints

```cpp
#define MAX_BPTS 50               // Maximum number of breakpoints
constexpr float MIN_BPTS = 2;     // Minimum number of breakpoints
```

### Voltage Scaling

```cpp
constexpr float VOLTAGE_SCALE = 5.0f;      // CV voltage range (±5V)
constexpr float BIPOLAR_SCALE = 0.25f;     // Bipolar scaling factor
```

---

## Utility Functions

### gRandGen struct

Random number generator with probability distribution shaping.

```cpp
struct gRandGen {
    float my_rand(DistType t, float rand);
};
```

#### my_rand()

```cpp
float my_rand(DistType t, float rand)
```

Transforms uniform random input through inverse distribution function.

**Parameters:**
- `t`: Distribution type (LINEAR, CAUCHY, ARCSINE)
- `rand`: Uniform random input from `rack::random::normal()` (mean=0, std=1)

**Returns:**
- Transformed random value suitable for stochastic walks

**Algorithm:**
- **LINEAR**: Returns input unchanged
- **CAUCHY**: Applies inverse Cauchy transform `(1/a) * tan(c * (2*rand - 1))`
- **ARCSINE**: Applies inverse arcsine transform `sin(π * (rand - 0.5) * a) / c`

**Example:**
```cpp
gRandGen rg;
float uniform = rack::random::normal();
float cauchy = rg.my_rand(CAUCHY, uniform);  // Heavy-tailed distribution
```

### Global Utility Functions

```cpp
float wrap(float in, float lb, float ub)
float mirror(float in, float lb, float ub)
```

See GendyOscillator methods above - these are also available as global functions.

---

## Usage Examples

### Basic Oscillator Setup

```cpp
#include "utils/GrandyOscillator.hpp"

GendyOscillator osc;

// Configure parameters
osc.num_bpts = 12;
osc.freq = 440.0f;
osc.max_amp_step = 0.15f;
osc.max_dur_step = 0.1f;
osc.g_rate = 2000.0f;
osc.dt = LINEAR;
osc.is_mirroring = true;
osc.is_fm_on = false;

// Process audio
float sampleRate = 44100.0f;
float deltaTime = 1.0f / sampleRate;

for (int i = 0; i < bufferSize; i++) {
    osc.process(deltaTime);
    buffer[i] = osc.out();
}
```

### FM Synthesis Mode

```cpp
// Enable FM synthesis
osc.is_fm_on = true;
osc.f_car = 880.0f;     // Carrier at A5
osc.f_mod = 110.0f;     // Modulator at A2
osc.i_mod = 500.0f;     // Moderate modulation index

// Process with FM
osc.process(deltaTime);
float fmSample = osc.out();
```

### CV Modulation Processing

```cpp
// In ReGrandy::process()
float freqCV = inputs[FREQ_INPUT].getVoltage();
float freqAttenuator = params[FREQCV_PARAM].getValue();

// Normalize CV to ±1.0 and apply attenuator
float freqMod = (freqCV / 5.0f) * freqAttenuator;

// Apply to base frequency parameter
float finalFreq = params[FREQ_PARAM].getValue() + freqMod;

// Convert to Hz with exponential scaling
osc.freq = clamp(261.626f * powf(2.0f, finalFreq), 1.0f, 3000.0f);
```

---

## Thread Safety

**Warning:** The module is **not thread-safe**. All methods should be called from the same thread (VCV Rack's audio thread).

---

## Performance Considerations

- **Breakpoint Count**: Higher `num_bpts` increases CPU usage linearly
- **Grain Rate**: Very high `g_rate` (>2000 Hz) can increase CPU load
- **FM Mode**: FM synthesis adds ~20% CPU overhead
- **Envelope Switching**: Negligible performance impact

**Optimization Tips:**
- Keep `num_bpts` below 30 for real-time performance
- Use LINEAR distribution for lowest CPU usage
- Disable FM mode when not needed

---

## Version History

See [CHANGELOG.md](CHANGELOG.md) for version-specific API changes.

---

## See Also

- [Quickstart Guide](QUICKSTART.md)
- [Advanced Usage](ADVANCED.md)
- [Contributing Guidelines](CONTRIBUTING.md)
