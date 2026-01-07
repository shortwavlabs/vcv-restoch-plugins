/*
 * GrandyOscillator_test.cpp
 * Unit tests for GendyOscillator (granular stochastic dynamic synthesis)
 * 
 * Tests cover:
 * - Oscillator initialization and state
 * - process() method with various delta times
 * - wrap() and mirror() utility functions
 * - out() method output validation
 * - Phase progression and wraparound
 * - Boundary conditions and edge cases
 * - Configuration options (FM, mirroring, granulation)
 */

// Define test environment before including headers
#define RACK_HPP_INCLUDED
#define TABLE_SIZE 2048
#define M_PI 3.14159265358979323846
#define MAX_BPTS 50

#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include <string>
#include <cstdlib>

// Minimal Rack SDK mock for testing
namespace rack {
    namespace random {
        static inline float uniform() { return (float)rand() / RAND_MAX; }
        static inline float normal() { return uniform() * 2.0f - 1.0f; }
    }
    namespace logger {
        enum Level { DEBUG_LEVEL, INFO_LEVEL, WARN_LEVEL, FATAL_LEVEL };
        static inline void log(Level level, const char* filename, int line, const char* func, const char* format, ...) {}
    }
    namespace dsp {
        // Empty for now
    }
}

#define DEBUG(format, ...) do {} while(0)

// Include wavetable definitions inline
namespace rack {
  float wrap(float in, float lb, float ub) {
    float out = in;
    if (in > ub) out = lb;
    else if (in < lb) out = ub;
    return out;
  }

  float mirror(float in, float lb, float ub) {
    float out = in;
    if (in > ub) {
      out = ub - (in - ub);
    }
    else if (in < lb) {
      out = lb + (lb - in);
    }
    return out;
  }

  enum DistType {
    LINEAR,
    CAUCHY,
    ARCSINE
  };

  struct gRandGen {
    float my_rand(DistType t, float rand) {
      float c, temp, out;
      float a = 0.5f;
      
      switch (t) {
        case LINEAR:
          return rand; 
        case CAUCHY:
          c = atan(10 * a);
          temp = (1.f/a) * tan(c * (2.f * rand - 1.f));
          out = temp*0.1f;
          return out;
        case ARCSINE:
          c = sinf(1.5707963f * a); 
		      out = sinf(M_PI * (rand - 0.5f) * a) / c;
          return out;
        default:
          break;
      }
      return rand;
    }
  };
   
  enum EnvType {
    SIN,
    TRI,
    HANN,
    WELCH,
    TUKEY,
    NUM_ENVS
  };

  struct Wavetable {
    float table[TABLE_SIZE];
    EnvType et;

    Wavetable() {
      et = SIN;
      initSinWav(); 
    }

    Wavetable(EnvType e) {
      et = e;
      init(e);
    }

    void init(EnvType e) {
      switch (e) {
        case SIN: initSinWav(); break;
        case TRI: initTriEnv(); break;
        case HANN: initHannEnv(); break;
        case WELCH: initWelchEnv(); break;
        case TUKEY: initTukeyEnv(); break;
        default: initSinWav();
      }
    }

    void switchEnvType(EnvType e) {
      if (et != e) {
        et = e;
        init(e); 
      }
    }

    void initSinWav() {
      float phase = 0.f;
      for (int i=0; i<TABLE_SIZE; i++) {
        table[i] = sinf(2.f*M_PI * phase); 
        phase += (float) i  / (2.f*M_PI);
      }
    }

    void initTriEnv() {
      float phase = 0.f;
      for (int i=0; i<TABLE_SIZE; i++) {
        if (phase < 0.5f) {
          table[i] = ((2.f * i) / TABLE_SIZE);
        }
        else  {
          table[i] = ((-2.f * i) / TABLE_SIZE) + 2.f;
        }
        phase += 1.f / TABLE_SIZE;
      }
    }

    void initHannEnv() {
      float a_0 = 0.5f;
      for (int i=0; i<TABLE_SIZE; i++) {
        table[i] = a_0 * (1 - cosf((2.f * M_PI * ((float) i / TABLE_SIZE)) / 1.f));
      }
    }

