/*
 * LowPassFilter16.h
 *
 * Copyright 2012 Tim Barrass
 *
 * This file is part of Mozzi.
 *
 * Mozzi is licensed under a Creative Commons
 * Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 */

#ifndef LOWPASS16_H_
#define LOWPASS16_H_

/*
simple resonant filter posted to musicdsp.org by Paul Kellett
http://www.musicdsp.org/archive.php?classid=3#259

// set feedback amount given f and q between 0 and 1
fb = q + q/(1.0 - f);

// for each sample...
buf0 = buf0 + f * (in - buf0 + fb * (buf0 - buf1));
buf1 = buf1 + f * (buf0 - buf1);
out = buf1;

fixed point version of the filter
"dave's blog of art and programming" http://www.pawfal.org/dave/blog/2011/09/
*/

// we are using .n fixed point (n bits for the fractional part)
#define FX_SHIFT 8
#define SHIFTED_1 ((uint8_t)255)

/** A resonant low pass filter for audio signals.
 */
class LowPassFilter16
{

public:
  /** Constructor.
   */
  LowPassFilter16() { ; }

  /**
  Set the cut off frequency and resonance.  Replaces setCutoffFreq() and
  setResonance().  (Because the internal calculations need to be done whenever either parameter changes.)
  @param cutoff range 0-255 represents 0-8191 Hz (AUDIO_RATE/2).
  Be careful of distortion at the lower end, especially with high resonance.
  @param resonance range 0-255, 255 is most resonant.
  */
  void setCutoffFreqAndResonance(uint8_t cutoff, uint8_t resonance)
  {
    f = cutoff;
    fb = resonance + ucfxmul(resonance, SHIFTED_1 - cutoff);
  }

  /** Calculate the next sample, given an input signal.
  @param in the signal input.
  @return the signal output.
  @note Timing: about 11us.
  */
  //  10.5 to 12.5 us, mostly 10.5 us (was 14us)
  inline int16_t next(int16_t in)
  {
    buf0 += fxmul(((in - buf0) + fxmul(fb, buf0 - buf1)), f);
    buf1 += ifxmul(buf0 - buf1, f); // could overflow if input changes fast
    return buf1;
  }

private:
  uint16_t f;
  uint32_t fb;
  int32_t buf0, buf1;

  // multiply two fixed point numbers (returns fixed point)
  inline unsigned int ucfxmul(uint32_t a, uint16_t b)
  {
    return ((a * b) >> FX_SHIFT);
  }

  // multiply two fixed point numbers (returns fixed point)
  inline int16_t ifxmul(int32_t a, uint16_t b) { return ((a * b) >> FX_SHIFT); }

  // multiply two fixed point numbers (returns fixed point)
  inline int32_t fxmul(int32_t a, int16_t b) { return ((a * b) >> FX_SHIFT); }
};

#endif /* LOWPASS16_H_ */
