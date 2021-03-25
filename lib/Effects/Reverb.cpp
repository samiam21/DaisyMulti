#include "Reverb.h"

void Reverb::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, unsigned long *avgTempo)
{
    hw = hardware;
    display = daisyDisplay;

    // Get the sample rate for initializing the reverb and calculating the max LP Frequency
    sample_rate = hw->AudioSampleRate();

    // Initialize the knobs
    mixKnob.Init(hw, KNOB_1_CHN, mixLevel);
    decayKnob.Init(hw, KNOB_2_CHN, feedback, feedbackMin, feedbackMax);
    toneKnob.Init(hw, KNOB_3_CHN, lpfreq, lpfreqMin, sample_rate / 2.0f);

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
        // Update the mix level
        if (mixKnob.SetNewValue(mixLevel))
        {
            debugPrintlnF(hw, "Updated the mix level to: %f", mixLevel);
            updateEditModeKnobValue(display, 0, mixLevel);
        }

        // Knob 1 controls the ratio
        if (decayKnob.SetNewValue(feedback))
        {
            verb.SetFeedback(feedback);
            debugPrintlnF(hw, "Updated the decay to: %f", feedback);
            updateEditModeKnobValue(display, 1, feedback);

            // Update the effect settings
            effectSettings.knobSettings[0] = feedback;
        }

        // Knob 2 controls the threshold
        if (toneKnob.SetNewValue(lpfreq))
        {
            verb.SetLpFreq(lpfreq);
            debugPrintlnF(hw, "Updated the tone to: %f", lpfreq);
            updateEditModeKnobValue(display, 2, lpfreq);

            // Update the effect settings
            effectSettings.knobSettings[1] = lpfreq;
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

    verb.SetFeedback(feedback);
    verb.SetLpFreq(lpfreq);
}