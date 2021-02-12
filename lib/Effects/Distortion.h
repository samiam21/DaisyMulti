#ifndef DISTORTION_H
#define DISTORTION_H

#include "daisy_seed.h"
#include "daisysp.h"
#include "../../include/IEffect.h"
#include "../../include/PedalConfig.h"
#include "../Inputs/Knob.h"
#include "../Inputs/Button.h"
#include "../Inputs/NFNToggle.h"

using namespace daisy;
using namespace daisysp;

/**********************************************
 * Distortion Effect
 * 
 * SPST 1 - 
 * SPST 2 - 
 * SPST 3 - 
 * SPST 4 - 
 * 
 * SPDT 1 - Clipping Level
 * SPDT 2 - 
 * 
 * Knob 1 - Pregain
 * Knob 2 - Gain
 * Knob 3 - Drive
 * Knob 4 - Mix
 * 
 * LED 1 - 
 * LED 2 - 
 * LED 3 - 
 * LED 4 - 
 **********************************************/

class Distortion : public IEffect
{
public:
    void Setup(daisy::DaisySeed *hardware);
    void Cleanup();
    float Process(float in);
    void Loop(bool allowEffectControl);
    char *GetEffectName();

private:
    float WaveShape(float in, float k);
    float HardClip(float in);
    void SetClipThreshold();

    DaisySeed *hw;
    float sample_rate;
    int currentClip = -1;
    const int LED_MAX_VALUE = 256;
    const int LED_MIN_VALUE = 0;

    // Effect constants
    const float pregainLevelMin = 1.2f;
    const float pregainLevelMax = 11.2f;
    const float gainLevelMin = 1.2f;
    const float gainLevelMax = 101.2f;
    const float driveLevelMin = 1.0f;
    const float driveLevelMax = 25.0f;
    const float clipThresholdHigh = 0.1f;
    const float clipThresholdMid = 2.0f;
    const float clipThresholdLow = 5.0f;

    // Effect parameters
    float pregainLevel = 0.0f;
    float gainLevel = 0.0f;
    float mixLevel = 0.0f;
    float driveLevel = 1.0f;
    float hardClipThreshold = 1.0f;

    Knob pregainKnob;
    Knob gainKnob;
    Knob mixKnob;
    Knob driveKnob;

    NFNToggle clippingToggle;

    Balance balance;
};

#endif