/*
 * ReStitcher.cpp
 *
 * VCV Rack ReStitcher module. An extened version of granular stochastic
 * synthesis that connects waves produced by up to four seperate
 * GRANDY oscillators
 */

#pragma once

#include "plugin.hpp"
#include "dsp/digital.hpp"
#include "dsp/resampler.hpp"

#include "utils/wavetable.hpp"
#include "utils/GrandyOscillator.hpp"

#define NUM_OSCS 4

struct ReStitcher : Module
{
  enum ParamIds
  {
    G_FREQ_PARAM,
    G_ASTP_PARAM,
    G_DSTP_PARAM,
    G_BPTS_PARAM,
    G_GRAT_PARAM,
    G_FCAR_PARAM,
    G_FMOD_PARAM,
    G_IMOD_PARAM,
    G_FREQCV_PARAM,
    G_ASTPCV_PARAM,
    G_DSTPCV_PARAM,
    G_BPTSCV_PARAM,
    G_GRATCV_PARAM,
    G_FCARCV_PARAM,
    G_FMODCV_PARAM,
    G_IMODCV_PARAM,
    G_NOSC_PARAM,
    TRIG_PARAM,
    ENUMS(F_PARAM, NUM_OSCS),
    ENUMS(B_PARAM, NUM_OSCS),
    ENUMS(A_PARAM, NUM_OSCS),
    ENUMS(D_PARAM, NUM_OSCS),
    ENUMS(G_PARAM, NUM_OSCS),
    ENUMS(FCAR_PARAM, NUM_OSCS),
    ENUMS(FMOD_PARAM, NUM_OSCS),
    ENUMS(IMOD_PARAM, NUM_OSCS),
    ENUMS(FCARCV_PARAM, NUM_OSCS),
    ENUMS(FMODCV_PARAM, NUM_OSCS),
    ENUMS(IMODCV_PARAM, NUM_OSCS),
    ENUMS(FCV_PARAM, NUM_OSCS),
    ENUMS(BCV_PARAM, NUM_OSCS),
    ENUMS(ACV_PARAM, NUM_OSCS),
    ENUMS(DCV_PARAM, NUM_OSCS),
    ENUMS(GCV_PARAM, NUM_OSCS),
    ENUMS(ST_PARAM, NUM_OSCS),
    FMTR_PARAM,
    PDST_PARAM,
    MIRR_PARAM,
    NUM_PARAMS
  };
  enum InputIds
  {
    WAV0_INPUT,
    G_FREQ_INPUT,
    G_ASTP_INPUT,
    G_DSTP_INPUT,
    G_BPTS_INPUT,
    G_GRAT_INPUT,
    G_FCAR_INPUT,
    G_FMOD_INPUT,
    G_IMOD_INPUT,
    ENUMS(F_INPUT, NUM_OSCS),
    ENUMS(B_INPUT, NUM_OSCS),
    ENUMS(A_INPUT, NUM_OSCS),
    ENUMS(D_INPUT, NUM_OSCS),
    ENUMS(G_INPUT, NUM_OSCS),
    ENUMS(FCAR_INPUT, NUM_OSCS),
    ENUMS(FMOD_INPUT, NUM_OSCS),
    ENUMS(IMOD_INPUT, NUM_OSCS),
    NUM_INPUTS
  };
  enum OutputIds
  {
    SINE_OUTPUT,
    NUM_OUTPUTS
  };
  enum LightIds
  {
    BLINK_LIGHT,
    ENUMS(ONOFF_LIGHT, NUM_OSCS),
    NUM_LIGHTS
  };

  float blinkPhase = 0.0;

  dsp::SchmittTrigger smpTrigger;

  GendyOscillator gos[NUM_OSCS];
  int osc_idx = 0;

  // allow an adjustable number of oscillators
  // to be used 1 -> 4
  int curr_num_oscs = NUM_OSCS;
  int stutters[NUM_OSCS] = {1};
  int current_stutter = 1;

  float phase = 0.f;
  float amp = 0.f;
  float amp_next = 0.f;
  float amp_out = 0.f;
  float speed = 0.f;

  bool is_swapping = false;
  int stutter = 1;

  // vars for global parameter controls
  float g_freq_mul = 1.0;
  float g_max_amp_add = 0.f;
  float g_max_dur_add = 0.f;

  float g_freq_sig = 0.f;
  float g_bpts_sig = 0.f;
  float g_astp_sig = 0.f;
  float g_dstp_sig = 0.f;
  float g_grat_sig = 0.f;
  float g_fcar_sig = 0.f;
  float g_fmod_sig = 0.f;
  float g_imod_sig = 0.f;

  float freq_sig = 0.f;
  float bpts_sig = 0.f;
  float astp_sig = 0.f;
  float dstp_sig = 0.f;
  float grat_sig = 0.f;
  float fcar_sig = 0.f;
  float fmod_sig = 0.f;
  float imod_sig = 0.f;

