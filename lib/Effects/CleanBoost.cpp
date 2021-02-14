#include "CleanBoost.h"

void CleanBoost::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

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
        }
    }
}

char *CleanBoost::GetEffectName()
{
    return (char *)"CLEAN BOOST";
}