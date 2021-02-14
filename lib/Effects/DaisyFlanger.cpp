#include "DaisyFlanger.h"

void DaisyFlanger::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

    // Initialize the knobs
    mixKnob.Init(hw, KNOB_1_CHN, mixLevel);
    rateKnob.Init(hw, KNOB_2_CHN, rate, rateMin, rateMax);
    widthKnob.Init(hw, KNOB_3_CHN, width);
    feedbackKnob.Init(hw, KNOB_4_CHN, feedback);

    // Initialize the Flanger
    float sample_rate = hw->AudioSampleRate();
    flanger.Init(sample_rate);
    flanger.SetLfoFreq(rate);
    flanger.SetLfoDepth(width);
    flanger.SetFeedback(feedback);
}

float DaisyFlanger::Process(float in)
{
    float wet, dry;

    // Adjust the input signal by the boost
    dry = in * mixLevel;

    // Process the flanger
    wet = flanger.Process(dry);

    // Mix wet and dry and send to I/O
    return wet * mixLevel + in * (1 - mixLevel);
}

void DaisyFlanger::Cleanup()
{
}

void DaisyFlanger::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Knob 1 controls the mix level
        if (mixKnob.SetNewValue(mixLevel))
        {
            debugPrintlnF(hw, "Updated the mix level to: %f", mixLevel);
        }

        // Knob 2 controls the LFO rate
        if (rateKnob.SetNewValue(rate))
        {
            flanger.SetLfoFreq(rate);

            debugPrintlnF(hw, "Updated the rate to: %f", rate);
        }

        // Knob 3 controls the LFO width
        if (widthKnob.SetNewValue(width))
        {
            flanger.SetLfoDepth(width);

            debugPrintlnF(hw, "Updated the width to: %f", width);
        }

        // Knob 4 controls the feedback
        if (feedbackKnob.SetNewValue(feedback))
        {
            flanger.SetFeedback(feedback);

            debugPrintlnF(hw, "Updated the feedback to to: %f", feedback);
        }
    }
}

char *DaisyFlanger::GetEffectName()
{
    return (char *)"FLANGER";
}