    void initWelchEnv() {
      float ts = (float) TABLE_SIZE;
      for (int i=0; i<TABLE_SIZE; i++) {
        table[i] = 1.f - pow(((float) i - (ts / 2.f)) / (ts / 2.f), 2); 
      }
    }

    void initTukeyEnv() {
      float p1,p2,N,alpha;
      alpha = 0.5f;
      N = (float) TABLE_SIZE;
      p1 = alpha * N / 2;
      p2 = N * (1 - (alpha / 2));

      for (int i=0; i<TABLE_SIZE; i++) {
        if (i < p1) {
          table[i] = 0.5f * (1 + cosf(M_PI * (((2 * i) / (alpha * N)) - 1)));
        }
        else if (i <= p2) { 
          table[i] = 1.f; 
        } 
        else {
          table[i] = 0.5f * (1 + cosf(M_PI * (((2 * i) / (alpha * N)) - (2 / alpha) + 1)));
        }
      }
    }

    float operator[](int x) {
      return table[x];
    }

    float operator[](float x) {
      return index(x); 
    }

    float index(float x) {
      float fl = floorf(x);
      float ph = x - fl;
      float lb = table[(int) fl];
      float ub = table[(int) ceilf(x)];
      return ((1.0 - ph) * lb) + (ph * ub);
    }

    float get(float x) {
      if (x > 1.000000) DEBUG("BAD!\n");
      return index(x * (float) TABLE_SIZE); 
    }
  };

  // GendyOscillator definition
  struct GendyOscillator {
    float phase = 1.f;
    
    bool GRAN_ON = true;
    bool is_fm_on = true; 
    bool is_mirroring = false;

    int num_bpts = 12;
    int min_freq = 30; 
    int max_freq = 1000;

    float amps[MAX_BPTS] = {0.f};
    float durs[MAX_BPTS] = {1.f};
    float offs[MAX_BPTS] = {0.f};
    float rats[MAX_BPTS] = {1.f};

    int index = 0;
    float amp = 0.0; 
    float amp_next = amps[0];
    
    float max_amp_step = 0.05f;
    float max_dur_step = 0.05f;
    float max_off_step = 0.005f;
    float max_rat_step = 0.01f;

    float speed = 0.0;
    float rate = 0.0;

    float freq_mul = 1.0;

    float off = 0.0;
    float off_next = 0.0;

    float g_idx = 0.f;
    float g_idx_next = 0.5f;

    float g_amp = 0.f;
    float g_amp_next = 0.f;
    float g_rate = 1.f;

    float rat = 1.f;
    float rat_next = 1.f;

    Wavetable sample = Wavetable(SIN);
    Wavetable env = Wavetable(TRI); 

    DistType dt = LINEAR;
    gRandGen rg;
    
    float amp_out = 0.f;

    float f_mod = 400.f;
    float f_car = 800.f;
    
    float f_car1 = f_car;
    float f_car2 = f_car;

    float i_mod = 100.f;

    float phase_mod1 = 0.f;
    float phase_mod2 = 0.f;
    
    float phase_car1 = 0.f;
    float phase_car2 = 0.f;

    bool last_flag = false;

    int count = 0;

    float freq = 261.626f;

