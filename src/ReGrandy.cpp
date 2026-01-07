#include "ReGrandy.hpp"

// Constants for better readability
namespace
{
  constexpr float MIN_FREQ = 1.0f;
  constexpr float MAX_FREQ = 3000.0f;
  constexpr float MIN_BPTS = 2;
  constexpr float VOLTAGE_SCALE = 5.0f;
  constexpr float BIPOLAR_SCALE = 0.25f;
  constexpr float MIN_AMP_STEP = 0.05f;
  constexpr float MAX_AMP_STEP = 0.3f;
  constexpr float MIN_DUR_STEP = 0.01f;
  constexpr float MAX_DUR_STEP = 0.3f;
  constexpr float MIN_FREQ_MUL = 0.05f;
  constexpr float MAX_FREQ_MUL = 4.0f;
  constexpr float MIN_G_RATE = 1e-6f;
  constexpr float MAX_G_RATE = 3000.0f;
  constexpr float MAX_F_CAR = 5000.0f;
  constexpr float MIN_I_MOD = 10.0f;
  constexpr float MAX_I_MOD = 3000.0f;
}

void ReGrandy::updateEnvelopeType(const ProcessArgs &args)
{
  int env_num = static_cast<int>(clamp(roundf(params[ENVS_PARAM].getValue()), 1.0f, 4.0f));

  if (env != static_cast<EnvType>(env_num))
  {
    DEBUG("Switching to env type: %d", env_num);
    env = static_cast<EnvType>(env_num);
    go.env.switchEnvType(env);
  }
}

void ReGrandy::processModulationInputs()
{
  // Process modulation inputs with clear scaling
  freq_sig = (inputs[FREQ_INPUT].getVoltage() / VOLTAGE_SCALE) * params[FREQCV_PARAM].getValue();
  bpts_sig = VOLTAGE_SCALE * dsp::quadraticBipolar((inputs[BPTS_INPUT].getVoltage() / VOLTAGE_SCALE) * params[BPTSCV_PARAM].getValue());
  astp_sig = dsp::quadraticBipolar((inputs[ASTP_INPUT].getVoltage() / VOLTAGE_SCALE) * params[ASTPCV_PARAM].getValue());
  dstp_sig = dsp::quadraticBipolar((inputs[DSTP_INPUT].getVoltage() / VOLTAGE_SCALE) * params[DSTPCV_PARAM].getValue());
  grat_sig = (inputs[GRAT_INPUT].getVoltage() / VOLTAGE_SCALE) * params[GRATCV_PARAM].getValue();

  // FM control signals
  fmod_sig = (inputs[FMOD_INPUT].getVoltage() / VOLTAGE_SCALE) * params[FMODCV_PARAM].getValue();
  imod_sig = dsp::quadraticBipolar((inputs[IMOD_INPUT].getVoltage() / VOLTAGE_SCALE) * params[IMODCV_PARAM].getValue());
}

void ReGrandy::updateGranularParameters()
{
  // Update breakpoints if changed
  int new_nbpts = clamp(static_cast<int>(params[BPTS_PARAM].getValue() + static_cast<int>(bpts_sig)), static_cast<int>(MIN_BPTS), MAX_BPTS);
  if (new_nbpts != go.num_bpts)
  {
    go.num_bpts = new_nbpts;
  }

  // Add base parameter values to modulation
  freq_sig += params[FREQ_PARAM].getValue();
  grat_sig += params[GRAT_PARAM].getValue();

  // Set granular parameters with proper clamping
  go.freq = clamp(dsp::FREQ_C4 * powf(2.0f, freq_sig), MIN_FREQ, MAX_FREQ);
  go.max_amp_step = rescale(params[ASTP_PARAM].getValue() + (astp_sig / BIPOLAR_SCALE), 0.0, 1.0, MIN_AMP_STEP, MAX_AMP_STEP);
  go.max_dur_step = rescale(params[DSTP_PARAM].getValue() + (dstp_sig / BIPOLAR_SCALE), 0.0, 1.0, MIN_DUR_STEP, MAX_DUR_STEP);
  go.freq_mul = rescale(params[FREQ_PARAM].getValue(), -1.0, 1.0, MIN_FREQ_MUL, MAX_FREQ_MUL);
  go.g_rate = clamp(dsp::FREQ_C4 * powf(2.0f, grat_sig), MIN_G_RATE, MAX_G_RATE);
}

void ReGrandy::updateFMParameters()
{
  // Set FM state
  go.is_fm_on = !(params[FMTR_PARAM].getValue() > 0.0f);

  // Add base parameter values to modulation
  fmod_sig += params[FMOD_PARAM].getValue();
  imod_sig += params[IMOD_PARAM].getValue();

  // Set FM parameters with proper clamping
  go.f_car = clamp(dsp::FREQ_C4 * powf(2.0f, params[FCAR_PARAM].getValue()), MIN_FREQ, MAX_F_CAR);
  go.f_mod = clamp(dsp::FREQ_C4 * powf(2.0f, fmod_sig), MIN_FREQ, MAX_F_CAR);
  go.i_mod = rescale(params[IMOD_PARAM].getValue(), 0.f, 1.f, MIN_I_MOD, MAX_I_MOD);
}

void ReGrandy::process(const ProcessArgs &args)
{
  float deltaTime = args.sampleTime;

  // Update envelope type if changed
  updateEnvelopeType(args);

  // Handle mirror/fold switch
  go.is_mirroring = static_cast<int>(params[MIRR_PARAM].getValue());

  // Process all modulation inputs
  processModulationInputs();

  // Update granular synthesis parameters
  updateGranularParameters();

  // Set distribution type
  go.dt = static_cast<DistType>(params[PDST_PARAM].getValue());

  // Update FM synthesis parameters
  updateFMParameters();

  // Process audio
  go.process(deltaTime);

  // Get raw output
  float rawOutput = go.out();
  
  // Process through limiter for anti-clipping and speaker protection
  float limitedOutput = limiter.process(rawOutput);

  // Output the limited signal
  outputs[SINE_OUTPUT].setVoltage(VOLTAGE_SCALE * limitedOutput);
  outputs[INV_OUTPUT].setVoltage(-(VOLTAGE_SCALE * limitedOutput));
}

Model *modelReGrandy = createModel<ReGrandy, ReGrandyWidget>("ReGrandy");
