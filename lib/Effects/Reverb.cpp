#include "Reverb.h"

void Reverb::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay)
{
    hw = hardware;
    display = daisyDisplay;

    // Get the sample rate for initializing the reverb and calculating the max LP Frequency
    sample_rate = hw->AudioSampleRate();

    // Initialize the knobs
    feedbackKnob.Init(hw, KNOB_1_CHN, feedback, feedbackMin, feedbackMax);
    lpfreqKnob.Init(hw, KNOB_2_CHN, lpfreq, lpfreqMin, sample_rate / 2.0f);
    mixKnob.Init(hw, KNOB_3_CHN, mixLevel);

    // Initialize the compressor
    int ret = verb.Init(sample_rate);
    debugPrintlnF(hw, "Verb setup %d", ret);
}

float Reverb::Process(float in)
{
    float out1;
    float out2;
    verb.Process(in, in, &out1, &out2);
    return out1 * mixLevel + in * (1 - mixLevel);
}

void Reverb::Cleanup()
{
}

void Reverb::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Knob 1 controls the ratio
        if (feedbackKnob.SetNewValue(feedback))
        {
            verb.SetFeedback(feedback);
            debugPrintlnF(hw, "Updated the feedback to: %f", feedback);

            // Update the effect settings
            effectSettings.knobSettings[0] = feedback;
        }

        // Knob 2 controls the threshold
        if (lpfreqKnob.SetNewValue(lpfreq))
        {
            verb.SetLpFreq(lpfreq);
            debugPrintlnF(hw, "Updated the LP Frequency to: %f", lpfreq);

            // Update the effect settings
            effectSettings.knobSettings[1] = lpfreq;
        }

        // Update the mix level
        if (mixKnob.SetNewValue(mixLevel))
        {
            debugPrintlnF(hw, "Updated the mix level to: %f", mixLevel);
        }
    }
}

char *Reverb::GetEffectName()
{
    return (char *)"REVERB";
}

char **Reverb::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings Reverb::GetEffectSettings()
{
    // Update the effect settings
    effectSettings.knobSettings[0] = feedback;
    effectSettings.knobSettings[1] = lpfreq;
    effectSettings.knobSettings[2] = mixLevel;

    // Return the settings
    return effectSettings;
}

void Reverb::SetEffectSettings(EffectSettings effectSettings)
{
    // Update boost level from the effect settings
    feedback = effectSettings.knobSettings[0];
    lpfreq = effectSettings.knobSettings[1];
    mixLevel = effectSettings.knobSettings[2];
}