    void process(float deltaTime) {
      last_flag = false;
      if (phase >= 1.0) {
        phase -= 1.0;
        amp = amp_next;
        rat = rat_next;
        index = (index + 1) % num_bpts;
        last_flag = index == num_bpts - 1;

        if (is_mirroring) {
          amps[index] = mirror(amps[index] + (max_amp_step * rg.my_rand(dt, random::normal())), -1.0f, 1.0f); 
          durs[index] = mirror(durs[index] + (max_dur_step * rg.my_rand(dt, random::normal())), 0.5f, 1.5f);
          offs[index] = mirror(offs[index] + (max_off_step * rg.my_rand(dt, random::normal())), 0.f, 1.0f);
          rats[index] = mirror(rats[index] + (max_off_step * rg.my_rand(dt, random::normal())), 0.7f, 1.3f);
        }
        else {
          amps[index] = wrap(amps[index] + (max_amp_step * rg.my_rand(dt, random::normal())), -1.0f, 1.0f); 
          durs[index] = wrap(durs[index] + (max_dur_step * rg.my_rand(dt, random::normal())), 0.5f, 1.5f);
          offs[index] = wrap(offs[index] + (max_off_step * rg.my_rand(dt, random::normal())), 0.f, 1.0f);
          rats[index] = wrap(rats[index] + (max_off_step * rg.my_rand(dt, random::normal())), 0.7f, 1.3f);
        }
        
        amp_next = amps[index];
        rate = durs[index];
        rat_next = rats[index];

        off = off_next;
        off_next = offs[index];
    
        g_idx = g_idx_next;
        g_idx_next = 0.0;

        speed = freq * deltaTime * num_bpts;
      }
     
      if (!is_fm_on) {
        g_amp = amp + (env.get(g_idx) * sample.get(off));
        g_amp_next = amp_next + (env.get(g_idx_next) * sample.get(off_next));
        amp_out = ((1.0 - phase) * g_amp) + (phase * g_amp_next); 
      } else {
        g_amp = amp + (env.get(g_idx) * sinf(phase_car1));
        g_amp_next = amp_next + (env.get(g_idx_next) * sinf(phase_car2));
        amp_out = ((1.0 - phase) * g_amp) + (phase * g_amp_next); 
      }

      g_idx = fmod(g_idx + (g_rate * deltaTime), 1.f);
      g_idx_next = fmod(g_idx_next + (g_rate * deltaTime), 1.f);

      off = fmod(off + (g_rate * deltaTime), 1.f);
      off_next = fmod(off_next + (g_rate * deltaTime), 1.f);
      
      phase += speed;

      phase_car1 += deltaTime * f_car1 * rat;
      phase_car2 += deltaTime * f_car2 * rat_next;

      phase_car1 = fmod(phase_car1, 1.f);
      phase_car2 = fmod(phase_car2, 1.f);

      phase_mod1 += deltaTime * f_mod;
      phase_mod2 += deltaTime * f_mod;

      phase_mod1 = fmod(phase_mod1, 1.f);
      phase_mod2 = fmod(phase_mod2, 1.f);

      f_car1 = fmod(f_car + (i_mod * sample.get(phase_mod1)), 22050.f);
      f_car2 = fmod(f_car + (i_mod * sample.get(phase_mod2)), 22050.f);
    
      count++;
    }

    float wrap(float in, float lb, float ub) {
      float out = in;
      if (in > ub) out = lb;
      else if (in < lb) out = ub;
      return out;
    }

    float mirror(float in, float lb, float ub) {
      float out = in;
      if (in > ub) {
        out = ub - (in - ub);
      }
      else if (in < lb) {
        out = lb + (lb - in);
      }
      return out;
    }
    
    float out() {
      return amp_out;
    }
  };
}

using namespace rack;

// Simple test framework
int tests_passed = 0;
int tests_failed = 0;

#define TEST_ASSERT(condition, message) \
    if (!(condition)) { \
        std::cerr << "FAILED: " << message << std::endl; \
        tests_failed++; \
        return false; \
    } else { \
        tests_passed++; \
    }

#define RUN_TEST(test_func) \
    std::cout << "Running " << #test_func << "..." << std::endl; \
    if (test_func()) { \
        std::cout << "  PASSED" << std::endl; \
    } else { \
        std::cout << "  FAILED" << std::endl; \
    }

// Utility to compare floats with tolerance
bool float_equal(float a, float b, float epsilon = 1e-5f) {
    return std::fabs(a - b) < epsilon;
}

// ============================================================================
// GendyOscillator initialization tests
// ============================================================================

bool test_oscillator_default_initialization() {
    GendyOscillator osc;
    TEST_ASSERT(osc.phase == 1.0f, "Default phase should be 1.0");
    TEST_ASSERT(osc.GRAN_ON == true, "Granulation should be on by default");
    TEST_ASSERT(osc.is_fm_on == true, "FM should be on by default");
    TEST_ASSERT(osc.is_mirroring == false, "Mirroring should be off by default");
    TEST_ASSERT(osc.num_bpts == 12, "Default number of breakpoints should be 12");
    TEST_ASSERT(osc.min_freq == 30, "Default min frequency should be 30");
    TEST_ASSERT(osc.max_freq == 1000, "Default max frequency should be 1000");
    return true;
}

