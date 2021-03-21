#ifndef DRIVE_H
#define DRIVE_H

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
 * Overdrive Effect
 * 
 * SPST 1 - 
 * SPST 2 - 
 * SPST 3 - 
 * SPST 4 - 
 * 
 * SPDT 1 - 
 * SPDT 2 - 
 * 
 * Knob 1 - Mix
 * Knob 2 - Pregain
 * Knob 3 - Gain
 * Knob 4 - Drive
 * 
 * LED 1 - 
 * LED 2 - 
 * LED 3 - 
 * LED 4 - 
 **********************************************/

class Drive : public IEffect
{
public:
    void Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, unsigned long *avgTempo);
    void Cleanup();
    float Process(float in);
    void Loop(bool allowEffectControl);
    char *GetEffectName();
    char **GetKnobNames();
    EffectSettings GetEffectSettings();
    void SetEffectSettings(EffectSettings effectSettings);

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"PREGAIN", (char *)"GAIN", (char *)"DRIVE"};

    float WaveShape(float in, float k);

    DaisySeed *hw;
    float sample_rate;
    const int LED_MAX_VALUE = 256;
    const int LED_MIN_VALUE = 0;

    // Effect constants
    const float pregainLevelMin = 1.2f;
    const float pregainLevelMax = 5.2f;
    const float gainLevelMin = 1.2f;
    const float gainLevelMax = 101.2f;
    const float driveLevelMin = 1.0f;
    const float driveLevelMax = 25.0f;

    // Effect parameters
    float a = 0;
    float k = 0;
    float pregainLevel = 0.0f;
    float gainLevel = 0.0f;
    float mixLevel = 0.0f;
    float driveLevel = 1.0f;

    Knob pregainKnob;
    Knob gainKnob;
    Knob mixKnob;
    Knob driveKnob;

    Balance balance;
};

#endif