#ifndef TREMOLO_H
#define TREMOLO_H

#include "daisy_seed.h"
#include "daisysp.h"
#include "../../../include/IEffect.h"
#include "../../../include/PedalConfig.h"
#include "../Inputs/Knob.h"
#include "../Inputs/NFNToggle.h"

using namespace daisy;
using namespace daisysp;

/**********************************************
 * Tremolo Effect
 * 
 * SPST 1 - 
 * SPST 2 - 
 * SPST 3 - 
 * SPST 4 - 
 * 
 * SPDT 1 - Waveform selector (SIN, SQUARE, RAMP)
 * SPDT 2 - 
 * 
 * Knob 1 - Mix
 * Knob 2 - Rate
 * Knob 3 - Width
 * Knob 4 - 
 * 
 * LED 1 - 
 * LED 2 - 
 * LED 3 - 
 * LED 4 - 
 **********************************************/

class DaisyTremolo : public IEffect
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
    void UpdateToggleDisplay();

private:
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"RATE", (char *)"WIDTH", (char *)""};

    const float rateMin = 0.0f;
    const float rateMax = 20.0f;

    float mixLevel = 0.0f;
    float rate = 0.0f;
    float width = 0.0f;
    int waveform = Oscillator::WAVE_SIN;

    Knob mixKnob;
    Knob rateKnob;
    Knob widthKnob;

    NFNToggle waveformSelector;

    Tremolo tremolo;
};

#endif