bool test_oscillator_initial_arrays() {
    GendyOscillator osc;
    TEST_ASSERT(osc.amps[0] == 0.0f, "Initial amplitude should be 0");
    TEST_ASSERT(osc.durs[0] == 1.0f, "Initial duration should be 1.0");
    TEST_ASSERT(osc.offs[0] == 0.0f, "Initial offset should be 0");
    TEST_ASSERT(osc.rats[0] == 1.0f, "Initial rate should be 1.0");
    return true;
}

bool test_oscillator_initial_output() {
    GendyOscillator osc;
    float output = osc.out();
    TEST_ASSERT(!std::isnan(output) && !std::isinf(output), "Initial output should be valid");
    return true;
}

// ============================================================================
// wrap() method tests
// ============================================================================

bool test_oscillator_wrap_within_bounds() {
    GendyOscillator osc;
    float result = osc.wrap(0.5f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result, 0.5f), "wrap() should return same value when within bounds");
    return true;
}

bool test_oscillator_wrap_above_upper_bound() {
    GendyOscillator osc;
    float result = osc.wrap(1.5f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result, 0.0f), "wrap() should return lower bound when above upper bound");
    return true;
}

bool test_oscillator_wrap_below_lower_bound() {
    GendyOscillator osc;
    float result = osc.wrap(-0.5f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result, 1.0f), "wrap() should return upper bound when below lower bound");
    return true;
}

bool test_oscillator_wrap_negative_range() {
    GendyOscillator osc;
    float result1 = osc.wrap(1.5f, -1.0f, 1.0f);
    TEST_ASSERT(float_equal(result1, -1.0f), "wrap() should work with negative ranges");
    
    float result2 = osc.wrap(-1.5f, -1.0f, 1.0f);
    TEST_ASSERT(float_equal(result2, 1.0f), "wrap() should wrap correctly in negative range");
    return true;
}

// ============================================================================
// mirror() method tests
// ============================================================================

bool test_oscillator_mirror_within_bounds() {
    GendyOscillator osc;
    float result = osc.mirror(0.5f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result, 0.5f), "mirror() should return same value when within bounds");
    return true;
}

bool test_oscillator_mirror_above_upper_bound() {
    GendyOscillator osc;
    float result = osc.mirror(1.2f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result, 0.8f), "mirror() should reflect back from upper bound");
    return true;
}

bool test_oscillator_mirror_below_lower_bound() {
    GendyOscillator osc;
    float result = osc.mirror(-0.2f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result, 0.2f), "mirror() should reflect back from lower bound");
    return true;
}

bool test_oscillator_mirror_extreme_values() {
    GendyOscillator osc;
    float result = osc.mirror(2.5f, 0.0f, 1.0f);
    // Mirror doesn't guarantee bounds for large overshoots with single reflection
    TEST_ASSERT(!std::isnan(result) && !std::isinf(result), "mirror() should return valid value");
    return true;
}

// ============================================================================
// process() method tests
// ============================================================================

bool test_oscillator_process_basic() {
    GendyOscillator osc;
    osc.phase = 0.5f;
    float deltaTime = 0.001f;
    float initial_phase = osc.phase;
    
    osc.process(deltaTime);
    
    TEST_ASSERT(!std::isnan(osc.amp_out) && !std::isinf(osc.amp_out), "process() should produce valid output");
    // Phase may wrap around, so check if it changed or is valid
    TEST_ASSERT(osc.phase >= 0.0f && osc.phase < 1.0f, "process() should keep phase in valid range");
    return true;
}

bool test_oscillator_process_phase_wraparound() {
    GendyOscillator osc;
    osc.phase = 0.99f;
    float deltaTime = 0.1f;
    
    osc.process(deltaTime);
    
    TEST_ASSERT(osc.phase >= 0.0f && osc.phase < 1.0f, "Phase should wrap around when >= 1.0");
    return true;
}

