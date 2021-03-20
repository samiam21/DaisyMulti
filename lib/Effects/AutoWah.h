#ifndef AUTOWAH_H
#define AUTOWAH_H

#include "daisy_seed.h"
#include "daisysp.h"
#include "../../include/IEffect.h"
#include "../../include/PedalConfig.h"
#include "../Inputs/Knob.h"

using namespace daisy;
using namespace daisysp;

/**********************************************
 * AutoWah Effect
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
 * Knob 2 - Wah
 * Knob 3 - Level
 * Knob 4 - 
 * 
 * LED 1 - 
 * LED 2 - 
 * LED 3 - 
 * LED 4 - 
 **********************************************/

class AutoWah : public IEffect
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
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"WAH", (char *)"LEVEL", (char *)""};

    DaisySeed *hw;

    // Effect parameters
    float mixLevel = 0.0f;
    float wahLevel = 0.0f;
    float level = 0.0f;

    Autowah autowah;

    Knob mixLevelKnob;
    Knob wahLevelKnob;
    Knob levelKnob;
};

#endif