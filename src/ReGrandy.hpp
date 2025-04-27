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

    configParam(FREQ_PARAM, -4.0, 3.0, 0.0);
    configParam(FREQCV_PARAM, 0.f, 1.f, 0.f);
    configParam(BPTS_PARAM, 3, MAX_BPTS, 0);
    configParam(BPTSCV_PARAM, 0.f, 1.f, 0.f);
    configParam(DSTP_PARAM, 0.f, 1.f, 0.f);
    configParam(DSTPCV_PARAM, 0.f, 1.f, 0.f);
    configParam(ASTP_PARAM, 0.f, 1.f, 0.f);
    configParam(ASTPCV_PARAM, 0.f, 1.f, 0.f);
    configParam(PDST_PARAM, 0.f, 2.f, 0.f);
    configParam(MIRR_PARAM, 0.f, 1.f, 0.f);
    configParam(GRAT_PARAM, -6.f, 3.f, 0.f);
    configParam(GRATCV_PARAM, 0.f, 1.f, 0.f);
    configParam(ENVS_PARAM, 1.0f, 4.0f, 4.0f);
    configParam(FCAR_PARAM, -4.f, 4.f, 0.f);
    configParam(FMOD_PARAM, -4.f, 4.f, 0.f);
    configParam(FMODCV_PARAM, 0.f, 1.f, 0.f);
    configParam(IMOD_PARAM, -4.f, 4.f, 0.f);
    configParam(IMODCV_PARAM, 0.f, 1.f, 0.f);
    configParam(FMTR_PARAM, 0.0f, 1.0f, 0.0f);
  }

  void process(const ProcessArgs &args) override;
};

struct ReGrandyWidget : ModuleWidget
{
  ReGrandyWidget(ReGrandy *module)
  {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/12HP.svg")));

    addChild(createWidget<ScrewSilver>(Vec(0, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 1 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // knob params
    addParam(createParam<RoundLargeBlackKnob>(Vec(36.307, 50.42), module, ReGrandy::FREQ_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(61.360, 94.21), module, ReGrandy::FREQCV_PARAM));

    addParam(createParam<RoundLargeBlackKnob>(Vec(104.307, 50.42), module, ReGrandy::BPTS_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(129.360, 94.21), module, ReGrandy::BPTSCV_PARAM));

    addParam(createParam<RoundLargeBlackKnob>(Vec(14.307, 145.54), module, ReGrandy::DSTP_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(39.360, 191.10), module, ReGrandy::DSTPCV_PARAM));

    addParam(createParam<RoundLargeBlackKnob>(Vec(84.307, 145.54), module, ReGrandy::ASTP_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(107.360, 191.10), module, ReGrandy::ASTPCV_PARAM));

    addParam(createParam<CKSSThree>(Vec(143.417, 147.64), module, ReGrandy::PDST_PARAM));
    addParam(createParam<CKSS>(Vec(143.379, 202.07), module, ReGrandy::MIRR_PARAM));

    addParam(createParam<RoundSmallBlackKnob>(Vec(35.360, 243.98), module, ReGrandy::GRAT_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(69.360, 243.98), module, ReGrandy::GRATCV_PARAM));

    addParam(createParam<RoundBlackSnapKnob>(Vec(141.195, 240.69), module, ReGrandy::ENVS_PARAM));

    // for fm
    addParam(createParam<RoundSmallBlackKnob>(Vec(23.360, 302.25), module, ReGrandy::FCAR_PARAM));

    addParam(createParam<RoundSmallBlackKnob>(Vec(63.360, 302.25), module, ReGrandy::FMOD_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(97.360, 302.25), module, ReGrandy::FMODCV_PARAM));

    addParam(createParam<RoundSmallBlackKnob>(Vec(13.360, 348.84), module, ReGrandy::IMOD_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(47.360, 348.84), module, ReGrandy::IMODCV_PARAM));

    addParam(createParam<CKSS>(Vec(11.360, 257.01), module, ReGrandy::FMTR_PARAM));

    // signal inputs
    addInput(createInput<PJ301MPort>(Vec(24.967, 93.61), module, ReGrandy::FREQ_INPUT));
    addInput(createInput<PJ301MPort>(Vec(92.967, 93.61), module, ReGrandy::BPTS_INPUT));

    addInput(createInput<PJ301MPort>(Vec(2.976, 188.72), module, ReGrandy::ASTP_INPUT));
    addInput(createInput<PJ301MPort>(Vec(70.966, 188.72), module, ReGrandy::DSTP_INPUT));

    addInput(createInput<PJ301MPort>(Vec(102.966, 243.50), module, ReGrandy::GRAT_INPUT));

    // for fm
    addInput(createInput<PJ301MPort>(Vec(130.966, 300.72), module, ReGrandy::FMOD_INPUT));
    addInput(createInput<PJ301MPort>(Vec(82.966, 348.50), module, ReGrandy::IMOD_INPUT));

    // output signal
    addOutput(createOutput<PJ301MPort>(Vec(124.003, 348.50), module, ReGrandy::SINE_OUTPUT));
  }
};