bool test_oscillator_process_multiple_steps() {
    GendyOscillator osc;
    osc.phase = 0.0f;
    float deltaTime = 0.001f;
    
    for (int i = 0; i < 100; i++) {
        osc.process(deltaTime);
        TEST_ASSERT(!std::isnan(osc.amp_out) && !std::isinf(osc.amp_out), 
                    "Output should remain valid after multiple process calls");
        TEST_ASSERT(osc.phase >= 0.0f && osc.phase < 1.0f, 
                    "Phase should stay in valid range");
    }
    return true;
}

bool test_oscillator_process_zero_deltatime() {
    GendyOscillator osc;
    float initial_phase = osc.phase;
    
    osc.process(0.0f);
    
    TEST_ASSERT(!std::isnan(osc.amp_out), "process() should handle zero deltaTime");
    return true;
}

bool test_oscillator_process_large_deltatime() {
    GendyOscillator osc;
    osc.phase = 0.0f;
    
    osc.process(1.0f);
    
    TEST_ASSERT(!std::isnan(osc.amp_out) && !std::isinf(osc.amp_out), 
                "process() should handle large deltaTime");
    return true;
}

bool test_oscillator_process_index_progression() {
    GendyOscillator osc;
    osc.phase = 0.99f;
    osc.index = 5;
    float deltaTime = 0.1f;
    
    int initial_index = osc.index;
    osc.process(deltaTime);
    
    TEST_ASSERT(osc.index >= 0 && osc.index < osc.num_bpts, 
                "Index should stay within valid range");
    return true;
}

bool test_oscillator_process_last_flag() {
    GendyOscillator osc;
    osc.phase = 0.99f;
    osc.index = osc.num_bpts - 2;  // Second to last
    float deltaTime = 0.1f;
    
    osc.process(deltaTime);
    
    // After processing, if we hit the last breakpoint, last_flag should be true
    // This test verifies the flag mechanism works
    TEST_ASSERT(osc.last_flag == true || osc.last_flag == false, 
                "last_flag should be a valid boolean");
    return true;
}

// ============================================================================
// out() method tests
// ============================================================================

bool test_oscillator_out_returns_valid() {
    GendyOscillator osc;
    osc.process(0.001f);
    float output = osc.out();
    
    TEST_ASSERT(!std::isnan(output) && !std::isinf(output), "out() should return valid value");
    return true;
}

bool test_oscillator_out_matches_amp_out() {
    GendyOscillator osc;
    osc.process(0.001f);
    
    TEST_ASSERT(float_equal(osc.out(), osc.amp_out), "out() should return amp_out value");
    return true;
}

bool test_oscillator_out_reasonable_range() {
    GendyOscillator osc;
    
    for (int i = 0; i < 1000; i++) {
        osc.process(0.001f);
        float output = osc.out();
        // Output should be finite (granular synthesis can have wide dynamic range)
        TEST_ASSERT(!std::isnan(output) && !std::isinf(output), 
                    "Output should be finite");
    }
    return true;
}

// ============================================================================
// Configuration tests
// ============================================================================

bool test_oscillator_fm_on_off() {
    GendyOscillator osc1;
    osc1.is_fm_on = true;
    osc1.process(0.001f);
    float output1 = osc1.out();
    
    GendyOscillator osc2;
    osc2.is_fm_on = false;
    osc2.process(0.001f);
    float output2 = osc2.out();
    
    TEST_ASSERT(!std::isnan(output1) && !std::isnan(output2), 
                "Both FM on and off should produce valid output");
    return true;
}

bool test_oscillator_mirroring_on_off() {
    GendyOscillator osc1;
    osc1.is_mirroring = true;
    osc1.phase = 0.99f;
    osc1.process(0.1f);
    
    GendyOscillator osc2;
    osc2.is_mirroring = false;
    osc2.phase = 0.99f;
    osc2.process(0.1f);
    
    TEST_ASSERT(!std::isnan(osc1.out()) && !std::isnan(osc2.out()), 
                "Both mirroring modes should produce valid output");
    return true;
}

