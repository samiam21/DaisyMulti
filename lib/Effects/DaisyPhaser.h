#ifndef PHASER_H
#define PHASER_H

#include "daisy_seed.h"
#include "daisysp.h"
#include "../../../include/IEffect.h"
#include "../../../include/PedalConfig.h"
#include "../Inputs/Knob.h"

using namespace daisy;
using namespace daisysp;

/**********************************************
 * Phaser Effect
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
 * Knob 2 - LFO Rate
 * Knob 3 - Width
 * Knob 4 - Feedback
 * 
 * LED 1 - 
 * LED 2 - 
 * LED 3 - 
 * LED 4 - 
 **********************************************/

class DaisyPhaser : public IEffect
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
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"DEPTH", (char *)"FREQ", (char *)"FEEDBK"};

    const float frequencyMin = 0.0f;
    const float frequencyMax = 1000.0f;

    float mixLevel = 0.0f;
    float depth = 0.0f;
    float frequency = 0.0f;
    float feedback = 0.0f;

    Knob mixKnob;
    Knob depthKnob;
    Knob frequencyKnob;
    Knob feedbackKnob;

    Phaser phaser;
};

#endif