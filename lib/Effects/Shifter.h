#ifndef SHIFTER_H
#define SHIFTER_H

#include <cmath>
#include "daisy_seed.h"
#include "daisysp.h"
#include "../../../include/IEffect.h"
#include "../../../include/PedalConfig.h"
#include "../Inputs/Knob.h"

using namespace daisysp;

/**********************************************
 * Pitch Shifter Effect
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
 * Knob 2 - Transpose
 * Knob 3 - Delay
 * Knob 4 - Fun
 * 
 * LED 1 - 
 * LED 2 - 
 * LED 3 - 
 * LED 4 - 
 **********************************************/

class Shifter : public IEffect
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
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"TRANS", (char *)"DEL", (char *)"FUN"};

    uint32_t GetDelay();
    float GetTranspose();

    const float transMin = -12.0f;
    const float transMax = 24.0f;
    const float delayMin = SHIFT_BUFFER_SIZE / 2;
    const float delayMax = SHIFT_BUFFER_SIZE * 2;
    const float funMin = 0.0f;
    const float funMax = 10.0f;

    float mix = 1.0f;
    float transpose = 0.0f;
    float delay = 0.001f;
    float fun = 0.001f;

    Knob mixKnob;
    Knob transKnob;
    Knob delayKnob;
    Knob funKnob;

    PitchShifter shifter;
};

#endif