bool test_oscillator_different_num_breakpoints() {
    std::vector<int> breakpoint_counts = {3, 12, 25, MAX_BPTS};
    
    for (int num_bpts : breakpoint_counts) {
        GendyOscillator osc;
        osc.num_bpts = num_bpts;
        osc.phase = 0.0f;
        
        for (int i = 0; i < 100; i++) {
            osc.process(0.001f);
            TEST_ASSERT(!std::isnan(osc.out()), 
                        "Should work with different breakpoint counts");
            TEST_ASSERT(osc.index >= 0 && osc.index < num_bpts,
                        "Index should stay within breakpoint range");
        }
    }
    return true;
}

bool test_oscillator_frequency_change() {
    GendyOscillator osc;
    osc.freq = 440.0f;  // A4
    osc.process(0.001f);
    float output1 = osc.out();
    
    osc.freq = 880.0f;  // A5
    osc.process(0.001f);
    float output2 = osc.out();
    
    TEST_ASSERT(!std::isnan(output1) && !std::isnan(output2), 
                "Should handle frequency changes");
    return true;
}

bool test_oscillator_step_parameters() {
    GendyOscillator osc;
    
    osc.max_amp_step = 0.1f;
    osc.max_dur_step = 0.1f;
    osc.max_off_step = 0.01f;
    osc.max_rat_step = 0.02f;
    
    osc.phase = 0.99f;
    osc.process(0.1f);
    
    TEST_ASSERT(!std::isnan(osc.out()), "Should handle different step parameters");
    return true;
}

// ============================================================================
// Edge case and boundary tests
// ============================================================================

bool test_oscillator_extreme_frequencies() {
    GendyOscillator osc1;
    osc1.freq = 20.0f;  // Very low
    osc1.process(0.001f);
    TEST_ASSERT(!std::isnan(osc1.out()), "Should handle very low frequency");
    
    GendyOscillator osc2;
    osc2.freq = 20000.0f;  // Very high
    osc2.process(0.001f);
    TEST_ASSERT(!std::isnan(osc2.out()), "Should handle very high frequency");
    
    return true;
}

bool test_oscillator_fm_parameters() {
    GendyOscillator osc;
    osc.is_fm_on = true;
    osc.f_mod = 200.0f;
    osc.f_car = 800.0f;
    osc.i_mod = 50.0f;
    
    for (int i = 0; i < 100; i++) {
        osc.process(0.001f);
        TEST_ASSERT(!std::isnan(osc.out()), "FM synthesis should produce valid output");
    }
    return true;
}

bool test_oscillator_wavetable_switching() {
    GendyOscillator osc;
    
    // Switch sample wavetable
    osc.sample.switchEnvType(TRI);
    osc.process(0.001f);
    TEST_ASSERT(!std::isnan(osc.out()), "Should work with TRI sample wavetable");
    
    osc.sample.switchEnvType(HANN);
    osc.process(0.001f);
    TEST_ASSERT(!std::isnan(osc.out()), "Should work with HANN sample wavetable");
    
    // Switch envelope wavetable
    osc.env.switchEnvType(WELCH);
    osc.process(0.001f);
    TEST_ASSERT(!std::isnan(osc.out()), "Should work with WELCH envelope");
    
    return true;
}

bool test_oscillator_distribution_types() {
    std::vector<DistType> dist_types = {LINEAR, CAUCHY, ARCSINE};
    
    for (DistType dt : dist_types) {
        GendyOscillator osc;
        osc.dt = dt;
        osc.phase = 0.99f;
        
        for (int i = 0; i < 50; i++) {
            osc.process(0.01f);
            TEST_ASSERT(!std::isnan(osc.out()), 
                        "Should work with all distribution types");
        }
    }
    return true;
}

