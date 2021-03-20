#include "CleanBoost.h"

void CleanBoost::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, unsigned long *avgTempo)
{
    hw = hardware;
    display = daisyDisplay;

    boostKnob.Init(hw, KNOB_1_CHN, boostLevel, boostLevelMin, boostLevelMax);
}

float CleanBoost::Process(float in)
{
    return in * boostLevel;
}

void CleanBoost::Cleanup()
{
}

void CleanBoost::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Knob 1 controls the boost level
        if (boostKnob.SetNewValue(boostLevel))
        {
            debugPrintlnF(hw, "Updated the boost level to: %f", boostLevel);
            updateEditModeKnobValue(display, 0, boostLevel);

            // Update the effect settings
            effectSettings.knobSettings[0] = boostLevel;
        }
    }
}

char *CleanBoost::GetEffectName()
{
    return (char *)"CLEAN BOOST";
}

char **CleanBoost::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings CleanBoost::GetEffectSettings()
{
    // Add boost level to the effect settings
    effectSettings.knobSettings[0] = boostLevel;

    // Return the settings
    return effectSettings;
}

void CleanBoost::SetEffectSettings(EffectSettings effectSettings)
{
    // Update boost level from the effect settings
    boostLevel = effectSettings.knobSettings[0];
}