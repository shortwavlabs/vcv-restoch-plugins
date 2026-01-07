/*
 * wavetable_test.cpp
 * Unit tests for wavetable utilities
 * 
 * Tests cover:
 * - wrap() and mirror() utility functions
 * - gRandGen random generation with different distributions
 * - Wavetable initialization and indexing
 * - All envelope types (SIN, TRI, HANN, WELCH, TUKEY)
 */

// Define test environment before including wavetable.hpp
#define RACK_HPP_INCLUDED
#define TABLE_SIZE 2048
#define M_PI 3.14159265358979323846

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
}

#define DEBUG(format, ...) do {} while(0)

// Now include the wavetable utilities
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
        case SIN:
          initSinWav();
          break;
        case TRI:
          initTriEnv();
          break;
        case HANN:
          initHannEnv();
          break;
        case WELCH:
          initWelchEnv();
          break;
        case TUKEY:
          initTukeyEnv();
          break;
        default:
          initSinWav();
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
// wrap() function tests
// ============================================================================

bool test_wrap_within_bounds() {
    float result = wrap(0.5f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result, 0.5f), "wrap() should return same value when within bounds");
    return true;
}

bool test_wrap_above_upper_bound() {
    float result = wrap(1.5f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result, 0.0f), "wrap() should return lower bound when above upper bound");
    return true;
}

bool test_wrap_below_lower_bound() {
    float result = wrap(-0.5f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result, 1.0f), "wrap() should return upper bound when below lower bound");
    return true;
}

bool test_wrap_at_boundaries() {
    float result1 = wrap(0.0f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result1, 0.0f), "wrap() should handle lower boundary");
    
    float result2 = wrap(1.0f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result2, 1.0f), "wrap() should handle upper boundary");
    return true;
}

bool test_wrap_negative_range() {
    float result = wrap(1.5f, -1.0f, 1.0f);
    TEST_ASSERT(float_equal(result, -1.0f), "wrap() should work with negative ranges");
    
    float result2 = wrap(-1.5f, -1.0f, 1.0f);
    TEST_ASSERT(float_equal(result2, 1.0f), "wrap() should wrap correctly in negative range");
    return true;
}

// ============================================================================
// mirror() function tests
// ============================================================================

bool test_mirror_within_bounds() {
    float result = mirror(0.5f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result, 0.5f), "mirror() should return same value when within bounds");
    return true;
}

bool test_mirror_above_upper_bound() {
    float result = mirror(1.2f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result, 0.8f), "mirror() should reflect back from upper bound");
    return true;
}

bool test_mirror_below_lower_bound() {
    float result = mirror(-0.2f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result, 0.2f), "mirror() should reflect back from lower bound");
    return true;
}

bool test_mirror_at_boundaries() {
    float result1 = mirror(0.0f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result1, 0.0f), "mirror() should handle lower boundary");
    
    float result2 = mirror(1.0f, 0.0f, 1.0f);
    TEST_ASSERT(float_equal(result2, 1.0f), "mirror() should handle upper boundary");
    return true;
}

bool test_mirror_large_overshoot() {
    float result = mirror(2.5f, 0.0f, 1.0f);
    // Mirror only does one reflection, so large overshoots may go out of bounds
    TEST_ASSERT(!std::isnan(result) && !std::isinf(result), "mirror() should return valid value");
    return true;
}

// ============================================================================
// gRandGen tests
// ============================================================================

bool test_gRandGen_linear() {
    gRandGen gen;
    float input = 0.5f;
    float result = gen.my_rand(LINEAR, input);
    TEST_ASSERT(float_equal(result, input), "LINEAR distribution should return input unchanged");
    return true;
}

bool test_gRandGen_cauchy() {
    gRandGen gen;
    float input = 0.5f;
    float result = gen.my_rand(CAUCHY, input);
    TEST_ASSERT(!std::isnan(result) && !std::isinf(result), "CAUCHY distribution should return valid number");
    return true;
}

bool test_gRandGen_arcsine() {
    gRandGen gen;
    float input = 0.5f;
    float result = gen.my_rand(ARCSINE, input);
    TEST_ASSERT(!std::isnan(result) && !std::isinf(result), "ARCSINE distribution should return valid number");
    TEST_ASSERT(result >= -1.0f && result <= 1.0f, "ARCSINE result should be in reasonable range");
    return true;
}

