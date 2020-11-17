/*
 * Controls.h
 * Version: 0.1.1
 * Since: 0.1.0
 */

#ifndef CONTROLS_H_
#define CONTROLS_H_

#include "control/Pot.h"
#include "control/Button.h"
#include "control/Led.h"

typedef void (*ProgramFunctionPointer)(uint8_t);
typedef void (*UpdateFunctionPointer)(uint8_t, uint16_t);

class Controls {

public:

    virtual Controls() {}
    virtual ~Controls() {}

    static void setup();

    static bool update();
    static bool isUpdated(uint8_t key);
    static void setUpdateHandler(UpdateFunctionPointer updateFunctionPointer);

    static uint16_t getPot(uint8_t key, bool mapped = true);
    static void setPot(uint8_t key, uint16_t value, bool lock = true);

    static void setLed(uint8_t key, uint8_t value);
    static void updateLeds();

    static uint16_t mapOscWaveform(uint16_t value);
    static uint16_t mapLfoGain(uint16_t value);

    static void setPrimaryState();
    static void setSecondaryState();
    static void setTertiaryState();

    static void setProgramState();
    static void checkWriteProgram();
    static uint8_t getSelectedProgram();
    static void setWriteProgramCallback(ProgramFunctionPointer writeProgramPointer);
    static void setReadProgramCallback(ProgramFunctionPointer readProgramPointer);

    static bool setState(uint8_t state, bool unlock = false, bool force = false);
    static uint8_t getState();

private:

    static Pot pots[CONTROLS_NUM_POTS];

    static LedB led1;
    static LedD led2;
    static Led *leds[CONTROLS_NUM_LEDS] = {
        &led1,
        &led2
    };

    static Button buttons[CONTROLS_NUM_BUTTONS];

    static UpdateFunctionPointer _updateFunctionPointer;
    static bool _updated;
    static uint8_t currentState;

    // Variables for programming
    static uint8_t selectedProgram;
    static ProgramFunctionPointer _writeProgramPointer;
    static ProgramFunctionPointer _readProgramPointer;

};

#endif /* CONTROLS_H_ */
