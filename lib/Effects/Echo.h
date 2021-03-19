#ifndef ECHO_H
#define ECHO_H

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
 * Echo Effect
 * 
 * SPST 1 - 
 * SPST 2 - 
 * SPST 3 - 
 * SPST 4 - 
 * 
 * SPDT 1 - Type Switcher
 * SPDT 2 - 
 * 
 * Knob 1 - Mix
 * Knob 2 - Decay
 * Knob 3 - Speed
 * Knob 4 - 
 * 
 * LED 1 - 
 * LED 2 - 
 * LED 3 - 
 * LED 4 - 
 **********************************************/

// Type constants
enum DelayType
{
    QUARTER = 0,
    DOTTED_EIGHTH = 1,
    TRIPLET = 2,

    DT_UNSET = 99
};

class Echo : public IEffect
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
    const char *knobNames[MAX_KNOBS] = {(char *)"MIX", (char *)"DECAY", (char *)"SPEED", (char *)""};

    void TapTempoInterruptHandler();
    void TypeSwitcherLoopControl();

    DaisySeed *hw;

    // Input handlers
    NFNToggle typeSwitcher;
    Knob mixLevelKnob;
    Knob decayKnob;
    Knob speedKnob;
    //Button tapTempoButton;

    static const size_t delayMaxSize = 48000;
    static const size_t initialTempoBpm = 90;

    // Decay constants
    const float minDecayValue = 0.0f;
    const float maxDecayValue = 0.75f;

    // Level constants
    const float mixLevelMin = 0.0f;
    const float mixLevelMax = 1.0f;

    // Volume boost constants
    const float speedMin = 30.0f;
    const float speedMax = 180.0f;

    // Mutable parameters
    DelayLine<float, delayMaxSize> del_line;
    float decayValue = 0.5f;
    float mixLevel = 0.5f;
    float speed = 0.0f;

    // Tap tempo mutables
    size_t currentTempoSamples;
    unsigned long tapTempoTime = 0;
    unsigned long currentTapTempoAvg = 0;
    unsigned long *pedalTapTempoAvg;

    // Type switcher mutables
    DelayType currentDelayType = DT_UNSET;
    float tempoModifier = 1.0f;
};

#endif