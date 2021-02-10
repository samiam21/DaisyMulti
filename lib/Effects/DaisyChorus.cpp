#include "DaisyChorus.h"

void DaisyChorus::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

    // Initialize the knobs
    boostKnob.Init(hw, KNOB_1_CHN, boostLevel, boostLevelMin, boostLevelMax);
    rateKnob.Init(hw, KNOB_2_CHN, rate, rateMin, rateMax);
    widthKnob.Init(hw, KNOB_3_CHN, width);
    delayKnob.Init(hw, KNOB_4_CHN, delay);

    // Initialize the Chorus
    float sample_rate = hw->AudioSampleRate();
    chorus.Init(sample_rate);
    chorus.SetLfoFreq(rate);
    chorus.SetLfoDepth(width);
    chorus.SetDelay(delay);
}

float DaisyChorus::Process(float in)
{
    float wet, dry;

    // Adjust the input signal by the boost
    dry = in * boostLevel;

    // Process the chorus
    wet = chorus.Process(dry);

    // Output the processed signal
    return wet;
}

void DaisyChorus::Cleanup()
{
}

void DaisyChorus::Loop(PedalState state)
{
    // Knob 1 controls the boost level
    if (boostKnob.SetNewValue(boostLevel))
    {
        debugPrintlnF(hw, "Updated the boost level to: %f", boostLevel);
    }

    // Knob 2 controls the LFO rate
    if (rateKnob.SetNewValue(rate))
    {
        chorus.SetLfoFreq(rate);

        debugPrintlnF(hw, "Updated the rate to: %f", rate);
    }

    // Knob 3 controls the LFO width
    if (widthKnob.SetNewValue(width))
    {
        chorus.SetLfoDepth(width);

        debugPrintlnF(hw, "Updated the width to: %f", width);
    }

    // Knob 4 controls the delay
    if (delayKnob.SetNewValue(delay))
    {
        chorus.SetDelay(delay);

        debugPrintlnF(hw, "Updated the delay to: %f", delay);
    }
}

char *DaisyChorus::GetEffectName()
{
    return (char *)"CHORUS";
}