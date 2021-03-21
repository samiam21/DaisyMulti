#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "daisy_seed.h"
#include "daisysp.h"
#include "../../../include/IEffect.h"
#include "../../../include/PedalConfig.h"
#include "../Inputs/Knob.h"

using namespace daisysp;

/**********************************************
 * Compressor Effect
 * 
 * SPST 1 - 
 * SPST 2 - 
 * SPST 3 - 
 * SPST 4 - 
 * 
 * SPDT 1 - 
 * SPDT 2 - 
 * 
 * Knob 1 - Ratio
 * Knob 2 - Threshold
 * Knob 3 - Attack
 * Knob 4 - Release
 * 
 * LED 1 - 
 * LED 2 - 
 * LED 3 - 
 * LED 4 - 
 **********************************************/

class DaisyCompressor : public IEffect
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
    const char *knobNames[MAX_KNOBS] = {(char *)"RATIO", (char *)"THRESH", (char *)"ATTACK", (char *)"RELEASE"};

    const float ratioMin = 1.0f;
    const float ratioMax = 10.0f;
    const float thresholdMin = 0.0f;
    const float thresholdMax = -20.0f;
    const float attackMin = 0.001f;
    const float attackMax = 2.0f;
    const float releaseMin = 0.001f;
    const float releaseMax = 2.0f;

    float ratio = 1.0f;
    float threshold = 0.0f;
    float attack = 0.001f;
    float release = 0.001f;

    Knob ratioKnob;
    Knob thresholdKnob;
    Knob attackKnob;
    Knob releaseKnob;

    Compressor compressor;
};

#endif