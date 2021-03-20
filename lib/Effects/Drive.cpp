#include "Drive.h"

void Drive::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, unsigned long *avgTempo)
{
    hw = hardware;
    display = daisyDisplay;

    // Initialize the knobs and effect values
    mixKnob.Init(hw, KNOB_1_CHN, mixLevel);
    pregainKnob.Init(hw, KNOB_2_CHN, pregainLevel, pregainLevelMin, pregainLevelMax);
    gainKnob.Init(hw, KNOB_3_CHN, gainLevel, gainLevelMin, gainLevelMax);
    driveKnob.Init(hw, KNOB_4_CHN, driveLevel, driveLevelMin, driveLevelMax);

    // Initialize the balancer
    sample_rate = hw->AudioSampleRate();
    balance.Init(sample_rate);
}

float Drive::Process(float in)
{
    float wet, dry;

    // Adjust input signal by the pregain
    dry = in;
    dry *= pregainLevel;

    // Adjust the wet signal by the gain
    wet = dry;
    wet *= gainLevel;

    // Wave shape the wet signal
    wet = WaveShape(wet, k);

    // Balance the output to account for volume gain in processing
    wet = balance.Process(wet, dry);

    // Send the output signal, mixed wet with dry
    return wet * mixLevel + dry * (1 - mixLevel);
}

void Drive::Cleanup()
{
}

void Drive::Loop(bool allowEffectControl)
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

        // Update the pregain level
        if (pregainKnob.SetNewValue(pregainLevel))
        {
            debugPrintlnF(hw, "Updated the pregain level to: %f", pregainLevel);
            updateEditModeKnobValue(display, 1, pregainLevel);
        }

        // Update the gain level
        if (gainKnob.SetNewValue(gainLevel))
        {
            debugPrintlnF(hw, "Updated the gain level to: %f", gainLevel);
            updateEditModeKnobValue(display, 2, gainLevel);
        }

        // Update the drive level
        if (driveKnob.SetNewValue(driveLevel))
        {
            // Calculate overdrive signal parameters
            a = sin(((driveLevel + 1) / 101) * (PI_VAL / 2));
            k = (2 * a) / (1 - a);

            debugPrintlnF(hw, "Updated the drive level to: %f", driveLevel);
            updateEditModeKnobValue(display, 3, driveLevel);
        }
    }
}

char *Drive::GetEffectName()
{
    return (char *)"OVERDRIVE";
}

char **Drive::GetKnobNames()
{
    return (char **)knobNames;
}

float Drive::WaveShape(float in, float k)
{
    return (1 + k) * in / (1 + k * abs(in));
}

EffectSettings Drive::GetEffectSettings()
{
    // Add levels to the effect settings
    effectSettings.knobSettings[0] = pregainLevel;
    effectSettings.knobSettings[1] = gainLevel;
    effectSettings.knobSettings[2] = driveLevel;
    effectSettings.knobSettings[3] = mixLevel;

    // Return the settings
    return effectSettings;
}

void Drive::SetEffectSettings(EffectSettings effectSettings)
{
    // Update levels from effect settings
    pregainLevel = effectSettings.knobSettings[0];
    gainLevel = effectSettings.knobSettings[1];
    driveLevel = effectSettings.knobSettings[2];
    mixLevel = effectSettings.knobSettings[3];

    // Calculate overdrive signal parameters
    a = sin(((driveLevel + 1) / 101) * (PI_VAL / 2));
    k = (2 * a) / (1 - a);
}