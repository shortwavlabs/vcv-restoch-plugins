#include "ReGenEcho.hpp"

void ReGenEcho::process(const ProcessArgs &args)
{
  // Implement a simple sine oscillator
  // float deltaTime = engineGetSampleTime();
  float amp_out = 0.0;

  // handle the 3 switches for accumlating and mirror toggle
  // and probability distrobution selection
  is_accumulating = (int)params[ACCM_PARAM].getValue();
  is_mirroring = (int)params[MIRR_PARAM].getValue();
  dt = (DistType)params[PDST_PARAM].getValue();

  // read in cv vals for astp, dstp and bpts
  bpts_sig = 5.f * dsp::quadraticBipolar((inputs[BPTS_INPUT].getVoltage() / 5.f) * params[BPTSCV_PARAM].getValue());
  astp_sig = dsp::quadraticBipolar((inputs[ASTP_INPUT].getVoltage() / 5.f) * params[ASTPCV_PARAM].getValue());
  dstp_sig = dsp::quadraticBipolar((inputs[DSTP_INPUT].getVoltage() / 5.f) * params[DSTPCV_PARAM].getValue());

  max_amp_step = rescale(params[ASTP_PARAM].getValue() + (astp_sig / 4.f), 0.0, 1.0, 0.05, 0.3);
  max_dur_step = rescale(params[DSTP_PARAM].getValue() + (dstp_sig / 4.f), 0.0, 1.0, 0.01, 0.3);

  sample_length = (int)(clamp(params[SLEN_PARAM].getValue(), 0.1, 1.f) * MAX_SAMPLE_SIZE);

  bpt_spc = (unsigned int)params[BPTS_PARAM].getValue() + 800;
  bpt_spc += (unsigned int)rescale(bpts_sig, -1.f, 1.f, 1.f, 200.f);
  num_bpts = sample_length / bpt_spc + 1;

  env_dur = bpt_spc / 2;

  // snap knob for selecting envelope for the grain
  int env_num = (int)clamp(roundf(params[ENVS_PARAM].getValue()), 1.0f, 4.0f);

  if (env.et != (EnvType)env_num)
  {
    env.switchEnvType((EnvType)env_num);
  }

  // handle sample reset
  if (smpTrigger.process(params[TRIG_PARAM].getValue()) || resetTrigger.process(inputs[RSET_INPUT].getVoltage() / 2.f))
  {
    for (unsigned int i = 0; i < MAX_SAMPLE_SIZE; i++)
      sample[i] = _sample[i];
    for (unsigned int i = 0; i < MAX_BPTS; i++)
    {
      mAmps[i] = 0.f;
      mDurs[i] = 1.f;
    }
  }

  // handle sample trigger through gate
  if (g2Trigger.process(inputs[GATE_INPUT].getVoltage() / 2.f))
  {

    // reset accumulated breakpoint vals
    for (unsigned int i = 0; i < MAX_BPTS; i++)
    {
      mAmps[i] = 0.f;
      mDurs[i] = 1.f;
    }

    num_bpts = sample_length / bpt_spc;
    sampling = true;
    idx = 0;
    s_i = 0;
  }

  if (sampling)
  {
    if (s_i >= MAX_SAMPLE_SIZE - 50)
    {
      float x, y, p;
      x = sample[s_i - 1];
      y = sample[0];
      p = 0.f;
      while (s_i < MAX_SAMPLE_SIZE)
      {
        sample[s_i] = (x * (1 - p)) + (y * p);
        p += 1.f / 50.f;
        s_i++;
      }
      DEBUG("Finished sampling");
      sampling = false;
    }
    else
    {
      sample[s_i] = inputs[WAV0_INPUT].getVoltage();
      _sample[s_i] = sample[s_i];
      s_i++;
    }
  }

  if (phase >= 1.0)
  {
    phase -= 1.0;

    amp = amp_next;
    index = (index + 1) % num_bpts;

    // adjust vals
    astp = max_amp_step * rg.my_rand(dt, random::normal());
    dstp = max_dur_step * rg.my_rand(dt, random::normal());

    if (is_mirroring)
    {
      mAmps[index] = mirror((is_accumulating ? mAmps[index] : 0.f) + astp, -1.0f, 1.0f);
      mDurs[index] = mirror(mDurs[index] + (dstp), 0.5, 1.5);
    }
    else
    {
      mAmps[index] = wrap((is_accumulating ? mAmps[index] : 0.f) + astp, -1.0f, 1.0f);
      mDurs[index] = wrap(mDurs[index] + dstp, 0.5, 1.5);
    }

    amp_next = mAmps[index];

    // step/adjust grain sample offsets
    g_idx = g_idx_next;
    g_idx_next = 0.0;
  }

  // change amp in sample buffer
  sample[idx] = wrap(sample[idx] + (amp * env.get(g_idx)), -5.f, 5.f);
  amp_out = sample[idx];

  idx = (idx + 1) % sample_length;
  g_idx = fmod(g_idx + (1.f / (4.f * env_dur)), 1.f);
  g_idx_next = fmod(g_idx_next + (1.f / (4.f * env_dur)), 1.f);

  phase += 1.f / (mDurs[index] * bpt_spc);

  // get that amp OUT
  outputs[SINE_OUTPUT].setVoltage(amp_out);
}

Model *modelReGenEcho = createModel<ReGenEcho, ReGenEchoWidget>("ReGenEcho");
