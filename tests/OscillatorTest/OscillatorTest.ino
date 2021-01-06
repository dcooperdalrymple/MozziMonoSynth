/**
 * Title: ControlsTest
 * Author: D Cooper Dalrymple
 * Created: 01/06/2021
 * Updated: 01/06/2021
 * Description: Tests two oscillators outputting in stereo. MCP4131-103 used as SPI digital pot to mix between both oscillators using lfo. This requires `#define STEREO_HACK true` in mozzi_config.h.
 * https://dcooperdalrymple.com/
 */

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/saw2048_int8.h>
#include <tables/triangle2048_int8.h>
#include <mozzi_midi.h>
#include <mozzi_fixmath.h>
#include <SPI.h>

#define CONTROL_RATE 64 // tested up to 256

Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> osc1(SAW2048_DATA);
Oscil<TRIANGLE2048_NUM_CELLS, AUDIO_RATE> osc2(TRIANGLE2048_DATA);
Oscil<TRIANGLE2048_NUM_CELLS, CONTROL_RATE> lfo(TRIANGLE2048_DATA);

#define NUM_NOTES 8
float melody[NUM_NOTES] = { 39.f, 42.f, 44.f, 42.f, 51.f, 47.f, 51.f, 52.f }; // On the run
uint8_t index = 0;
uint8_t timer = CONTROL_RATE >> 3;

#define SPI_ADDRESS 0x00
#define SPI_CS 6

void setup() {
    pinMode(SPI_CS, OUTPUT);
    SPI.begin();

    startMozzi(CONTROL_RATE);

    lfo.setFreq(0.1f);
}

void updateControl() {
    if (++timer > CONTROL_RATE >> 3) {
        timer = 0;
        osc1.setFreq(mtof(melody[index]));
        osc2.setFreq(mtof(melody[index]) * 1.01f);
        index++;
        if (index >= NUM_NOTES) index = 0;
    }

    // Update digital spi pot for oscillator mix
    digitalPotWrite((int16_t)(lfo.next() + 128) >> 1);
}

// Needed for stereo output
int audio_out_1, audio_out_2;

void updateAudio() {
    audio_out_1 = osc1.next();
    audio_out_2 = osc2.next();
}

void loop() {
    audioHook();
}

int digitalPotWrite(int16_t value) {
    digitalWrite(SPI_CS, LOW);
    SPI.transfer(SPI_ADDRESS);
    SPI.transfer(value);
    digitalWrite(SPI_CS, HIGH);
}
