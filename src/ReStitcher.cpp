#include "ReStitcher.hpp"

void ReStitcher::process(const ProcessArgs &args)
{
  float deltaTime = args.sampleTime;

  // read in global switches
  g_is_mirroring = (int)params[MIRR_PARAM].getValue();
  g_is_fm_on = !(params[FMTR_PARAM].getValue() > 0.f);
  g_dt = (DistType)params[PDST_PARAM].getValue();

  // read in global controls
  g_freq_sig = params[G_FREQ_PARAM].getValue();
  g_bpts_sig = params[G_BPTS_PARAM].getValue();
  g_astp_sig = params[G_ASTP_PARAM].getValue();
  g_dstp_sig = params[G_DSTP_PARAM].getValue();
  g_grat_sig = params[G_GRAT_PARAM].getValue();

  g_fcar_sig = params[G_FCAR_PARAM].getValue();
  g_fmod_sig = params[G_FMOD_PARAM].getValue();
  g_imod_sig = params[G_IMOD_PARAM].getValue();

  g_freq_sig = (inputs[G_FREQ_INPUT].getVoltage() / 5.f) * params[G_FREQCV_PARAM].getValue();
  g_bpts_sig = (inputs[G_BPTS_INPUT].getVoltage() / 5.f) * params[G_BPTSCV_PARAM].getValue();
  g_astp_sig = (inputs[G_ASTP_INPUT].getVoltage() / 5.f) * params[G_ASTPCV_PARAM].getValue();
  g_dstp_sig = (inputs[G_DSTP_INPUT].getVoltage() / 5.f) * params[G_DSTPCV_PARAM].getValue();
  g_grat_sig = (inputs[G_GRAT_INPUT].getVoltage() / 5.f) * params[G_GRATCV_PARAM].getValue();

  g_fcar_sig += (inputs[G_FCAR_INPUT].getVoltage() / 5.f) * params[G_FCARCV_PARAM].getValue();
  g_fmod_sig += (inputs[G_FMOD_INPUT].getVoltage() / 5.f) * params[G_FMODCV_PARAM].getValue();
  g_imod_sig += (inputs[G_IMOD_INPUT].getVoltage() / 5.f) * params[G_IMODCV_PARAM].getValue();

  int prev = curr_num_oscs;
  curr_num_oscs = (int)clamp(params[G_NOSC_PARAM].getValue(), 1.f, 4.f);

  if (prev != curr_num_oscs)
    DEBUG("new # of oscs: %d\n", curr_num_oscs);

  // read in all the parameters for each oscillator
  for (int i = 0; i < NUM_OSCS; i++)
  {

    lights[ONOFF_LIGHT + i].setBrightness(i < curr_num_oscs ? 1.0f : 0.0f);
    stutters[i] = (int)params[ST_PARAM + i].getValue();

    gos[i].is_mirroring = g_is_mirroring;
    gos[i].is_fm_on = g_is_fm_on;
    gos[i].dt = g_dt;

    // accept modulation of signal inputs for each parameter

    freq_sig = (inputs[F_INPUT + i].getVoltage() / 5.f) * params[FCV_PARAM + i].getValue();
    freq_sig += g_freq_sig;
    freq_sig += params[F_PARAM + i].getValue();
    gos[i].freq = clamp(261.626f * powf(2.0f, freq_sig), 1.f, 3000.f);

    bpts_sig = 5.f * dsp::quadraticBipolar((inputs[B_INPUT + i].getVoltage() / 5.f) * params[BCV_PARAM + i].getValue());
    bpts_sig += g_bpts_sig;
    gos[i].num_bpts = clamp((int)params[B_PARAM + i].getValue() + (int)bpts_sig, 2, MAX_BPTS);

    astp_sig = dsp::quadraticBipolar((inputs[A_INPUT + i].getVoltage() / 5.f) * params[ACV_PARAM + i].getValue());
    astp_sig += g_astp_sig;
    gos[i].max_amp_step = rescale(params[A_PARAM + i].getValue() + (astp_sig / 4.f), 0.0, 1.0, 0.05, 0.3);

    dstp_sig = dsp::quadraticBipolar((inputs[D_INPUT + i].getVoltage() / 5.f) * params[DCV_PARAM].getValue());
    dstp_sig += g_dstp_sig;
    gos[i].max_dur_step = rescale(params[D_PARAM + i].getValue() + (dstp_sig / 4.f), 0.0, 1.0, 0.01, 0.3);

    grat_sig = (inputs[G_INPUT + i].getVoltage() / 5.f) * params[GCV_PARAM].getValue();
    gos[i].g_rate = clamp(261.626f * powf(2.0f, grat_sig + g_grat_sig), 1e-6, 3000.f);

    // fm control sigs
    fcar_sig = g_grat_sig;
    fcar_sig += g_fcar_sig;
    fcar_sig += params[FCAR_PARAM + i].getValue();
    gos[i].f_car = clamp(261.626f * powf(2.0f, fcar_sig), 1.f, 3000.f);

    // no local controls for the frequency of the modulating signal, so just
    // respond to the global control values
    gos[i].f_mod = clamp(261.626f * powf(2.0f, g_fmod_sig), 1.f, 3000.f);

    imod_sig = dsp::quadraticBipolar((inputs[IMOD_INPUT + i].getVoltage() / 5.f) * params[IMODCV_PARAM + i].getValue());
    imod_sig += g_imod_sig;
    imod_sig += params[IMOD_PARAM].getValue();
    gos[i].i_mod = rescale(imod_sig, 0.f, 1.f, 10.f, 3000.f);
  }

  if (is_swapping)
  {
    amp_out = ((1.0 - phase) * amp) + (phase * amp_next);
    phase += speed;

    if (phase >= 1.0)
      is_swapping = false;
  }
  else
  {

    gos[osc_idx].process(deltaTime);
    amp_out = gos[osc_idx].out();

    if (gos[osc_idx].last_flag)
    {
      current_stutter--;
      if (current_stutter < 1)
      {
        amp = amp_out;
        speed = gos[osc_idx].speed;
        osc_idx = (osc_idx + 1) % curr_num_oscs;

        gos[osc_idx].process(deltaTime);
        amp_next = gos[osc_idx].out();

        current_stutter = stutters[osc_idx];

        phase = 0.f;
        is_swapping = true;
      }
    }
  }

  outputs[SINE_OUTPUT].setVoltage(5.0f * amp_out);
}

Model *modelReStitcher = createModel<ReStitcher, ReStitcherWidget>("ReStitcher");
