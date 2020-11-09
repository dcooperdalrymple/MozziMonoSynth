/**
 * Title: MonoSynth
 * Author: D Cooper Dalrymple
 * Created: 11/04/2020
 * Updated: 11/04/2020
 * Description: 14-bit monophonic synthesizer using the Mozzi audio synthesis library. Best used for bass voices. Uses HiFi mode on Arduino Uno with hex inverter buffer and hardware low pass filter.
 * https://dcooperdalrymple.com/
 */

// Constants

/// Hardware

#define STATUS_LED 13

#define OSC_OCTAVE_DEC_BUTTON 2
#define OSC_OCTAVE_INC_BUTTON 4
#define OSC_OCTAVE_LED 3

#define OSC_WAVEFORM_POT A0
#define OSC_VIBRATO_POT A1
#define LPF_FREQUENCY_POT A2
#define LPF_RESONANCE_POT A3
#define ENV_ATTACK_POT A4
#define ENV_RELEASE_POT A5

/// Mozzi

#define CONTROL_RATE 128 // Defaults to 64
#define TABLE_NUM_CELLS SIN1024X16_NUM_CELLS

/// Oscillator

#define OSC_WAVEFORM_NUM 4
#define OSC_VIBRATO_FREQUENCY 15
#define OSC_VIBRATO_MIN 0
#define OSC_VIBRATO_MAX 255

/// Envelope

#define ENV_ATTACK_LEVEL 255
#define ENV_DECAY_LEVEL 255
#define ENV_DECAY_TIME 50
#define ENV_SUSTAIN_TIME 60000
#define ENV_ATTACK_MIN 50
#define ENV_ATTACK_MAX 10000
#define ENV_RELEASE_MIN 50
#define ENV_RELEASE_MAX 20000

// Libraries

#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

#include <MozziGuts.h>
#include <mozzi_fixmath.h>
#include <mozzi_midi.h>
#include <mozzi_analog.h>

#include "Oscil16.h"
#include <LowPassFilter.h>
#include <Line.h>
#include "ADSR16.h"

#include "tables/sin1024_int16.h"
#include "tables/tri1024_int16.h"
#include "tables/saw1024_int16.h"
#include "tables/sqr1024_int16.h"

#include "Voice.h"

// Local Interfaces

// Synthesis Objects

Oscil16 <TABLE_NUM_CELLS, AUDIO_RATE> oscillator;
Oscil16 <TABLE_NUM_CELLS, CONTROL_RATE> lfo(SIN1024X16_NUM_CELLS); // Vibrato LFO
LowPassFilter lpf;
ADSR16 <CONTROL_RATE, CONTROL_RATE> envelope;
int16_t envelope_gain;

// Settings Objects

Voice voice;

void setup() {
  MIDI.setHandleNoteOn(receiveNoteOn);
  MIDI.setHandleNoteOff(receiveNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);

  pinMode(STATUS_LED, OUTPUT);

  setWave(0);
  lfo.setFreq(OSC_VIBRATO_FREQUENCY);

  envelope.setADLevels(ENV_ATTACK_LEVEL, ENV_DECAY_LEVEL);
  envelope.setTimes(ENV_ATTACK_MIN, ENV_DECAY_TIME, ENV_SUSTAIN_TIME, ENV_RELEASE_MIN);
  envelope_gain = 0;

  lpf.setCutoffFreq(255);
  lpf.setResonance(0);

  startMozzi(CONTROL_RATE);
}

void loop() {
  audioHook();
}

void setWave(uint8_t table) {
  switch (table) {
    case 0:
      oscillator.setTable(SIN1024X16_DATA);
      break;
    case 1:
      oscillator.setTable(TRI1024X16_DATA);
      break;
    case 2:
      oscillator.setTable(SAW1024X16_DATA);
      break;
    case 3:
      oscillator.setTable(SQR1024X16_DATA);
      break;
  }
}

bool updated;
void updateControl() {
  MIDI.read();

  // Update Oscillator
  if (voice.setOscWaveform(mozziAnalogRead(OSC_WAVEFORM_POT))) setWave(voice.getOscWaveform());

  // Update Oscillator Pitch LFO (Vibrator)
  voice.setOscVibrato(mozziAnalogRead(OSC_VIBRATO_POT));

  // Update Low Pass Filter
  updated = voice.setLpfFrequency(mozziAnalogRead(LPF_FREQUENCY_POT));
  updated = voice.setLpfResonance(mozziAnalogRead(LPF_RESONANCE_POT)) || updated;
  if (updated) {
    lpf.setCutoffFreq(voice.getLpfFrequency());
    lpf.setResonance(voice.getLpfResonance());
  }

  // Update Envelope
  updated = voice.setEnvAttack(mozziAnalogRead(ENV_ATTACK_POT));
  updated = voice.setEnvRelease(mozziAnalogRead(ENV_RELEASE_POT)) || updated;
  if (updated) envelope.setTimes(voice.getEnvAttack(), ENV_DECAY_TIME, ENV_SUSTAIN_TIME, voice.getEnvRelease());

  envelope.update();
  envelope_gain = envelope.next();
}

int16_t updateAudio() {
  if (voice.getOscVibrato() != 0) {
    Q15n16 vibrato = (Q15n16)voice.getOscVibrato() * lfo.next();
    return (int16_t)(((int32_t)envelope_gain * lpf.next(oscillator.phMod(vibrato))) >> 18);
  } else {
    return (int16_t)(((int32_t)envelope_gain * lpf.next(oscillator.next())) >> 18);
  }
}

uint8_t currentNote = 255;
void receiveNoteOn(byte channel, byte note, byte velocity) {
  currentNote = note;
  digitalWrite(STATUS_LED, HIGH);

  oscillator.setFreq(mtof(float(note)));
  envelope.noteOn();
}
void receiveNoteOff(byte channel, byte note, byte velocity) {
  if (currentNote != note) return;
  digitalWrite(STATUS_LED, LOW);

  envelope.noteOff();
}
