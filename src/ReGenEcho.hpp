#pragma once

#include "plugin.hpp"
#include "dsp/digital.hpp"
#include "dsp/resampler.hpp"
#include "utils/wavetable.hpp"
#include "utils/Limiter.hpp"

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

  float phase = 1.0f;
  float blinkPhase = 0.0f;

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

  // spacing between breakpoints... in samples
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

  AudioLimiter limiter;

  ReGenEcho()
  {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

    configParam(SLEN_PARAM, 0.01f, 1.f, 1.0f, "Sample Length");
    configParam(BPTS_PARAM, 0, 2200, 800.0, "Breakpoint Spacing");
    configParam(BPTSCV_PARAM, 0.f, 1.f, 0.0, "Breakpoint Spacing CV Amount");
    configParam(ASTP_PARAM, 0.0, 0.6, 0.2, "Maximum Amplitude Step");
    configParam(ASTPCV_PARAM, 0.f, 1.f, 0.f, "Amplitude Step CV Amount");
    configParam(DSTP_PARAM, 0.0, 0.2, 0.1, "Maximum Duration Step");
    configParam(DSTPCV_PARAM, 0.f, 1.f, 0.f, "Duration Step CV Amount");
    configParam(ENVS_PARAM, 1.f, 4.f, 2.f, "Envelope Type");
    configParam(ACCM_PARAM, 0.f, 1.f, 0.f, "Accumulate Mode");
    configParam(MIRR_PARAM, 0.f, 1.f, 0.f, "Mirror Mode");
    configParam(PDST_PARAM, 0.f, 2.f, 0.f, "Probability Distribution");

    configInput(WAV0_INPUT, "Audio Input");
    configInput(GATE_INPUT, "Gate/Trigger");
    configInput(RSET_INPUT, "Reset");
    configInput(BPTS_INPUT, "Breakpoint Spacing CV");
    configInput(ASTP_INPUT, "Amplitude Step CV");
    configInput(DSTP_INPUT, "Duration Step CV");

    configOutput(SINE_OUTPUT, "Audio Output");

    // Initialize limiter with default sample rate
    limiter.init(APP->engine->getSampleRate());
  }

  void process(const ProcessArgs &args) override;

  void onSampleRateChange() override
  {
    limiter.init(APP->engine->getSampleRate());
  }
};

struct ReGenEchoWidget : ModuleWidget
{
  ReGenEchoWidget(ReGenEcho *module)
  {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ReGenEcho-panel.svg")));

    addChild(createWidget<ScrewSilver>(Vec(0, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 1 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    // Sample Length
    addParam(createParam<RoundSmallBlackKnob>(Vec(9.883, 40.49), module, ReGenEcho::SLEN_PARAM));

    // Breakpoint Spacing
    addParam(createParam<RoundSmallBlackKnob>(Vec(9.883, 139.97), module, ReGenEcho::BPTS_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(55.883, 168.88), module, ReGenEcho::BPTSCV_PARAM));
    
    // Amplitude Step
    addParam(createParam<RoundSmallBlackKnob>(Vec(9.883, 208.54), module, ReGenEcho::ASTP_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(55.883, 208.54), module, ReGenEcho::ASTPCV_PARAM));
    
    // Duration Step
    addParam(createParam<RoundSmallBlackKnob>(Vec(9.883, 277.11), module, ReGenEcho::DSTP_PARAM));
    addParam(createParam<RoundSmallBlackKnob>(Vec(55.883, 277.11), module, ReGenEcho::DSTPCV_PARAM));
    
    // Envelope Type
    addParam(createParam<RoundBlackSnapKnob>(Vec(7.883, 344.25), module, ReGenEcho::ENVS_PARAM));

    // Switches for toggling accumulate, mirroring and changing probability distribution
    addParam(createParam<CKSS>(Vec(60.789, 72.98), module, ReGenEcho::ACCM_PARAM)); 
    addParam(createParam<CKSS>(Vec(60.789, 103.69), module, ReGenEcho::MIRR_PARAM)); 
    addParam(createParam<CKSSThree>(Vec(60.789, 132.26), module, ReGenEcho::PDST_PARAM)); 
    
    // Inputs
    addInput(createInput<PJ301MPort>(Vec(10.281, 69.79), module, ReGenEcho::WAV0_INPUT));
    addInput(createInput<PJ301MPort>(Vec(10.281, 95.54), module, ReGenEcho::GATE_INPUT));
    addInput(createInput<PJ301MPort>(Vec(58.281, 44.05), module, ReGenEcho::RSET_INPUT));
    addInput(createInput<PJ301MPort>(Vec(10.281, 169.01), module, ReGenEcho::BPTS_INPUT));
    addInput(createInput<PJ301MPort>(Vec(10.281, 236.72), module, ReGenEcho::ASTP_INPUT));
    addInput(createInput<PJ301MPort>(Vec(10.281, 306.00), module, ReGenEcho::DSTP_INPUT));

    // Output
    addOutput(createOutput<PJ301MPort>(Vec(50.50, 347.46), module, ReGenEcho::SINE_OUTPUT));
  }
};
