#ifndef REVERB_H
#define REVERB_H

#include "daisy_seed.h"
#include "daisysp.h"
#include "../../../include/IEffect.h"
#include "../../../include/PedalConfig.h"
#include "../Inputs/Knob.h"

using namespace daisysp;

/**********************************************
 * Reverb Effect
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
 * Knob 2 - Decay
 * Knob 3 - Tone
 * Knob 4 -
 *
 * LED 1 -
 * LED 2 -
 * LED 3 -
 * LED 4 -
 **********************************************/

static ReverbSc DSY_SDRAM_BSS verb;

class Reverb : public IEffect
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
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"DECAY", (char *)"TONE", (char *)""};

    const float feedbackMin = 0.4f;
    const float feedbackMax = 1.0f;
    const float lpfreqMin = 0.0f;

    float sample_rate;
    float feedback = 1.0f;
    float lpfreq = 0.0f;
    float mixLevel = 0.0f;

    Knob decayKnob;
    Knob toneKnob;
    Knob mixKnob;
};

#endif