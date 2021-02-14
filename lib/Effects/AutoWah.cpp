#include "AutoWah.h"

void AutoWah::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

    autowah.Init(hw->AudioSampleRate());

    // Initialize the knobs and effect values
    dryWetKnob.Init(hw, KNOB_1_CHN, dryWetLevel, dryWetLevelMin, dryWetLevelMax);
    levelKnob.Init(hw, KNOB_2_CHN, level);
    wahLevelKnob.Init(hw, KNOB_3_CHN, wahLevel, wahLevelMin, wahLevelMax);

    // Set parameters for autowah
    autowah.SetDryWet(dryWetLevel);
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
    return wet * dryWetLevel + in * (1 - dryWetLevel);
}

void AutoWah::Cleanup()
{
}

void AutoWah::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Update the dry/wet level
        if (dryWetKnob.SetNewValue(dryWetLevel))
        {
            autowah.SetDryWet(dryWetLevel);
            debugPrintlnF(hw, "Updated the dry/wet level to: %f", dryWetLevel);
        }

        // Update the wah level
        if (wahLevelKnob.SetNewValue(wahLevel))
        {
            autowah.SetWah(wahLevel);
            debugPrintlnF(hw, "Updated the wah level to: %f", wahLevel);
        }

        // Update the level
        if (levelKnob.SetNewValue(level))
        {
            autowah.SetLevel(level);
            debugPrintlnF(hw, "Updated the level to: %f", level);
        }
    }
}

char *AutoWah::GetEffectName()
{
    return (char *)"AutoWah";
}