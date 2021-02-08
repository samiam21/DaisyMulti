#ifndef KNOB_H
#define KNOB_H

#include "daisy_seed.h"
#include "../../include/PedalConfig.h"

using namespace daisy;

/**
 * Knob class to handle reading a knob value while accounting for jitter
 * This class will initialize the provided pin in the init function
 */
class Knob
{
public:
    /**
     * These functions will initialize the knob and set the initial value
     */
    void Init(DaisySeed *hardware, uint8_t chn, float &valueToSet);
    void Init(DaisySeed *hardware, uint8_t chn, float &valueToSet, float pMinValue, float pMaxValue);

    /**
     * This function will set a new value to the float passed in
     * If the knob value has not changed, or is within the jitter 
     * threshold, the value will not be changed.
     * @param valueToSet
     * @return Returns true if a new value was set, false if not
     */
    bool SetNewValue(float &valueToSet);

private:
    float GetNewValue(float newKnobReading);

    // Knob constants
    const float knobJitter = .01f;
    const float minKnobReadingValue = 0.0f;
    const float maxKnobReadingValue = 1.0f;

    // Class variables
    DaisySeed *hw;
    uint8_t knobChannel;
    float maxValue = 1.0f;
    float minValue = 0.0f;
    float knobReading = 0.0f;
};

#endif