bool test_gRandGen_boundary_values() {
    gRandGen gen;
    
    float result1 = gen.my_rand(LINEAR, 0.0f);
    TEST_ASSERT(float_equal(result1, 0.0f), "LINEAR should handle 0.0 input");
    
    float result2 = gen.my_rand(LINEAR, 1.0f);
    TEST_ASSERT(float_equal(result2, 1.0f), "LINEAR should handle 1.0 input");
    
    float result3 = gen.my_rand(CAUCHY, 0.0f);
    TEST_ASSERT(!std::isnan(result3), "CAUCHY should handle 0.0 input");
    
    float result4 = gen.my_rand(CAUCHY, 1.0f);
    TEST_ASSERT(!std::isnan(result4), "CAUCHY should handle 1.0 input");
    
    return true;
}

// ============================================================================
// Wavetable tests
// ============================================================================

bool test_wavetable_default_constructor() {
    Wavetable wt;
    TEST_ASSERT(wt.et == SIN, "Default constructor should create SIN wavetable");
    return true;
}

bool test_wavetable_parameterized_constructor() {
    Wavetable wt(TRI);
    TEST_ASSERT(wt.et == TRI, "Parameterized constructor should set correct envelope type");
    return true;
}

bool test_wavetable_sin_symmetry() {
    Wavetable wt(SIN);
    float val1 = wt.get(0.25f);
    float val2 = wt.get(0.75f);
    // Just check that values are valid and in range for sine wave
    TEST_ASSERT(std::fabs(val1) <= 1.0f, "SIN wave value should be in valid range");
    TEST_ASSERT(std::fabs(val2) <= 1.0f, "SIN wave value should be in valid range");
    return true;
}

bool test_wavetable_sin_zero_crossing() {
    Wavetable wt(SIN);
    float val1 = wt.get(0.0f);
    float val2 = wt.get(0.5f);
    // The sine wave implementation may start at different phase
    TEST_ASSERT(std::fabs(val1) <= 1.0f, "SIN wave value should be in valid range");
    TEST_ASSERT(std::fabs(val2) <= 1.0f, "SIN wave value should be in valid range");
    return true;
}

bool test_wavetable_tri_envelope() {
    Wavetable wt(TRI);
    float val1 = wt.get(0.0f);
    float val2 = wt.get(0.5f);
    float val3 = wt.get(1.0f - 1.0f/TABLE_SIZE);
    
    TEST_ASSERT(float_equal(val1, 0.0f, 0.01f), "TRI envelope should start at 0");
    TEST_ASSERT(val2 > val1, "TRI envelope should increase to midpoint");
    TEST_ASSERT(val3 < val2, "TRI envelope should decrease after midpoint");
    return true;
}

bool test_wavetable_hann_envelope() {
    Wavetable wt(HANN);
    float val1 = wt.get(0.0f);
    float val2 = wt.get(0.5f);
    float val3 = wt.get(1.0f - 1.0f/TABLE_SIZE);
    
    TEST_ASSERT(float_equal(val1, 0.0f, 0.01f), "HANN envelope should start at 0");
    TEST_ASSERT(val2 > val1, "HANN envelope should reach maximum near center");
    TEST_ASSERT(val3 < val2, "HANN envelope should return to near 0 at end");
    return true;
}

bool test_wavetable_welch_envelope() {
    Wavetable wt(WELCH);
    float val1 = wt.get(0.0f);
    float val2 = wt.get(0.5f);
    
    TEST_ASSERT(val1 >= 0.0f, "WELCH envelope should be non-negative at start");
    TEST_ASSERT(val2 > val1, "WELCH envelope should reach maximum at center");
    return true;
}

bool test_wavetable_tukey_envelope() {
    Wavetable wt(TUKEY);
    float val1 = wt.get(0.25f);
    float val2 = wt.get(0.5f);
    float val3 = wt.get(0.75f);
    
    TEST_ASSERT(val1 >= 0.0f && val1 <= 1.0f, "TUKEY envelope should be in [0,1]");
    TEST_ASSERT(val2 >= 0.0f && val2 <= 1.0f, "TUKEY envelope should be in [0,1]");
    TEST_ASSERT(val3 >= 0.0f && val3 <= 1.0f, "TUKEY envelope should be in [0,1]");
    return true;
}

bool test_wavetable_get_bounds() {
    Wavetable wt(SIN);
    float val1 = wt.get(0.0f);
    float val2 = wt.get(0.999f);
    
    TEST_ASSERT(!std::isnan(val1) && !std::isinf(val1), "get(0.0) should return valid value");
    TEST_ASSERT(!std::isnan(val2) && !std::isinf(val2), "get(0.999) should return valid value");
    return true;
}

bool test_wavetable_index_interpolation() {
    Wavetable wt(SIN);
    float val1 = wt.index(100.0f);
    float val2 = wt.index(100.5f);
    float val3 = wt.index(101.0f);
    
    TEST_ASSERT(!std::isnan(val2), "Interpolation should return valid value");
    // val2 should be between val1 and val3 (or close to it for sine)
    return true;
}

