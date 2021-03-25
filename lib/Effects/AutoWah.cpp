#include "AutoWah.h"

void AutoWah::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, unsigned long *avgTempo)
{
    hw = hardware;
    display = daisyDisplay;

    float sample_rate = hw->AudioSampleRate();
    autowah.Init(sample_rate);

    // Initialize the knobs and effect values
    mixLevelKnob.Init(hw, KNOB_1_CHN, mixLevel);
    wahLevelKnob.Init(hw, KNOB_2_CHN, wahLevel);
    levelKnob.Init(hw, KNOB_3_CHN, level);

    // Set parameters for autowah
    autowah.SetWah(wahLevel);
    autowah.SetLevel(level);
}

float AutoWah::Process(float in)
{
    float wet;

    // Read signal from I/O and apply gain
    wet = in;

    // Get wet signal by adding autowah
    wet = autowah.Process(wet);

    // Mix wet and dry and send to I/O
    return wet * mixLevel + in * (1 - mixLevel);
}

void AutoWah::Cleanup()
{
}

void AutoWah::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Update the mix level
        if (mixLevelKnob.SetNewValue(mixLevel))
        {
            //autowah.SetDryWet(dryWetLevel);
            debugPrintlnF(hw, "Updated the mix level to: %f", mixLevel);
            updateEditModeKnobValue(display, 0, mixLevel);
        }

        // Update the wah level
        if (wahLevelKnob.SetNewValue(wahLevel))
        {
            autowah.SetWah(wahLevel);
            debugPrintlnF(hw, "Updated the wah level to: %f", wahLevel);
            updateEditModeKnobValue(display, 1, wahLevel);
        }

        // Update the level
        if (levelKnob.SetNewValue(level))
        {
            autowah.SetLevel(level);
            debugPrintlnF(hw, "Updated the level to: %f", level);
            updateEditModeKnobValue(display, 2, level);
        }
    }
}

char *AutoWah::GetEffectName()
{
    return (char *)"AUTOWAH";
}

char **AutoWah::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings AutoWah::GetEffectSettings()
{
    // Add levels to the effect settings
    effectSettings.knobSettings[0] = mixLevel;
    effectSettings.knobSettings[1] = wahLevel;
    effectSettings.knobSettings[2] = level;

    // Return the settings
    return effectSettings;
}

void AutoWah::SetEffectSettings(EffectSettings effectSettings)
{
    // Update levels from the effect settings
    mixLevel = effectSettings.knobSettings[0];
    wahLevel = effectSettings.knobSettings[1];
    autowah.SetWah(wahLevel);
    level = effectSettings.knobSettings[2];
    autowah.SetLevel(level);
}