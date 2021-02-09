#include "SimpleBypass.h"

void SimpleBypass::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

    knob1.Init(hw, KNOB_1_CHN, boostLevel, boostLevelMin, boostLevelMax);
}

float SimpleBypass::Process(float in)
{
    return in * boostLevel;
}

void SimpleBypass::Cleanup()
{
}

void SimpleBypass::Loop(PedalState state)
{
    // Knob 1 controls the boost level
    if (knob1.SetNewValue(boostLevel))
    {
        debugPrintlnF(hw, "Updated the boost level to: %f", boostLevel);
    }
}

char *SimpleBypass::GetEffectName()
{
    return (char *)"Simple Bypass";
}