  bool g_is_mirroring = false;
  bool g_is_fm_on = false;
  DistType g_dt = LINEAR;

  float wrap(float, float, float);

  ReStitcher()
  {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

    for (int i = 0; i < NUM_OSCS; i++)
    {
      configParam(F_PARAM + i, -4.f, 4.f, 0.f, "Oscillator frequency");
      configParam(B_PARAM + i, 3.f, MAX_BPTS, 0.f, "Number of breakpoints");
      configParam(A_PARAM + i, 0.f, 1.f, 0.f, "Maximum amplitude step");
      configParam(D_PARAM + i, 0.f, 1.f, 0.f, "Maximum duration step");
      configParam(G_PARAM + i, 0.7, 1.3, 0.0, "Granulation rate");
      configParam(FCV_PARAM + i, 0.f, 1.f, 0.f, "Frequency CV amount");
      configParam(BCV_PARAM + i, 0.f, 1.f, 0.f, "Breakpoints CV amount");
      configParam(ACV_PARAM + i, 0.f, 1.f, 0.f, "Amplitude step CV amount");
      configParam(DCV_PARAM + i, 0.f, 1.f, 0.f, "Duration step CV amount");
      configParam(GCV_PARAM + i, 0.f, 1.f, 0.f, "Granulation rate CV amount");
      configParam(ST_PARAM + i, 1.f, 5.f, 5.f, "Stutter - number of cycles to output");
    }

    configParam(G_FREQ_PARAM, -1.f, 1.f, 0.f, "Global frequency modifier");
    configParam(G_BPTS_PARAM, -1.f, 1.f, 0.f, "Global breakpoints modifier");
    configParam(G_ASTP_PARAM, -1.f, 1.f, 0.f, "Global amplitude step modifier");
    configParam(G_DSTP_PARAM, -1.f, 1.f, 0.f, "Global duration step modifier");
    configParam(G_GRAT_PARAM, -1.f, 1.f, 0.f, "Global granulation rate modifier");
    configParam(G_FCAR_PARAM, -1.f, 1.f, 0.f, "Global carrier frequency modifier");
    configParam(G_FMOD_PARAM, -1.f, 1.f, 0.f, "Global modulation frequency modifier");
    configParam(G_IMOD_PARAM, -1.f, 1.f, 0.f, "Global modulation index modifier");
    configParam(G_FREQCV_PARAM, 0.f, 1.f, 0.f, "Global frequency CV amount");
    configParam(G_BPTSCV_PARAM, 0.f, 1.f, 0.f, "Global breakpoints CV amount");
    configParam(G_ASTPCV_PARAM, 0.f, 1.f, 0.f, "Global amplitude step CV amount");
    configParam(G_DSTPCV_PARAM, 0.f, 1.f, 0.f, "Global duration step CV amount");
    configParam(G_GRATCV_PARAM, 0.f, 1.f, 0.f, "Global granulation rate CV amount");
    configParam(G_FCARCV_PARAM, 0.f, 1.f, 0.f, "Global carrier frequency CV amount");
    configParam(G_FMODCV_PARAM, 0.f, 1.f, 0.f, "Global modulation frequency CV amount");
    configParam(G_IMODCV_PARAM, 0.f, 1.f, 0.f, "Global modulation index CV amount");
    configParam(G_NOSC_PARAM, 1.f, 4.f, 4.f, "Number of active oscillators");
    configParam(FMTR_PARAM, 0.0f, 1.0f, 0.0f, "FM mode toggle");
    configParam(MIRR_PARAM, 0.f, 1.f, 0.f, "Mirror toggle - mirror vs wrap breakpoints");
    configParam(PDST_PARAM, 0.f, 2.f, 0.f, "Probability distribution - Linear/Cauchy/Arcsin");
  }

  void process(const ProcessArgs &args) override;
};

