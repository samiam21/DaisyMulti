#ifndef KNOBTEST_H
#define KNOBTEST_H

#include "../../include/IEffect.h"
#include "../../include/PedalConfig.h"
#include "../Inputs/Button.h"
#include "../Inputs/Knob.h"

/**********************************************
 * KnobTest Effect
 * 
 * SPST 1 - 
 * SPST 2 - 
 * SPST 3 - 
 * SPST 4 - 
 * 
 * SPDT 1 - 
 * SPDT 2 - 
 * 
 * Knob 1 - 
 * Knob 2 - 
 * Knob 3 - 
 * Knob 4 - 
 * 
 * LED 1 - 
 * LED 2 - 
 * LED 3 - 
 * LED 4 - 
 **********************************************/

class KnobTest : public IEffect
{
public:
    void Setup(daisy::DaisySeed *hardware);
    void Cleanup();
    void AudioCallback(float **in, float **out, size_t size);
    void Loop();
    char *GetEffectName();

private:
    size_t numChannels;
    const int LED_MAX_VALUE = 256;
    const int LED_MIN_VALUE = 0;

    const float minKnob3Value = -5.0f;
    const float maxKnob3Value = 10.0f;
    const float minKnob4Value = 0.0f;
    const float maxKnob4Value = 1.0f;

    bool isLed3On = false;
    float led1Value = 0.0f;
    float led2Value = 0.0f;
    float knob3Value = 0.0f;
    float knob4Value = 0.0f;

    Knob knob1;
    Knob knob2;
    Knob knob3;
    Knob knob4;

    Led led1;
    Led led2;
};

#endif