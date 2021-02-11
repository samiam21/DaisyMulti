#include "Drive.h"

void Drive::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

    // Initialize the knobs and effect values
    pregainKnob.Init(hw, KNOB_1_CHN, pregainLevel, pregainLevelMin, pregainLevelMax);
    gainKnob.Init(hw, KNOB_2_CHN, gainLevel, gainLevelMin, gainLevelMax);
    driveKnob.Init(hw, KNOB_3_CHN, driveLevel, driveLevelMin, driveLevelMax);
    mixKnob.Init(hw, KNOB_4_CHN, mixLevel);

    // Initialize the balancer
    sample_rate = hw->AudioSampleRate();
    balance.Init(sample_rate);
}

float Drive::Process(float in)
{
    // Set overdrive signal parameters
    float a = sin(((driveLevel + 1) / 101) * (PI_VAL / 2));
    float k = (2 * a) / (1 - a);

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

void Drive::Loop(PedalState state)
{
    // Update the pregain level
    if (pregainKnob.SetNewValue(pregainLevel))
    {
        debugPrintlnF(hw, "Updated the pregain level to: %f", pregainLevel);
    }

    // Update the gain level
    if (gainKnob.SetNewValue(gainLevel))
    {
        debugPrintlnF(hw, "Updated the gain level to: %f", gainLevel);
    }

    // Update the mix level
    if (mixKnob.SetNewValue(mixLevel))
    {
        debugPrintlnF(hw, "Updated the mix level to: %f", mixLevel);
    }

    // Update the drive level
    if (driveKnob.SetNewValue(driveLevel))
    {
        debugPrintlnF(hw, "Updated the drive level to: %f", driveLevel);
    }
}

char *Drive::GetEffectName()
{
    return (char *)"Overdrive";
}

float Drive::WaveShape(float in, float k)
{
    return (1 + k) * in / (1 + k * abs(in));
}