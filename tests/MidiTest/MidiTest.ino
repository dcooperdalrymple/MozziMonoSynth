/**
 * Title: MidiTest
 * Author: D Cooper Dalrymple
 * Created: 11/10/2020
 * Updated: 11/10/2020
 * Description: Simple Midi input test with LED indication for Mozzi Mono Synth.
 * https://dcooperdalrymple.com/
 */

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define LED_1 11
#define LED_2 6

void setup() {
  MIDI.setHandleNoteOn(receiveNoteOn);
  MIDI.setHandleNoteOff(receiveNoteOff);
  MIDI.setHandleControlChange(receiveControlChange);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
}

void loop() {
  MIDI.read();
}

void receiveNoteOn(byte channel, byte note, byte velocity) {
  analogWrite(LED_1, velocity);
}

void receiveNoteOff(byte channel, byte note, byte velocity) {
  digitalWrite(LED_1, LOW);
}

void receiveControlChange(byte channel, byte number, byte value) {
  analogWrite(LED_2, value);
}
