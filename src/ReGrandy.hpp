/*
 * ReGrandy.cpp
 *
 * VCV Rack Module with a single GRANDY oscillator
 */

#pragma once

#include "plugin.hpp"
#include "dsp/resampler.hpp"
#include "utils/GrandyOscillator.hpp"
#include "utils/Limiter.hpp"

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
    INV_OUTPUT,
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
  
  AudioLimiter limiter;

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
    
    // Initialize limiter with default sample rate
    limiter.init(APP->engine->getSampleRate());
  }

  void process(const ProcessArgs &args) override;
  
  void onSampleRateChange() override
  {
    limiter.init(APP->engine->getSampleRate());
  }

  void updateEnvelopeType(const ProcessArgs &args);
  void processModulationInputs();
  void updateGranularParameters();
  void updateFMParameters();
};

struct ReGrandyWidget : ModuleWidget
{
  ReGrandyWidget(ReGrandy *module)
  {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ReGrandy-panel.svg")));

    addChild(createWidget<ScrewSilver>(Vec(0, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 1 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // Frequency
    addParam(createParam<RoundLargeBlackKnob>(Vec(19, 20), module, ReGrandy::FREQ_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(26, 80), module, ReGrandy::FREQCV_PARAM));
    addInput(createInput<PJ301MPort>(Vec(26, 108), module, ReGrandy::FREQ_INPUT));

    // Breakpoints
    addParam(createParam<RoundLargeBlackKnob>(Vec(69, 20), module, ReGrandy::BPTS_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(76, 80), module, ReGrandy::BPTSCV_PARAM));
    addInput(createInput<PJ301MPort>(Vec(76, 108), module, ReGrandy::BPTS_INPUT));

    // DSTP
    addParam(createParam<RoundLargeBlackKnob>(Vec(119, 20), module, ReGrandy::DSTP_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(126, 80), module, ReGrandy::DSTPCV_PARAM));
    addInput(createInput<PJ301MPort>(Vec(126, 108), module, ReGrandy::DSTP_INPUT));

    // ASTP
    addParam(createParam<RoundLargeBlackKnob>(Vec(169, 20), module, ReGrandy::ASTP_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(176, 80), module, ReGrandy::ASTPCV_PARAM));
    addInput(createInput<PJ301MPort>(Vec(176, 108), module, ReGrandy::ASTP_INPUT));

    // Grat
    addParam(createParam<RoundLargeBlackKnob>(Vec(19, 200), module, ReGrandy::GRAT_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(26, 260), module, ReGrandy::GRATCV_PARAM));
    addInput(createInput<PJ301MPort>(Vec(26, 288), module, ReGrandy::GRAT_INPUT));

    // F Mod
    addParam(createParam<RoundLargeBlackKnob>(Vec(69, 200), module, ReGrandy::FMOD_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(76, 260), module, ReGrandy::FMODCV_PARAM));
    addInput(createInput<PJ301MPort>(Vec(76, 288), module, ReGrandy::FMOD_INPUT));

    // I Mod
    addParam(createParam<RoundLargeBlackKnob>(Vec(119, 200), module, ReGrandy::IMOD_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(126, 260), module, ReGrandy::IMODCV_PARAM));
    addInput(createInput<PJ301MPort>(Vec(126, 288), module, ReGrandy::IMOD_INPUT));

    // F Carrier
    addParam(createParam<RoundLargeBlackKnob>(Vec(169, 200), module, ReGrandy::FCAR_PARAM));

    // Envs
    addParam(createParam<RoundBlackSnapKnob>(Vec(171, 257), module, ReGrandy::ENVS_PARAM));

    // PDST Mode 
    addParam(createParam<CKSSThree>(Vec(80.5, 155), module, ReGrandy::PDST_PARAM));

    // FM Toggle
    addParam(createParam<CKSS>(Vec(105.5, 155), module, ReGrandy::FMTR_PARAM));

    // Mirror Mode
    addParam(createParam<CKSS>(Vec(130.5, 155), module, ReGrandy::MIRR_PARAM));

    // OSC Output
    addOutput(createOutput<PJ301MPort>(Vec(76, 347), module, ReGrandy::SINE_OUTPUT));

    // Inv Output
    addOutput(createOutput<PJ301MPort>(Vec(126, 347), module, ReGrandy::INV_OUTPUT));
  }
};
