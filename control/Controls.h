/*
 * Controls.h
 */

#ifndef CONTROLS_H_
#define CONTROLS_H_

#include "Pot.h"
#include "Button.h"
#include "Led.h"

typedef void (*ProgramFunctionPointer)(uint8_t);
typedef void (*UpdateFunctionPointer)(uint8_t, uint16_t);

class Controls {

private:

    static Pot pots[CONTROLS_NUM_POTS];

    static LedB led1;
    static LedD led2;
    static Led *leds[CONTROLS_NUM_LEDS] = {
        &led1,
        &led2
    };

    static Button buttons[CONTROLS_NUM_BUTTONS];

    static uint8_t i, j;
    static UpdateFunctionPointer _updateFunctionPointer;
    static bool _updated;
    static uint8_t currentState;

    // Variables for programming
    static uint8_t selectedProgram;
    static ProgramFunctionPointer _writeProgramPointer;
    static ProgramFunctionPointer _readProgramPointer;

public:

    static void setup() {
        // Primary
        Controls::pots[OSC_1_WAVEFORM_KEY] = Pot(OSC_1_WAVEFORM_PIN);
        Controls::pots[LFO_AMOUNT_KEY] = Pot(LFO_AMOUNT_PIN);
        Controls::pots[LPF_FREQUENCY_KEY] = Pot(LPF_FREQUENCY_PIN);
        Controls::pots[LPF_RESONANCE_KEY] = Pot(LPF_RESONANCE_PIN);
        Controls::pots[ENV_1_ATTACK_KEY] = Pot(ENV_1_ATTACK_PIN);
        Controls::pots[ENV_1_RELEASE_KEY] = Pot(ENV_1_RELEASE_PIN);
        // Secondary
        Controls::pots[OSC_2_WAVEFORM_KEY] = Pot(OSC_2_WAVEFORM_PIN);
        Controls::pots[LFO_FREQUENCY_KEY] = Pot(LFO_FREQUENCY_PIN);
        Controls::pots[ENV_2_AMOUNT_KEY] = Pot(ENV_2_AMOUNT_PIN);
        Controls::pots[ENV_2_LEVEL_KEY] = Pot(ENV_2_LEVEL_PIN);
        Controls::pots[ENV_2_ATTACK_KEY] = Pot(ENV_2_ATTACK_PIN);
        Controls::pots[ENV_2_RELEASE_KEY] = Pot(ENV_2_RELEASE_PIN);
        // Tertiary
        Controls::pots[OSC_DETUNE_KEY] = Pot(OSC_DETUNE_PIN);
        Controls::pots[LFO_DESTINATION_KEY] = Pot(LFO_DESTINATION_PIN);
        Controls::pots[OSC_MIX_KEY] = Pot(OSC_MIX_PIN);
        Controls::pots[OSC_TUNE_KEY] = Pot(OSC_TUNE_PIN);
        Controls::pots[ENV_1_DECAY_KEY] = Pot(ENV_1_DECAY_PIN);
        Controls::pots[ENV_2_DECAY_KEY] = Pot(ENV_2_DECAY_PIN);

        Controls::pots[OSC_1_WAVEFORM_KEY].setMapPointer(Controls::mapOscWaveform);
        //Controls::pots[LFO_AMOUNT_KEY].setMapPointer(Controls::mapLfoGain);
        Controls::pots[LFO_AMOUNT_KEY].setMinMax(LFO_AMOUNT_MIN, LFO_AMOUNT_MAX);

        Controls::pots[LPF_FREQUENCY_KEY].setMinMax(LPF_FREQUENCY_MIN, LPF_FREQUENCY_MAX);
        Controls::pots[LPF_RESONANCE_KEY].setMinMax(LPF_RESONANCE_MIN, LPF_RESONANCE_MAX);

        Controls::pots[ENV_1_ATTACK_KEY].setMinMax(ENV_1_ATTACK_MIN, ENV_1_ATTACK_MAX);
        Controls::pots[ENV_1_RELEASE_KEY].setMinMax(ENV_1_RELEASE_MIN, ENV_1_RELEASE_MAX);

        Controls::leds[LED_1_KEY]->setPin(LED_1_PIN);
        Controls::leds[LED_2_KEY]->setPin(LED_2_PIN);
        for (i = 0; i < CONTROLS_NUM_LEDS; i++) {
            Controls::leds[i]->set(0);
        }

        Controls::buttons[BUTTON_1_KEY] = Button(BUTTON_1_PIN);
        Controls::buttons[BUTTON_1_KEY].setDownHandler(Controls::setSecondaryState);
        Controls::buttons[BUTTON_1_KEY].setUpHandler(Controls::setPrimaryState);
        Controls::buttons[BUTTON_1_KEY].setPressHandler(Controls::setProgramState);

        Controls::buttons[BUTTON_2_KEY] = Button(BUTTON_2_PIN);
        Controls::buttons[BUTTON_2_KEY].setDownHandler(Controls::setTertiaryState);
        Controls::buttons[BUTTON_2_KEY].setUpHandler(Controls::setPrimaryState);
        Controls::buttons[BUTTON_2_KEY].setPressHandler(Controls::setProgramState);

        Controls::selectedProgram = 0;
        Controls::setState(STATE_PRIMARY, true, true);
    }