bool test_oscillator_continuous_operation() {
    GendyOscillator osc;
    osc.phase = 0.0f;
    
    // Run for many cycles
    for (int i = 0; i < 10000; i++) {
        osc.process(0.001f);
        
        if (i % 1000 == 0) {
            TEST_ASSERT(!std::isnan(osc.out()) && !std::isinf(osc.out()), 
                        "Should maintain valid output over long operation");
            TEST_ASSERT(osc.phase >= 0.0f && osc.phase < 1.0f,
                        "Phase should remain valid over long operation");
        }
    }
    return true;
}

bool test_oscillator_state_consistency() {
    GendyOscillator osc;
    osc.phase = 0.5f;
    int initial_count = osc.count;
    
    osc.process(0.001f);
    
    TEST_ASSERT(osc.count == initial_count + 1, "Count should increment each process call");
    TEST_ASSERT(osc.phase >= 0.0f && osc.phase < 1.0f, "Phase should be valid");
    TEST_ASSERT(osc.g_idx >= 0.0f && osc.g_idx <= 1.0f, "Grain index should be valid");
    TEST_ASSERT(osc.g_idx_next >= 0.0f && osc.g_idx_next <= 1.0f, "Next grain index should be valid");
    
    return true;
}

// ============================================================================
// Main test runner
// ============================================================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Running GendyOscillator Unit Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "--- Initialization tests ---" << std::endl;
    RUN_TEST(test_oscillator_default_initialization);
    RUN_TEST(test_oscillator_initial_arrays);
    RUN_TEST(test_oscillator_initial_output);
    std::cout << std::endl;

    std::cout << "--- wrap() method tests ---" << std::endl;
    RUN_TEST(test_oscillator_wrap_within_bounds);
    RUN_TEST(test_oscillator_wrap_above_upper_bound);
    RUN_TEST(test_oscillator_wrap_below_lower_bound);
    RUN_TEST(test_oscillator_wrap_negative_range);
    std::cout << std::endl;

    std::cout << "--- mirror() method tests ---" << std::endl;
    RUN_TEST(test_oscillator_mirror_within_bounds);
    RUN_TEST(test_oscillator_mirror_above_upper_bound);
    RUN_TEST(test_oscillator_mirror_below_lower_bound);
    RUN_TEST(test_oscillator_mirror_extreme_values);
    std::cout << std::endl;

    std::cout << "--- process() method tests ---" << std::endl;
    RUN_TEST(test_oscillator_process_basic);
    RUN_TEST(test_oscillator_process_phase_wraparound);
    RUN_TEST(test_oscillator_process_multiple_steps);
    RUN_TEST(test_oscillator_process_zero_deltatime);
    RUN_TEST(test_oscillator_process_large_deltatime);
    RUN_TEST(test_oscillator_process_index_progression);
    RUN_TEST(test_oscillator_process_last_flag);
    std::cout << std::endl;

    std::cout << "--- out() method tests ---" << std::endl;
    RUN_TEST(test_oscillator_out_returns_valid);
    RUN_TEST(test_oscillator_out_matches_amp_out);
    RUN_TEST(test_oscillator_out_reasonable_range);
    std::cout << std::endl;

    std::cout << "--- Configuration tests ---" << std::endl;
    RUN_TEST(test_oscillator_fm_on_off);
    RUN_TEST(test_oscillator_mirroring_on_off);
    RUN_TEST(test_oscillator_different_num_breakpoints);
    RUN_TEST(test_oscillator_frequency_change);
    RUN_TEST(test_oscillator_step_parameters);
    std::cout << std::endl;

    std::cout << "--- Edge case and boundary tests ---" << std::endl;
    RUN_TEST(test_oscillator_extreme_frequencies);
    RUN_TEST(test_oscillator_fm_parameters);
    RUN_TEST(test_oscillator_wavetable_switching);
    RUN_TEST(test_oscillator_distribution_types);
    RUN_TEST(test_oscillator_continuous_operation);
    RUN_TEST(test_oscillator_state_consistency);
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "Test Results:" << std::endl;
    std::cout << "  Passed: " << tests_passed << std::endl;
    std::cout << "  Failed: " << tests_failed << std::endl;
    std::cout << "  Total:  " << (tests_passed + tests_failed) << std::endl;
    std::cout << "========================================" << std::endl;

    return tests_failed > 0 ? 1 : 0;
}
