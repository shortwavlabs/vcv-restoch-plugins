#pragma once

#include "plugin.hpp"
#include "dsp/digital.hpp"
#include "dsp/resampler.hpp"

#include "utils/wavetable.hpp"

#define MAX_BPTS 4096
#define MAX_SAMPLE_SIZE 44100

struct ReGenEcho : Module
{
  enum ParamIds
  {
    BPTS_PARAM,
    TRIG_PARAM,
    GATE_PARAM,
    ASTP_PARAM,
    DSTP_PARAM,
    ENVS_PARAM,
    SLEN_PARAM,
    BPTSCV_PARAM,
    ASTPCV_PARAM,
    DSTPCV_PARAM,
    MIRR_PARAM,
    PDST_PARAM,
    ACCM_PARAM,
    NUM_PARAMS
  };
  enum InputIds
  {
    WAV0_INPUT,
    GATE_INPUT,
    RSET_INPUT,
    BPTS_INPUT,
    ASTP_INPUT,
    DSTP_INPUT,
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

  float phase = 1.0;
  float blinkPhase = 0.0;

  dsp::SchmittTrigger smpTrigger;
  dsp::SchmittTrigger gTrigger;
  dsp::SchmittTrigger g2Trigger;
  dsp::SchmittTrigger resetTrigger;

  float sample[MAX_SAMPLE_SIZE] = {0.f};
  float _sample[MAX_SAMPLE_SIZE] = {0.f};

  unsigned int channels;
  unsigned int sampleRate;

  unsigned int sample_length = MAX_SAMPLE_SIZE;

  unsigned int idx = 0;

  // spacing between breakpoints... in samples rn
  unsigned int bpt_spc = 1500;
  unsigned int env_dur = bpt_spc / 2;

  // number of breakpoints - to be calculated according to size of
  // the sample
  unsigned int num_bpts = MAX_SAMPLE_SIZE / bpt_spc;

  float mAmps[MAX_BPTS] = {0.f};
  float mDurs[MAX_BPTS] = {1.f};

  Wavetable env = Wavetable(TRI);

  unsigned int index = 0;

  float max_amp_step = 0.05f;
  float max_dur_step = 0.05f;

  float amp = 0.f;
  float amp_next = 0.f;
  float g_idx = 0.f;
  float g_idx_next = 0.5f;

  // when true read in from wav0_input and store in the sample buffer
  bool sampling = false;
  unsigned int s_i = 0;

  float bpts_sig = 1.f;
  float astp_sig = 1.f;
  float dstp_sig = 1.f;

  float astp = 1.f;
  float dstp = 1.f;

  bool is_mirroring = false;
  bool is_accumulating = false;

  gRandGen rg;
  DistType dt = LINEAR;

  ReGenEcho()
  {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

    configParam(SLEN_PARAM, 0.01f, 1.f, 0.f, "Length of sample", "%", 0, 100);
    configParam(BPTS_PARAM, 0, 2200, 0.0, "Breakpoints spacing");
    configParam(BPTSCV_PARAM, 0.f, 1.f, 0.0, "Breakpoints CV amount", "%", 0, 100);
    configParam(ASTP_PARAM, 0.0, 0.6, 0.9, "Maximum amplitude step");
    configParam(ASTPCV_PARAM, 0.f, 1.f, 0.f, "Amplitude step CV amount", "%", 0, 100);
    configParam(DSTP_PARAM, 0.0, 0.2, 0.9, "Maximum duration step");
    configParam(DSTPCV_PARAM, 0.f, 1.f, 0.f, "Duration step CV amount", "%", 0, 100);
    configParam(ENVS_PARAM, 1.f, 4.f, 4.f, "Envelope");
    configParam(ACCM_PARAM, 0.f, 1.f, 0.f, "Breakpoint accumulation");
    configParam(MIRR_PARAM, 0.f, 1.f, 0.f, "Mirror breakpoints");
    configParam(PDST_PARAM, 0.f, 2.f, 0.f, "Probability distribution");
  }

  void process(const ProcessArgs &args) override;
};

struct ReGenEchoWidget : ModuleWidget
{
  ReGenEchoWidget(ReGenEcho *module)
  {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/9HP_Blank.svg")));

    addChild(createWidget<ScrewSilver>(Vec(0, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 1 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    addParam(createParam<RoundSmallBlackKnob>(Vec(9.883, 25.49), module, ReGenEcho::SLEN_PARAM));

    addParam(createParam<RoundSmallBlackKnob>(Vec(9.883, 124.97), module, ReGenEcho::BPTS_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(55.883, 153.88), module, ReGenEcho::BPTSCV_PARAM));

    addParam(createParam<RoundSmallBlackKnob>(Vec(9.883, 193.54), module, ReGenEcho::ASTP_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(55.883, 193.54), module, ReGenEcho::ASTPCV_PARAM));

    addParam(createParam<RoundSmallBlackKnob>(Vec(9.883, 262.11), module, ReGenEcho::DSTP_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(55.883, 262.11), module, ReGenEcho::DSTPCV_PARAM));

    addParam(createParam<RoundBlackSnapKnob>(Vec(7.883, 329.25), module, ReGenEcho::ENVS_PARAM));

    // triggers for toggling mirroring and changing probability distribution
    addParam(createParam<CKSS>(Vec(60.789, 57.98), module, ReGenEcho::ACCM_PARAM));
    addParam(createParam<CKSS>(Vec(60.789, 88.69), module, ReGenEcho::MIRR_PARAM));
    addParam(createParam<CKSSThree>(Vec(60.789, 117.26), module, ReGenEcho::PDST_PARAM));

    addInput(createInput<PJ301MPort>(Vec(10.281, 54.79), module, ReGenEcho::WAV0_INPUT));
    addInput(createInput<PJ301MPort>(Vec(10.281, 80.54), module, ReGenEcho::GATE_INPUT));

    addInput(createInput<PJ301MPort>(Vec(58.281, 29.05), module, ReGenEcho::RSET_INPUT));

    addInput(createInput<PJ301MPort>(Vec(10.281, 154.01), module, ReGenEcho::BPTS_INPUT));
    addInput(createInput<PJ301MPort>(Vec(10.281, 221.72), module, ReGenEcho::ASTP_INPUT));
    addInput(createInput<PJ301MPort>(Vec(10.281, 291.00), module, ReGenEcho::DSTP_INPUT));

    addOutput(createOutput<PJ301MPort>(Vec(50.50, 332.46), module, ReGenEcho::SINE_OUTPUT));
  }
};