    static bool update() {
        for (i = 0; i < CONTROLS_NUM_BUTTONS; i++) {
            buttons[i].update();
        }

        if (currentState > CONTROLS_NUM_STATES) return; // Check if we're in program mode

        _updated = false;
        for (i = STATE_NUM_POTS * currentState; i < STATE_NUM_POTS * (currentState + 1); i++) {
            if (!pots[i].read()) continue;
            _updated = true;
            if (_updateFunctionPointer) _updateFunctionPointer(i, pots[i].get());
        }

        return _updated;
    }
    static bool isUpdated(uint8_t key) {
        return pots[key].isUpdated();
    }
    static void setUpdateHandler(UpdateFunctionPointer updateFunctionPointer) {
        _updateFunctionPointer = updateFunctionPointer;
    }

    static uint16_t getPot(uint8_t key, bool mapped = true) {
        return pots[key].get(mapped);
    }
    static void setPot(uint8_t key, uint16_t value, bool lock = true) {
        pots[key].set(value);
        if (!!lock) pots[key].lock();
    }

    static void setLed(uint8_t key, uint8_t value) {
        leds[key]->set(value);
    }
    static void updateLeds() {
        leds[LED_1_KEY]->update();
        leds[LED_2_KEY]->update();
    }

    static uint16_t mapOscWaveform(uint16_t value) {
        if (value > 767) {
            return 3;
        } else if (value < 256) {
            return 0;
        } else if (value > 511) {
            return 2;
        } else { // value > 255
            return 1;
        }
    }

    static uint16_t mapLfoGain(uint16_t value) {
        if (value <= LFO_AMOUNT_GAP) return 0;
        return map(value - LFO_AMOUNT_GAP, POT_MIN, POT_MAX - LFO_AMOUNT_GAP, LFO_AMOUNT_MIN, LFO_AMOUNT_MAX);
    }

    static void setPrimaryState() {
        if (currentState == STATE_PROGRAM)  {
            // Check that both buttons are pressed
            for (i = 0; i < CONTROLS_NUM_BUTTONS; i++) {
                if (!buttons[i].isPressed()) return;
            }
            if (_readProgramPointer) _readProgramPointer(selectedProgram);
        }
        setState(STATE_PRIMARY);
    }
    static void setSecondaryState() {
        setState(STATE_SECONDARY);
    }
    static void setTertiaryState() {
        setState(STATE_TERTIARY);
    }

    static void setProgramState() {
        if (currentState == STATE_PROGRAM) {
            // Change selected program
            if (buttons[BUTTON_1_KEY].isPressed() && !buttons[BUTTON_2_KEY].isPressed()) {
                if (selectedProgram > 0) selectedProgram--;
            } else if (!buttons[BUTTON_1_KEY].isPressed() && buttons[BUTTON_2_KEY].isPressed()) {
                if (selectedProgram < PROGRAM_COUNT - 1) selectedProgram++;
            }
            return;
        }

        // Check if all buttons are pressed
        for (i = 0; i < CONTROLS_NUM_BUTTONS; i++) {
            if (!buttons[i].isPressed()) return;
        }

        setState(STATE_PROGRAM, false);
    }
    static void checkWriteProgram() {
        if (currentState != STATE_PROGRAM) return;

        // Check if all buttons long pressed
        for (i = 0; i < CONTROLS_NUM_BUTTONS; i++) {
            if (!buttons[i].isLongPressed()) return;
        }

        if (_writeProgramPointer) _writeProgramPointer(selectedProgram);
    }
    static uint8_t getSelectedProgram() {
        return selectedProgram;
    }
    static void setWriteProgramCallback(ProgramFunctionPointer writeProgramPointer) {
        _writeProgramPointer = writeProgramPointer;
    }
    static void setReadProgramCallback(ProgramFunctionPointer readProgramPointer) {
        _readProgramPointer = readProgramPointer;
    }

    static bool setState(uint8_t state, bool unlock = false, bool force = false) {
        if (!force && Controls::currentState == state) return false;
        Controls::currentState = state;

        // Lock all pots
        for (i = 0; i < CONTROLS_NUM_POTS; i++) {
            Controls::pots[i].lock();
        }

        // Only unlock those in current state
        if (!!unlock) {
            for (i = STATE_NUM_POTS * Controls::currentState; i < STATE_NUM_POTS * (Controls::currentState + 1); i++) {
                Controls::pots[i].unlock();
            }
        }
    }
    static uint8_t getState() {
        return Controls::currentState;
    }

};

#endif /* CONTROLS_H_ */
