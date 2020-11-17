/*
 * Voice.h
 * Version: 0.1.1
 * Since: 0.1.0
 */

#ifndef VOICE_H_
#define VOICE_H_

// Components
#include <Oscil.h>
#include <LowPassFilter.h>
#include <ADSR.h> // Line.h required

// Wavetables
#include <tables/sin1024_int8.h>
#include <tables/triangle1024_int8.h>
#include <tables/saw1024_int8.h>
#include "tables/square1024_int8.h"

class Voice {
public:

    virtual Voice() {}
    virtual ~Voice() {}

    void init() {
        setWaveform(0, true);
        oscillator.setPhase(0);
        oscillator.setFreq(440.f);

        lfo.setTable(SIN1024_DATA);
        lfo.setPhase(0);
        lfo.setFreq(lfoFrequency);
        lfo_amount = 0;

        envelope.setADLevels(ENV_1_ATTACK_LEVEL, ENV_DECAY_LEVEL);
        envelope.setTimes(ENV_1_ATTACK_MIN, ENV_DECAY_TIME, ENV_SUSTAIN_TIME, ENV_1_RELEASE_MIN);
        envelope_gain = 0;

        filter.setCutoffFreqAndResonance(255, 0);
    }

    void setWaveform(uint8_t table, bool force = false) {
        if (_table == table && !force) return;
        _table = table;

        switch (table) {
            case 0:
                oscillator.setTable(SIN1024_DATA);
                break;
            case 1:
                oscillator.setTable(TRIANGLE1024_DATA);
                break;
            case 2:
                oscillator.setTable(SAW1024_DATA);
                break;
            case 3:
                oscillator.setTable(SQUARE1024_DATA);
                break;
        }
    }

    void setVibratoIntensity(uint8_t amount) {
        lfo_amount = amount;
    }

    void setFilterFrequencyResonance(uint8_t frequency, uint8_t resonance, bool force = false) {
        if (_frequency == frequency && _resonance == resonance && !force) return;
        _frequency = frequency;
        _resonance = resonance;

        filter.setCutoffFreqAndResonance(frequency, resonance);
    }

    void setEnvelopeAttackRelease(uint16_t attackTime, uint16_t releaseTime, bool force = false) {
        if (_attackTime == attackTime && _releaseTime == releaseTime && !force) return;
        _attackTime = attackTime;
        _releaseTime = releaseTime;

        envelope.setTimes(attackTime, ENV_DECAY_TIME, ENV_SUSTAIN_TIME, releaseTime);
        envelope.update(); // Allows for mid-tranmition updates
    }

    void noteOn(uint8_t note) {
        oscillator.setFreq(mtof(float(note)));
        envelope.noteOn(false); // reset = true
    }
    void noteOff() {
        envelope.noteOff();
    }

    void update() {
        // Envelope
        envelope.update();
        envelope_gain = envelope.next();

        // Vibrato LFO
        lfo_current = (int16_t)lfo_amount * lfo.next();
    }

    uint8_t getCurrentLFO() {
        return (uint8_t)map(lfo_current, -32768, 32767, 0, 255);
    }
    uint8_t getCurrentGain() {
        return envelope_gain;
    }

    int16_t next() {
        // bit shift in and out to help keep filter happy
        return (int16_t)((int32_t)(envelope_gain * filter.next(oscillator.phMod((Q15n16)lfo_current))) >> 4); // 16-bit -> 14-bit & extra two bits to prevent distortion from filter
    }

private:

    Oscil <OSC_NUM_CELLS, AUDIO_RATE> oscillator;
    Oscil <LFO_NUM_CELLS, CONTROL_RATE> lfo;
    int16_t lfo_current;
    uint8_t lfo_amount;
    LowPassFilter filter;
    ADSR <CONTROL_RATE, CONTROL_RATE> envelope;
    uint8_t envelope_gain;

    // Setters
    uint8_t _table;
    uint8_t _frequency, _resonance;
    uint16_t _attackTime, _releaseTime;

};

#endif /* VOICE_H_ */