struct ReStitcherWidget : ModuleWidget
{
  ReStitcherWidget(ReStitcher *module)
  {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/23HP_Blank.svg")));

    addChild(createWidget<ScrewSilver>(Vec(0, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 1 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    for (int i = 0; i < NUM_OSCS; i++)
    {
      addParam(createParam<RoundSmallBlackKnob>(Vec(18.004, 10.89 + (i * 95)), module, ReStitcher::F_PARAM + i));
      addParam(createParam<RoundSmallBlackKnob>(Vec(44.004, 10.89 + (i * 95)), module, ReStitcher::B_PARAM + i));

      addParam(createParam<RoundSmallBlackKnob>(Vec(70.004, 10.89 + (i * 95)), module, ReStitcher::A_PARAM + i));
      addParam(createParam<RoundSmallBlackKnob>(Vec(96.004, 10.89 + (i * 95)), module, ReStitcher::D_PARAM + i));

      addParam(createParam<RoundSmallBlackKnob>(Vec(122.004, 10.89 + (i * 95)), module, ReStitcher::G_PARAM + i));

      addParam(createParam<RoundSmallBlackKnob>(Vec(18.004, 36.89 + (i * 95)), module, ReStitcher::FCV_PARAM + i));
      addParam(createParam<RoundSmallBlackKnob>(Vec(44.004, 36.89 + (i * 95)), module, ReStitcher::BCV_PARAM + i));

      addParam(createParam<RoundSmallBlackKnob>(Vec(70.004, 36.89 + (i * 95)), module, ReStitcher::ACV_PARAM + i));
      addParam(createParam<RoundSmallBlackKnob>(Vec(96.004, 36.89 + (i * 95)), module, ReStitcher::DCV_PARAM + i));

      addParam(createParam<RoundSmallBlackKnob>(Vec(122.004, 36.89 + (i * 95)), module, ReStitcher::GCV_PARAM + i));

      // stutter param
      addParam(createParam<RoundBlackSnapKnob>(Vec(157.640, 23.57 + (i * 95)), module, ReStitcher::ST_PARAM + i));

      // CV inputs
      addInput(createInput<PJ301MPort>(Vec(18.004, 64.39 + (i * 95)), module, ReStitcher::F_INPUT + i));
      addInput(createInput<PJ301MPort>(Vec(44.004, 64.39 + (i * 95)), module, ReStitcher::B_INPUT + i));

      addInput(createInput<PJ301MPort>(Vec(70.004, 64.39 + (i * 95)), module, ReStitcher::A_INPUT + i));
      addInput(createInput<PJ301MPort>(Vec(96.004, 64.39 + (i * 95)), module, ReStitcher::D_INPUT + i));

      addInput(createInput<PJ301MPort>(Vec(122.004, 64.39 + (i * 95)), module, ReStitcher::G_INPUT + i));

      // light to signal if oscillator on / off
      addChild(createLight<SmallLight<GreenLight>>(Vec(157.185, 75 + (i * 95)), module, ReStitcher::ONOFF_LIGHT + i));
    }

    // global controls (on the right of the panel)
    addParam(createParam<RoundSmallBlackKnob>(Vec(239.140, 26.77), module, ReStitcher::G_FREQ_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(239.140, 60.77), module, ReStitcher::G_BPTS_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(239.140, 94.77), module, ReStitcher::G_ASTP_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(239.140, 128.77), module, ReStitcher::G_DSTP_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(239.140, 161.77), module, ReStitcher::G_GRAT_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(239.140, 200.77), module, ReStitcher::G_FCAR_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(239.140, 234.77), module, ReStitcher::G_FMOD_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(239.140, 268.77), module, ReStitcher::G_IMOD_PARAM));

    addParam(createParam<RoundSmallBlackKnob>(Vec(271.140, 26.77), module, ReStitcher::G_FREQCV_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(271.140, 60.77), module, ReStitcher::G_BPTSCV_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(271.140, 94.77), module, ReStitcher::G_ASTPCV_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(271.140, 128.77), module, ReStitcher::G_DSTPCV_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(271.140, 161.77), module, ReStitcher::G_GRATCV_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(271.140, 200.77), module, ReStitcher::G_FCARCV_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(271.140, 234.77), module, ReStitcher::G_FMODCV_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(271.140, 268.77), module, ReStitcher::G_IMODCV_PARAM));

    addInput(createInput<PJ301MPort>(Vec(301.539, 26.77), module, ReStitcher::G_FREQ_INPUT));
    addInput(createInput<PJ301MPort>(Vec(301.539, 60.77), module, ReStitcher::G_BPTS_INPUT));
    addInput(createInput<PJ301MPort>(Vec(301.539, 94.77), module, ReStitcher::G_ASTP_INPUT));
    addInput(createInput<PJ301MPort>(Vec(301.539, 128.77), module, ReStitcher::G_DSTP_INPUT));
    addInput(createInput<PJ301MPort>(Vec(301.539, 161.77), module, ReStitcher::G_GRAT_INPUT));
    addInput(createInput<PJ301MPort>(Vec(301.539, 200.77), module, ReStitcher::G_FCAR_INPUT));
    addInput(createInput<PJ301MPort>(Vec(301.539, 234.77), module, ReStitcher::G_FMOD_INPUT));
    addInput(createInput<PJ301MPort>(Vec(301.539, 268.77), module, ReStitcher::G_IMOD_INPUT));

    addParam(createParam<RoundBlackSnapKnob>(Vec(285.140, 306.80), module, ReStitcher::G_NOSC_PARAM));

    // the few switches for fm toggle, probability distrobution selection
    // and mirroring toggle
    addParam(createParam<CKSS>(Vec(218.392, 304.22), module, ReStitcher::FMTR_PARAM));
    addParam(createParam<CKSS>(Vec(252.392, 324.22), module, ReStitcher::MIRR_PARAM));
    addParam(createParam<CKSSThree>(Vec(218.392, 338.16), module, ReStitcher::PDST_PARAM));

    addOutput(createOutput<PJ301MPort>(Vec(286.140, 342.50), module, ReStitcher::SINE_OUTPUT));
  }
};
