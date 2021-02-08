#include "SimpleBypass.h"

void SimpleBypass::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

    knob1.Init(hw, KNOB_1_CHN, boostLevel, boostLevelMin, boostLevelMax);
}

void SimpleBypass::AudioCallback(float **in, float **out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        out[AUDIO_OUT_CH][i] = in[AUDIO_IN_CH][i] * boostLevel;
    }
}

void SimpleBypass::Cleanup()
{
}

void SimpleBypass::Loop()
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