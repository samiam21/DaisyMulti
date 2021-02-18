#include "AutoWah.h"

void AutoWah::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

    float sample_rate = hw->AudioSampleRate();
    autowah.Init(sample_rate);
    balance.Init(sample_rate);

    // Initialize the knobs and effect values
    mixLevelKnob.Init(hw, KNOB_1_CHN, mixLevel);
    levelKnob.Init(hw, KNOB_2_CHN, level);
    wahLevelKnob.Init(hw, KNOB_3_CHN, wahLevel);

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
    return (char *)"AUTOWAH";
}