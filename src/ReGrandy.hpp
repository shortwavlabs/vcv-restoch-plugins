/*
 * ReGrandy.cpp
 *
 * VCV Rack Module with a single GRANDY oscillator
 */

#pragma once

#include "plugin.hpp"
#include "dsp/resampler.hpp"
#include "utils/GrandyOscillator.hpp"

struct ReGrandy : Module
{
  enum ParamIds
  {
    FREQ_PARAM,
    ASTP_PARAM,
    DSTP_PARAM,
    BPTS_PARAM,
    GRAT_PARAM,
    GRATCV_PARAM,
    FREQCV_PARAM,
    ASTPCV_PARAM,
    DSTPCV_PARAM,
    BPTSCV_PARAM,
    TRIG_PARAM,
    FMTR_PARAM,
    ENVS_PARAM,
    FMOD_PARAM,
    FCAR_PARAM,
    IMOD_PARAM,
    FMODCV_PARAM,
    IMODCV_PARAM,
    PDST_PARAM,
    MIRR_PARAM,
    NUM_PARAMS
  };

  enum InputIds
  {
    FREQ_INPUT,
    ASTP_INPUT,
    DSTP_INPUT,
    BPTS_INPUT,
    ENVS_INPUT,
    FMOD_INPUT,
    IMOD_INPUT,
    GRAT_INPUT,
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
    NUM_LIGHTS
  };

  float blinkPhase = 0.0;

  dsp::SchmittTrigger smpTrigger;

  GendyOscillator go;

  EnvType env = (EnvType)1;

  float freq_sig = 0.f;
  float astp_sig = 0.f;
  float dstp_sig = 0.f;
  float grat_sig = 0.f;
  float envs_sig = 0.f;
  float bpts_sig = 0.f;
  float fmod_sig = 0.f;
  float imod_sig = 0.f;

  bool fm_is_on = false;

  float wrap(float, float, float);

  ReGrandy()
  {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

    configParam(FREQ_PARAM, -4.0, 3.0, 0.0, "Frequency");
    configParam(FREQCV_PARAM, 0.f, 1.f, 0.f, "Frequency CV Amount");
    configParam(BPTS_PARAM, 3, MAX_BPTS, 0, "Number of Breakpoints");
    configParam(BPTSCV_PARAM, 0.f, 1.f, 0.f, "Breakpoints CV Amount");
    configParam(DSTP_PARAM, 0.f, 1.f, 0.f, "Maximum Duration Step");
    configParam(DSTPCV_PARAM, 0.f, 1.f, 0.f, "Duration Step CV Amount");
    configParam(ASTP_PARAM, 0.f, 1.f, 0.f, "Maximum Amplitude Step");
    configParam(ASTPCV_PARAM, 0.f, 1.f, 0.f, "Amplitude Step CV Amount");
    configParam(PDST_PARAM, 0.f, 2.f, 0.f, "Probability Distribution", "l - LINEAR, c - CAUCHY, a - ARCSIN");
    configParam(MIRR_PARAM, 0.f, 1.f, 0.f, "Mirror Mode", "Toggle between wrapping and mirroring of breakpoints");
    configParam(GRAT_PARAM, -6.f, 3.f, 0.f, "Granulation Frequency", "Control frequency of the sin wave that is granulated");
    configParam(GRATCV_PARAM, 0.f, 1.f, 0.f, "Granulation Frequency CV Amount");
    configParam(ENVS_PARAM, 1.0f, 4.0f, 4.0f, "Envelope Type");
    configParam(FCAR_PARAM, -4.f, 4.f, 0.f, "FM Carrier Frequency");
    configParam(FMOD_PARAM, -4.f, 4.f, 0.f, "FM Modulation Frequency");
    configParam(FMODCV_PARAM, 0.f, 1.f, 0.f, "FM Modulation Frequency CV Amount");
    configParam(IMOD_PARAM, -4.f, 4.f, 0.f, "FM Modulation Index");
    configParam(IMODCV_PARAM, 0.f, 1.f, 0.f, "FM Modulation Index CV Amount");
    configParam(FMTR_PARAM, 0.0f, 1.0f, 0.0f, "FM Mode Toggle");
  }

  void process(const ProcessArgs &args) override;
};

struct ReGrandyWidget : ModuleWidget
{
  ReGrandyWidget(ReGrandy *module)
  {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ReGrandy-v4d.svg")));

    addChild(createWidget<ScrewSilver>(Vec(0, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 1 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // knob params
    addParam(createParam<RoundLargeBlackKnob>(Vec(40.307, 43.42), module, ReGrandy::FREQ_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(65.360, 87.21), module, ReGrandy::FREQCV_PARAM));

    addParam(createParam<RoundLargeBlackKnob>(Vec(108.307, 43.42), module, ReGrandy::BPTS_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(133.360, 87.21), module, ReGrandy::BPTSCV_PARAM));

    addParam(createParam<RoundLargeBlackKnob>(Vec(18.307, 138.54), module, ReGrandy::DSTP_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(43.360, 184.10), module, ReGrandy::DSTPCV_PARAM));

    addParam(createParam<RoundLargeBlackKnob>(Vec(88.307, 138.54), module, ReGrandy::ASTP_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(111.360, 184.10), module, ReGrandy::ASTPCV_PARAM));

    addParam(createParam<CKSSThree>(Vec(147.417, 140.64), module, ReGrandy::PDST_PARAM));
    addParam(createParam<CKSS>(Vec(147.379, 195.07), module, ReGrandy::MIRR_PARAM));

    addParam(createParam<RoundSmallBlackKnob>(Vec(39.360, 236.98), module, ReGrandy::GRAT_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(73.360, 236.98), module, ReGrandy::GRATCV_PARAM));

    addParam(createParam<RoundBlackSnapKnob>(Vec(145.195, 233.69), module, ReGrandy::ENVS_PARAM));

    // for fm
    addParam(createParam<RoundSmallBlackKnob>(Vec(27.360, 295.25), module, ReGrandy::FCAR_PARAM));

    addParam(createParam<RoundSmallBlackKnob>(Vec(67.360, 295.25), module, ReGrandy::FMOD_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(101.360, 295.25), module, ReGrandy::FMODCV_PARAM));

    addParam(createParam<RoundSmallBlackKnob>(Vec(17.360, 341.84), module, ReGrandy::IMOD_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(51.360, 341.84), module, ReGrandy::IMODCV_PARAM));

    addParam(createParam<CKSS>(Vec(15.360, 250.01), module, ReGrandy::FMTR_PARAM));

    // signal inputs
    addInput(createInput<PJ301MPort>(Vec(28.967, 86.61), module, ReGrandy::FREQ_INPUT));
    addInput(createInput<PJ301MPort>(Vec(96.967, 86.61), module, ReGrandy::BPTS_INPUT));

    addInput(createInput<PJ301MPort>(Vec(6.976, 181.72), module, ReGrandy::ASTP_INPUT));
    addInput(createInput<PJ301MPort>(Vec(74.966, 181.72), module, ReGrandy::DSTP_INPUT));

    addInput(createInput<PJ301MPort>(Vec(106.966, 236.50), module, ReGrandy::GRAT_INPUT));

    // for fm
    addInput(createInput<PJ301MPort>(Vec(134.966, 293.72), module, ReGrandy::FMOD_INPUT));
    addInput(createInput<PJ301MPort>(Vec(86.966, 341.50), module, ReGrandy::IMOD_INPUT));

    // output signal
    addOutput(createOutput<PJ301MPort>(Vec(128.003, 341.50), module, ReGrandy::SINE_OUTPUT));
  }
};
