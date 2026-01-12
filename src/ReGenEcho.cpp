#include "ReGenEcho.hpp"
#include <cstring>

namespace
{
  constexpr float VOLTAGE_SCALE = 5.0f;
  constexpr float INV_VOLTAGE_SCALE = 0.2f; // 1/5
  constexpr float MIN_AMP_STEP = 0.05f;
  constexpr float MAX_AMP_STEP = 0.3f;
  constexpr float MIN_DUR_STEP = 0.01f;
  constexpr float MAX_DUR_STEP = 0.3f;
  constexpr int MIN_BPT_SPC = 800;
  constexpr int MAX_BPT_SPC_OFFSET = 200;
  constexpr float INV_CROSSFADE_SAMPLES = 0.02f; // 1/50
}

void ReGenEcho::process(const ProcessArgs &args)
{
  // Update switches and distribution type
  is_accumulating = static_cast<bool>(params[ACCM_PARAM].getValue());
  is_mirroring = static_cast<bool>(params[MIRR_PARAM].getValue());
  dt = static_cast<DistType>(params[PDST_PARAM].getValue());

  // Process CV inputs with optimized scaling
  const float bpts_cv = inputs[BPTS_INPUT].getVoltage() * INV_VOLTAGE_SCALE * params[BPTSCV_PARAM].getValue();
  const float astp_cv = inputs[ASTP_INPUT].getVoltage() * INV_VOLTAGE_SCALE * params[ASTPCV_PARAM].getValue();
  const float dstp_cv = inputs[DSTP_INPUT].getVoltage() * INV_VOLTAGE_SCALE * params[DSTPCV_PARAM].getValue();
  
  bpts_sig = VOLTAGE_SCALE * dsp::quadraticBipolar(bpts_cv);
  astp_sig = dsp::quadraticBipolar(astp_cv);
  dstp_sig = dsp::quadraticBipolar(dstp_cv);

  // Calculate step sizes
  max_amp_step = rescale(params[ASTP_PARAM].getValue() + astp_sig * 4.0f, 0.0f, 1.0f, MIN_AMP_STEP, MAX_AMP_STEP);
  max_dur_step = rescale(params[DSTP_PARAM].getValue() + dstp_sig * 4.0f, 0.0f, 1.0f, MIN_DUR_STEP, MAX_DUR_STEP);

  // Update sample length
  sample_length = static_cast<unsigned int>(clamp(params[SLEN_PARAM].getValue(), 0.1f, 1.0f) * MAX_SAMPLE_SIZE);

  // Calculate breakpoint spacing
  bpt_spc = static_cast<unsigned int>(params[BPTS_PARAM].getValue()) + MIN_BPT_SPC;
  bpt_spc += static_cast<unsigned int>(rescale(bpts_sig, -1.0f, 1.0f, 1.0f, static_cast<float>(MAX_BPT_SPC_OFFSET)));
  num_bpts = sample_length / bpt_spc + 1;
  env_dur = bpt_spc >> 1; // Bit shift for divide by 2

  // Update envelope type if changed
  const int env_num = static_cast<int>(clamp(roundf(params[ENVS_PARAM].getValue()), 1.0f, 4.0f));
  if (env.et != static_cast<EnvType>(env_num))
  {
    env.switchEnvType(static_cast<EnvType>(env_num));
  }

  // Handle reset trigger - use memcpy for fast array copy
  if (smpTrigger.process(params[TRIG_PARAM].getValue()) || resetTrigger.process(inputs[RSET_INPUT].getVoltage() * 0.5f))
  {
    std::memcpy(sample, _sample, MAX_SAMPLE_SIZE * sizeof(float));
    std::memset(mAmps, 0, MAX_BPTS * sizeof(float));
    for (unsigned int i = 0; i < MAX_BPTS; i++)
    {
      mDurs[i] = 1.0f;
    }
  }

  // Handle gate trigger
  if (g2Trigger.process(inputs[GATE_INPUT].getVoltage() * 0.5f))
  {
    std::memset(mAmps, 0, MAX_BPTS * sizeof(float));
    for (unsigned int i = 0; i < MAX_BPTS; i++)
    {
      mDurs[i] = 1.0f;
    }
    num_bpts = sample_length / bpt_spc;
    sampling = true;
    idx = 0;
    s_i = 0;
  }

  // Handle sampling
  if (sampling)
  {
    if (s_i >= MAX_SAMPLE_SIZE - 50)
    {
      // Crossfade the end of the sample to the beginning to avoid clicks
      const float x = sample[s_i - 1];
      const float y = sample[0];
      float p = 0.0f;
      while (s_i < MAX_SAMPLE_SIZE)
      {
        sample[s_i] = x + p * (y - x); // Optimized lerp
        p += INV_CROSSFADE_SAMPLES;
        s_i++;
      }
      DEBUG("Finished sampling");
      sampling = false;
    }
    else
    {
      const float input = inputs[WAV0_INPUT].getVoltage();
      sample[s_i] = input;
      _sample[s_i] = input;
      s_i++;
    } 
  }

  // Update breakpoint when phase wraps
  if (phase >= 1.0f)
  {
    phase -= 1.0f;
    amp = amp_next;
    
    // Wrap index efficiently
    index++;
    if (index >= num_bpts) index = 0;
    
    // Generate random steps
    astp = max_amp_step * rg.my_rand(dt, random::normal());
    dstp = max_dur_step * rg.my_rand(dt, random::normal());

    // Apply mirror or wrap mode
    const float base_amp = is_accumulating ? mAmps[index] : 0.0f;
    if (is_mirroring)
    {
      mAmps[index] = mirror(base_amp + astp, -1.0f, 1.0f); 
      mDurs[index] = mirror(mDurs[index] + dstp, 0.5f, 1.5f);
    }
    else
    {
      mAmps[index] = wrap(base_amp + astp, -1.0f, 1.0f); 
      mDurs[index] = wrap(mDurs[index] + dstp, 0.5f, 1.5f);
    }
  
    amp_next = mAmps[index];
    g_idx = g_idx_next;
    g_idx_next = 0.0f;
  }

  // Process sample buffer - cache envelope calculation
  const float env_value = env.get(g_idx);
  sample[idx] = wrap(sample[idx] + amp * env_value, -5.0f, 5.0f);
  const float amp_out = sample[idx];

  // Update indices - optimize modulo for sample_length
  idx++;
  if (idx >= sample_length) idx = 0;
  
  // Cache reciprocal for grain index increment
  const float grain_inc = 1.0f / (4.0f * env_dur);
  g_idx += grain_inc;
  if (g_idx >= 1.0f) g_idx -= 1.0f;
  
  g_idx_next += grain_inc;
  if (g_idx_next >= 1.0f) g_idx_next -= 1.0f;
  
  // Cache reciprocal for phase increment
  const float phase_inc = 1.0f / (mDurs[index] * bpt_spc);
  phase += phase_inc;

  // Apply limiter and output
  const float limited_output = limiter.process(amp_out);
  outputs[SINE_OUTPUT].setVoltage(limited_output);
}

Model *modelReGenEcho = createModel<ReGenEcho, ReGenEchoWidget>("ReGenEcho");
