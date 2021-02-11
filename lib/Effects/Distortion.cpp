#include "Distortion.h"

void Distortion::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

    // Initialize soft on off button and LED
    // giveItButton.Init(hw, hw->GetPin(effectSPSTPin1));
    // giveItLed.Init(hw->GetPin(effectLedPin1), false);
    // giveItLed.Set(giveItAllShesGot ? 1.f : 0);
    // giveItLed.Update();

    // Initialize the clipping toggle
    clippingToggle.Init(hw->GetPin(effectSPDT1Pin1), hw->GetPin(effectSPDT1Pin2));
    SetClipThreshold();

    // Initialize the knobs and effect values
    pregainKnob.Init(hw, KNOB_1_CHN, pregainLevel, pregainLevelMin, pregainLevelMax);
    gainKnob.Init(hw, KNOB_2_CHN, gainLevel, gainLevelMin, gainLevelMax);
    driveKnob.Init(hw, KNOB_3_CHN, driveLevel, driveLevelMin, driveLevelMax);
    mixKnob.Init(hw, KNOB_4_CHN, mixLevel);

    // Initialize the balancer
    sample_rate = hw->AudioSampleRate();
    balance.Init(sample_rate);
}

float Distortion::Process(float in)
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

    // Apply the hard clipping if it is enabled
    if (giveItAllShesGot)
    {
        wet = HardClip(wet);
    }

    // Balance the output to account for volume gain in processing
    wet = balance.Process(wet, dry);

    // Send the output signal, mixed wet with dry
    return wet * mixLevel + dry * (1 - mixLevel);
}

void Distortion::Cleanup()
{
    // Turn off the LED
    giveItLed.Set(0);
    giveItLed.Update();
}

void Distortion::Loop(PedalState state)
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

    // // Check for the give it all she's got on/off
    // if (giveItButton.IsPressed())
    // {
    //     giveItAllShesGot = !giveItAllShesGot;
    //     giveItLed.Set(giveItAllShesGot ? 1.f : 0);
    //     giveItLed.Update();

    //     debugPrintlnF(hw, "Give it all she's got!: %s", (giveItAllShesGot) ? "ON" : "OFF");
    // }

    // Update the clipping threshold
    SetClipThreshold();
}

char *Distortion::GetEffectName()
{
    return (char *)"Distortion";
}

float Distortion::WaveShape(float in, float k)
{
    return (1 + k) * in / (1 + k * abs(in));
}

float Distortion::HardClip(float in)
{
    in = in > hardClipThreshold ? hardClipThreshold : in;
    in = in < -hardClipThreshold ? -hardClipThreshold : in;
    return in;
}

void Distortion::SetClipThreshold()
{
    int togg = clippingToggle.ReadToggle();
    if (togg != currentClip)
    {
        if (togg == 0)
        {
            hardClipThreshold = clipThresholdHigh;
            debugPrintln(hw, "Clipping set to high");
        }
        else if (togg == 1)
        {
            hardClipThreshold = clipThresholdMid;
            debugPrintln(hw, "Clipping set to middle");
        }
        else
        {
            hardClipThreshold = clipThresholdLow;
            debugPrintln(hw, "Clipping set to low");
        }

        currentClip = togg;
    }
}