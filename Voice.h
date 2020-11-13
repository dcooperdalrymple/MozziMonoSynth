/*
 * Voice.h
 *
 */

#ifndef VOICE_H_
#define VOICE_H_

// Components
#include <Oscil.h>
#include <LowPassFilter.h>
#include <ADSR.h> // Line.h required

// Wavetables
#include <tables/sin512_int8.h>
#include <tables/triangle512_int8.h>
#include <tables/saw_analogue512_int8.h>
#include <tables/square_analogue512_int8.h>

/// Oscillator

#define OSC_NUM_CELLS SIN512_NUM_CELLS
#define OSC_WAVEFORM_NUM 4
const float oscVibratoFrequency = 2.5f;
#define OSC_VIBRATO_GAP 20
#define OSC_VIBRATO_MIN 0
#define OSC_VIBRATO_MAX 255

/// Low Pass Filter

#define LPF_FREQUENCY_MIN 20
#define LPF_FREQUENCY_MAX 255
#define LPF_RESONANCE_MIN 0
#define LPF_RESONANCE_MAX 170

/// Envelope

#define ENV_ATTACK_LEVEL 255
#define ENV_DECAY_LEVEL 255
#define ENV_DECAY_TIME 50
#define ENV_SUSTAIN_TIME 60000
#define ENV_ATTACK_MIN 20
#define ENV_ATTACK_MAX 2000
#define ENV_RELEASE_MIN 20
#define ENV_RELEASE_MAX 4000

class Voice {
public:

    Voice() {
        setWaveform(0);

        lfo.setTable(SIN512_DATA);
        lfo.setFreq(oscVibratoFrequency);
        lfo_amount = 0;

        envelope.setADLevels(ENV_ATTACK_LEVEL, ENV_DECAY_LEVEL);
        envelope.setTimes(ENV_ATTACK_MIN, ENV_DECAY_TIME, ENV_SUSTAIN_TIME, ENV_RELEASE_MIN);
        envelope_gain = 0;

        filter.setCutoffFreqAndResonance(255, 0);
    }
    virtual ~Voice() {}

    void setWaveform(uint8_t table) {
        switch (table) {
            case 0:
                oscillator.setTable(SIN512_DATA);
                break;
            case 1:
                oscillator.setTable(TRIANGLE512_DATA);
                break;
            case 2:
                oscillator.setTable(SAW_ANALOGUE512_DATA);
                break;
            case 3:
                oscillator.setTable(SQUARE_ANALOGUE512_DATA);
                break;
        }
    }

    void setVibratoIntensity(uint8_t amount) {
        lfo_amount = amount;
    }

    void setFilterFrequencyResonance(uint8_t frequency, uint8_t resonance) {
        filter.setCutoffFreqAndResonance(frequency, resonance);
    }

    void setEnvelopeAttackRelease(uint16_t attackTime, uint16_t releaseTime) {
        envelope.setTimes(attackTime, ENV_DECAY_TIME, ENV_SUSTAIN_TIME, releaseTime);
    }

    void noteOn(uint8_t note) {
        oscillator.setFreq(mtof(float(note)));
        envelope.noteOn();
    }
    void noteOff() {
        envelope.noteOff();
    }

    void update() {
        envelope.update();
        envelope_gain = envelope.next();
    }

    uint8_t getCurrentLFO() {
        return Q15n16_to_Q8n0(lfo_current);
    }
    uint8_t getCurrentGain() {
        return envelope_gain;
    }

    int16_t next() {
        //return (int16_t)(envelope_gain * oscillator.next()) >> 2; // No Vibrato or LPF
        //return (int16_t)(envelope_gain * filter.next(oscillator.next())); // No Vibrato
        lfo_current = (Q15n16)lfo_amount * lfo.next();
        return (int16_t)(envelope_gain * filter.next(oscillator.phMod(lfo_current)));
    }

private:

    Oscil <OSC_NUM_CELLS, AUDIO_RATE> oscillator;
    Oscil <OSC_NUM_CELLS, CONTROL_RATE> lfo;
    uint8_t lfo_amount;
    Q15n16 lfo_current;
    LowPassFilter filter;
    ADSR <CONTROL_RATE, CONTROL_RATE> envelope;
    uint8_t envelope_gain;

};

#endif /* VOICE_H_ */
