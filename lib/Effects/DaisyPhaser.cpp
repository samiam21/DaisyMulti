#include "DaisyPhaser.h"

void DaisyPhaser::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, unsigned long *avgTempo)
{
    hw = hardware;
    display = daisyDisplay;

    // Initialize the knobs
    mixKnob.Init(hw, KNOB_1_CHN, mixLevel);
    depthKnob.Init(hw, KNOB_2_CHN, depth);
    frequencyKnob.Init(hw, KNOB_3_CHN, frequency, frequencyMin, frequencyMax);
    feedbackKnob.Init(hw, KNOB_4_CHN, feedback);

    // Initialize the Flanger
    phaser.Init(hw->AudioSampleRate());
    phaser.SetLfoDepth(depth);
    phaser.SetFreq(frequency);
    phaser.SetFeedback(feedback);
}

float DaisyPhaser::Process(float in)
{
    float wet, dry;

    // Adjust the input signal by the boost
    dry = in;

    // Process the phaser
    wet = phaser.Process(dry);

    // Mix wet and dry and send to I/O
    return wet * mixLevel + in * (1 - mixLevel);
}

void DaisyPhaser::Cleanup()
{
}

void DaisyPhaser::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Knob 1 controls the mix level
        if (mixKnob.SetNewValue(mixLevel))
        {
            debugPrintlnF(hw, "Updated the mix level to: %f", mixLevel);
            updateEditModeKnobValue(display, 0, mixLevel);
        }

        // Knob 2 controls the LFO depth
        if (depthKnob.SetNewValue(depth))
        {
            phaser.SetLfoDepth(depth);

            debugPrintlnF(hw, "Updated the depth to: %f", depth);
            updateEditModeKnobValue(display, 1, depth);
        }

        // Knob 3 controls the frequency
        if (frequencyKnob.SetNewValue(frequency))
        {
            phaser.SetFreq(frequency);

            debugPrintlnF(hw, "Updated the frequency to: %f", frequency);
            updateEditModeKnobValue(display, 2, frequency);
        }

        // Knob 4 controls the feedback
        if (feedbackKnob.SetNewValue(feedback))
        {
            phaser.SetFeedback(feedback);

            debugPrintlnF(hw, "Updated the feedback to to: %f", feedback);
            updateEditModeKnobValue(display, 3, feedback);
        }
    }
}

char *DaisyPhaser::GetEffectName()
{
    return (char *)"PHASER";
}

char **DaisyPhaser::GetKnobNames()
{
    return (char **)knobNames;
}

EffectSettings DaisyPhaser::GetEffectSettings()
{
    // Add levels to the effect settings
    effectSettings.knobSettings[0] = mixLevel;
    effectSettings.knobSettings[1] = depth;
    effectSettings.knobSettings[2] = frequency;
    effectSettings.knobSettings[3] = feedback;

    // Return the settings
    return effectSettings;
}

void DaisyPhaser::SetEffectSettings(EffectSettings effectSettings)
{
    // Update levels with effect settings
    mixLevel = effectSettings.knobSettings[0];
    depth = effectSettings.knobSettings[1];
    phaser.SetLfoDepth(depth);
    frequency = effectSettings.knobSettings[2];
    phaser.SetFreq(frequency);
    feedback = effectSettings.knobSettings[3];
    phaser.SetFeedback(feedback);
}