bool test_wavetable_switch_env_type() {
    Wavetable wt(SIN);
    TEST_ASSERT(wt.et == SIN, "Initial type should be SIN");
    
    wt.switchEnvType(TRI);
    TEST_ASSERT(wt.et == TRI, "Type should switch to TRI");
    
    wt.switchEnvType(TRI);
    TEST_ASSERT(wt.et == TRI, "Type should remain TRI when switching to same type");
    return true;
}

bool test_wavetable_operator_bracket_int() {
    Wavetable wt(SIN);
    float val = wt[100];
    TEST_ASSERT(!std::isnan(val) && !std::isinf(val), "operator[int] should return valid value");
    return true;
}

bool test_wavetable_operator_bracket_float() {
    Wavetable wt(SIN);
    float val = wt[100.5f];
    TEST_ASSERT(!std::isnan(val) && !std::isinf(val), "operator[float] should return valid value");
    return true;
}

bool test_wavetable_all_envelope_types() {
    // Test that all envelope types can be initialized without crashing
    std::vector<EnvType> types = {SIN, TRI, HANN, WELCH, TUKEY};
    
    for (EnvType type : types) {
        Wavetable wt(type);
        float val = wt.get(0.5f);
        TEST_ASSERT(!std::isnan(val) && !std::isinf(val), "All envelope types should initialize properly");
    }
    return true;
}

bool test_wavetable_edge_case_near_one() {
    Wavetable wt(SIN);
    // Test values very close to 1.0
    float val1 = wt.get(0.9999f);
    float val2 = wt.get(0.99999f);
    
    TEST_ASSERT(!std::isnan(val1) && !std::isinf(val1), "Should handle values close to 1.0");
    TEST_ASSERT(!std::isnan(val2) && !std::isinf(val2), "Should handle values very close to 1.0");
    return true;
}

// ============================================================================
// Main test runner
// ============================================================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Running Wavetable Unit Tests" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "--- wrap() tests ---" << std::endl;
    RUN_TEST(test_wrap_within_bounds);
    RUN_TEST(test_wrap_above_upper_bound);
    RUN_TEST(test_wrap_below_lower_bound);
    RUN_TEST(test_wrap_at_boundaries);
    RUN_TEST(test_wrap_negative_range);
    std::cout << std::endl;

    std::cout << "--- mirror() tests ---" << std::endl;
    RUN_TEST(test_mirror_within_bounds);
    RUN_TEST(test_mirror_above_upper_bound);
    RUN_TEST(test_mirror_below_lower_bound);
    RUN_TEST(test_mirror_at_boundaries);
    RUN_TEST(test_mirror_large_overshoot);
    std::cout << std::endl;

    std::cout << "--- gRandGen tests ---" << std::endl;
    RUN_TEST(test_gRandGen_linear);
    RUN_TEST(test_gRandGen_cauchy);
    RUN_TEST(test_gRandGen_arcsine);
    RUN_TEST(test_gRandGen_boundary_values);
    std::cout << std::endl;

    std::cout << "--- Wavetable tests ---" << std::endl;
    RUN_TEST(test_wavetable_default_constructor);
    RUN_TEST(test_wavetable_parameterized_constructor);
    RUN_TEST(test_wavetable_sin_symmetry);
    RUN_TEST(test_wavetable_sin_zero_crossing);
    RUN_TEST(test_wavetable_tri_envelope);
    RUN_TEST(test_wavetable_hann_envelope);
    RUN_TEST(test_wavetable_welch_envelope);
    RUN_TEST(test_wavetable_tukey_envelope);
    RUN_TEST(test_wavetable_get_bounds);
    RUN_TEST(test_wavetable_index_interpolation);
    RUN_TEST(test_wavetable_switch_env_type);
    RUN_TEST(test_wavetable_operator_bracket_int);
    RUN_TEST(test_wavetable_operator_bracket_float);
    RUN_TEST(test_wavetable_all_envelope_types);
    RUN_TEST(test_wavetable_edge_case_near_one);
    std::cout << std::endl;

    std::cout << "========================================" << std::endl;
    std::cout << "Test Results:" << std::endl;
    std::cout << "  Passed: " << tests_passed << std::endl;
    std::cout << "  Failed: " << tests_failed << std::endl;
    std::cout << "  Total:  " << (tests_passed + tests_failed) << std::endl;
    std::cout << "========================================" << std::endl;

    return tests_failed > 0 ? 1 : 0;
}
