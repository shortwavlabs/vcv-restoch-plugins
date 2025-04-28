#include "ReGrandy.hpp"

void ReGrandy::process(const ProcessArgs &args)
{
  float deltaTime = args.sampleTime;

  // snap knob for selecting envelope for the grain
  int env_num = (int)clamp(roundf(params[ENVS_PARAM].getValue()), 1.0f, 4.0f);

  if (env != (EnvType)env_num)
  {
    DEBUG("Switching to env type: %d", env_num);
    env = (EnvType)env_num;
    go.env.switchEnvType(env);
  }

  // handle mirror / fold switch
  go.is_mirroring = (int)params[MIRR_PARAM].getValue();

  // accept modulation of signal inputs for each parameter
  freq_sig = (inputs[FREQ_INPUT].getVoltage() / 5.f) * params[FREQCV_PARAM].getValue();

  bpts_sig = 5.f * dsp::quadraticBipolar((inputs[BPTS_INPUT].getVoltage() / 5.f) * params[BPTSCV_PARAM].getValue());
  astp_sig = dsp::quadraticBipolar((inputs[ASTP_INPUT].getVoltage() / 5.f) * params[ASTPCV_PARAM].getValue());
  dstp_sig = dsp::quadraticBipolar((inputs[DSTP_INPUT].getVoltage() / 5.f) * params[DSTPCV_PARAM].getValue());
  grat_sig = (inputs[GRAT_INPUT].getVoltage() / 5.f) * params[GRATCV_PARAM].getValue();

  // fm control sigs
  fmod_sig = (inputs[FMOD_INPUT].getVoltage() / 5.f) * params[FMODCV_PARAM].getValue();
  imod_sig = dsp::quadraticBipolar((inputs[IMOD_INPUT].getVoltage() / 5.f) * params[IMODCV_PARAM].getValue());

  int new_nbpts = clamp((int)params[BPTS_PARAM].getValue() + (int)bpts_sig, 2, MAX_BPTS);
  if (new_nbpts != go.num_bpts)
    go.num_bpts = new_nbpts;

  // better frequency control
  freq_sig += params[FREQ_PARAM].getValue();
  grat_sig += params[GRAT_PARAM].getValue();

  go.freq = clamp(dsp::FREQ_C4 * powf(2.0f, freq_sig), 1.f, 3000.f);

  go.max_amp_step = rescale(params[ASTP_PARAM].getValue() + (astp_sig / 4.f), 0.0, 1.0, 0.05, 0.3);
  go.max_dur_step = rescale(params[DSTP_PARAM].getValue() + (dstp_sig / 4.f), 0.0, 1.0, 0.01, 0.3);
  go.freq_mul = rescale(params[FREQ_PARAM].getValue(), -1.0, 1.0, 0.05, 4.0);
  go.g_rate = clamp(dsp::FREQ_C4 * powf(2.0f, grat_sig), 1e-6, 3000.f);

  go.dt = (DistType)params[PDST_PARAM].getValue();

  // set fm params
  go.is_fm_on = !(params[FMTR_PARAM].getValue() > 0.0f);

  fmod_sig += params[FMOD_PARAM].getValue();
  imod_sig += params[IMOD_PARAM].getValue();

  go.f_car = clamp(dsp::FREQ_C4 * powf(2.0f, params[FCAR_PARAM].getValue()), 1.f, 5000.f);
  go.f_mod = clamp(dsp::FREQ_C4 * powf(2.0f, fmod_sig), 1.f, 5000.f);

  go.i_mod = rescale(params[IMOD_PARAM].getValue(), 0.f, 1.f, 10.f, 3000.f);

  go.process(deltaTime);

  outputs[SINE_OUTPUT].setVoltage(5.0f * go.out());
}

Model *modelReGrandy = createModel<ReGrandy, ReGrandyWidget>("ReGrandy");
