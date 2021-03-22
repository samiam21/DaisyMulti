#include "DaisyFold.h"

void DaisyFold::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, unsigned long *avgTempo)
{
    hw = hardware;
    display = daisyDisplay;

    // Initialize the knobs
    mixKnob.Init(hw, KNOB_1_CHN, mix);
    incrementKnob.Init(hw, KNOB_2_CHN, increment, incrementMin, incrementMax);

    // Initialize the compressor
    fold.Init();
    fold.SetIncrement(increment);
}

float DaisyFold::Process(float in)
{
    float wet = fold.Process(in);

    // Mix wet and dry and send to I/O
    return wet * mix + in * (1 - mix);
}

void DaisyFold::Cleanup()
{
}

void DaisyFold::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Knob 1 controls the mix
        if (mixKnob.SetNewValue(mix))
        {
            debugPrintlnF(hw, "Updated the mix to: %f", mix);
            updateEditModeKnobValue(display, 0, mix);
        }

        // Knob 2 controls the increment
        if (incrementKnob.SetNewValue(increment))
        {
            fold.SetIncrement(increment);
            debugPrintlnF(hw, "Updated the increment to: %f", increment);
            updateEditModeKnobValue(display, 1, increment);
        }
    }
}

char *DaisyFold::GetEffectName()
{
    return (char *)"FOLD";
}

char **DaisyFold::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings DaisyFold::GetEffectSettings()
{
    // Update the effect settings
    effectSettings.knobSettings[0] = mix;
    effectSettings.knobSettings[1] = increment;

    // Return the settings
    return effectSettings;
}

void DaisyFold::SetEffectSettings(EffectSettings effectSettings)
{
    // Update from the effect settings
    mix = effectSettings.knobSettings[0];
    increment = effectSettings.knobSettings[1];

    fold.SetIncrement(increment);
}