#include "plugin.hpp"


struct Uplyt : Module {
	enum ParamId {
		BPMCONTROL_PARAM,
		SUBDIVIDESWITCH_PARAM,
		TUPLETCONTROL_PARAM,
		TUPLETCVCONTROL_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		BPMCVINPUT_INPUT,
		TUPLETCVINPUT_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		BPMOUTPUT_OUTPUT,
		SUBDIVIDEOUTPUT_OUTPUT,
		TUPLETOUTPUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	//float voltage = 0.f;

	float bpm = 120.f;

	int beatFrames = 0;  // Number of frames remaining for the current beat output signal
	int waitFrames = 0;  // Number of frames left until the next beat output signal starts

	Uplyt() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(BPMCONTROL_PARAM, 0.f, 720.f, 120.f, "");
		configParam(SUBDIVIDESWITCH_PARAM, 0.f, 1.f, 0.f, "");
		configParam(TUPLETCONTROL_PARAM, 0.f, 1.f, 0.f, "");
		configParam(TUPLETCVCONTROL_PARAM, 0.f, 1.f, 0.f, "");
		configInput(BPMCVINPUT_INPUT, "");
		configInput(TUPLETCVINPUT_INPUT, "");
		configOutput(BPMOUTPUT_OUTPUT, "");
		configOutput(SUBDIVIDEOUTPUT_OUTPUT, "");
		configOutput(TUPLETOUTPUT_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
		int defaultBeatFrames = 80;
		float voltage = 0.f;

		if (int(bpm) != int(params[BPMCONTROL_PARAM].getValue()))
		{
			// BPM Control changed
			bpm = params[BPMCONTROL_PARAM].getValue();
		}

		beatFrames--;
		waitFrames--;

		if (waitFrames <= 0)
		{
			beatFrames = defaultBeatFrames;
			waitFrames = int(args.sampleRate / (bpm / 60));
		}

		if (beatFrames > 0)
		{
			voltage = 10.f;
		}
			
		outputs[BPMOUTPUT_OUTPUT].setVoltage(voltage);
	}
};


struct UplytWidget : ModuleWidget {
	UplytWidget(Uplyt* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Uplyt.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(10.277, 9.706)), module, Uplyt::BPMCONTROL_PARAM));
		addParam(createParam<RoundBlackKnob>(mm2px(Vec(5.024, 50.016)), module, Uplyt::SUBDIVIDESWITCH_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(9.821, 75.024)), module, Uplyt::TUPLETCONTROL_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(5.595, 88.385)), module, Uplyt::TUPLETCVCONTROL_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(14.845, 16.444)), module, Uplyt::BPMCVINPUT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(13.931, 87.7)), module, Uplyt::TUPLETCVINPUT_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(10.962, 36.656)), module, Uplyt::BPMOUTPUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(9.935, 63.262)), module, Uplyt::SUBDIVIDEOUTPUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(9.364, 105.628)), module, Uplyt::TUPLETOUTPUT_OUTPUT));
	}
};


Model* modelUplyt = createModel<Uplyt, UplytWidget>("Uplyt");
