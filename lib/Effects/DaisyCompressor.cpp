#include "DaisyCompressor.h"

void DaisyCompressor::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay)
{
    hw = hardware;
    display = daisyDisplay;

    // Initialize the knobs
    ratioKnob.Init(hw, KNOB_1_CHN, ratio, ratioMin, ratioMax);
    thresholdKnob.Init(hw, KNOB_2_CHN, threshold, thresholdMin, thresholdMax);
    attackKnob.Init(hw, KNOB_3_CHN, attack, attackMin, attackMax);
    releaseKnob.Init(hw, KNOB_4_CHN, release, releaseMin, releaseMax);

    // Initialize the compressor
    compressor.Init(hw->AudioSampleRate());
    compressor.AutoMakeup(true);
}

float DaisyCompressor::Process(float in)
{
    return compressor.Process(in);
}

void DaisyCompressor::Cleanup()
{
}

void DaisyCompressor::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Knob 1 controls the ratio
        if (ratioKnob.SetNewValue(ratio))
        {
            compressor.SetRatio(ratio);
            debugPrintlnF(hw, "Updated the ratio to: %f", ratio);

            // Update the effect settings
            effectSettings.knobSettings[0] = ratio;
        }

        // Knob 2 controls the threshold
        if (thresholdKnob.SetNewValue(threshold))
        {
            compressor.SetThreshold(threshold);
            debugPrintlnF(hw, "Updated the threshold to: %f", threshold);

            // Update the effect settings
            effectSettings.knobSettings[1] = threshold;
        }

        // Knob 3 controls the attack
        if (attackKnob.SetNewValue(attack))
        {
            compressor.SetAttack(attack);
            debugPrintlnF(hw, "Updated the attack to: %f", attack);

            // Update the effect settings
            effectSettings.knobSettings[2] = attack;
        }

        // Knob 4 controls the release
        if (releaseKnob.SetNewValue(release))
        {
            compressor.SetRelease(release);
            debugPrintlnF(hw, "Updated the release to: %f", release);

            // Update the effect settings
            effectSettings.knobSettings[3] = release;
        }
    }
}

char *DaisyCompressor::GetEffectName()
{
    return (char *)"COMPRESSOR";
}

char **DaisyCompressor::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings DaisyCompressor::GetEffectSettings()
{
    // Update the effect settings
    effectSettings.knobSettings[0] = ratio;
    effectSettings.knobSettings[1] = threshold;
    effectSettings.knobSettings[2] = attack;
    effectSettings.knobSettings[3] = release;

    // Return the settings
    return effectSettings;
}

void DaisyCompressor::SetEffectSettings(EffectSettings effectSettings)
{
    // Update boost level from the effect settings
    ratio = effectSettings.knobSettings[0];
    threshold = effectSettings.knobSettings[1];
    attack = effectSettings.knobSettings[2];
    release = effectSettings.knobSettings[3];
}