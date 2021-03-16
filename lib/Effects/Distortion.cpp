#include "Distortion.h"

void Distortion::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay)
{
    hw = hardware;
    display = daisyDisplay;

    // Initialize the clipping toggle
    clippingToggle.Init(hw->GetPin(effectSPDT1Pin1), hw->GetPin(effectSPDT1Pin2));
    SetClipThreshold();
    UpdateToggleDisplay();

    // Initialize the knobs and effect values
    pregainKnob.Init(hw, KNOB_1_CHN, pregainLevel, pregainLevelMin, pregainLevelMax);
    gainKnob.Init(hw, KNOB_2_CHN, gainLevel, gainLevelMin, gainLevelMax);
    mixKnob.Init(hw, KNOB_3_CHN, mixLevel);

    // Initialize the balancer
    sample_rate = hw->AudioSampleRate();
    balance.Init(sample_rate);
}

float Distortion::Process(float in)
{
    float wet, dry;

    // Adjust input signal by the pregain
    dry = in;
    dry *= pregainLevel;

    // Adjust the wet signal by the gain
    wet = dry;
    wet *= gainLevel;

    // Wave shape the wet signal
    wet = WaveShape(wet);

    // Apply the hard clipping
    wet = HardClip(wet);

    // Balance the output to account for volume gain in processing
    wet = balance.Process(wet, dry);

    // Send the output signal, mixed wet with dry
    return wet * mixLevel + dry * (1 - mixLevel);
}

void Distortion::Cleanup()
{
}

void Distortion::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
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

        // Update the clipping threshold
        SetClipThreshold();
    }
}

char *Distortion::GetEffectName()
{
    return (char *)"DISTORTION";
}

char **Distortion::GetKnobNames()
{
    return (char **)knobNames;
}

void Distortion::UpdateToggleDisplay()
{
    switch (currentClip)
    {
    case 0:
        updateEditModeToggleValue(display, (char *)"HIGH");
        break;
    case 1:
        updateEditModeToggleValue(display, (char *)"MIDDLE");
        break;
    case 2:
        updateEditModeToggleValue(display, (char *)"LOW");
        break;
    }
}

float Distortion::WaveShape(float in)
{
    if (in > 0)
        return 1 - expf(-in);
    return -1 + expf(in);
}

float Distortion::HardClip(float in)
{
    in = in > hardClipThreshold ? hardClipThreshold : in;
    in = in < -hardClipThreshold ? -hardClipThreshold : in;
    return in;
}

void Distortion::SetClipThreshold()
{
    u_int8_t togg = clippingToggle.ReadToggle();
    if (togg != currentClip)
    {
        if (togg == 0)
        {
            hardClipThreshold = clipThresholdHigh;
            debugPrintln(hw, "Clipping set to high");
            UpdateToggleDisplay();
        }
        else if (togg == 1)
        {
            hardClipThreshold = clipThresholdMid;
            debugPrintln(hw, "Clipping set to middle");
            UpdateToggleDisplay();
        }
        else
        {
            hardClipThreshold = clipThresholdLow;
            debugPrintln(hw, "Clipping set to low");
            UpdateToggleDisplay();
        }

        currentClip = togg;
    }
}

EffectSettings Distortion::GetEffectSettings()
{
    // Add levels to the effect settings
    effectSettings.knobSettings[0] = pregainLevel;
    effectSettings.knobSettings[1] = gainLevel;
    effectSettings.knobSettings[2] = mixLevel;

    // Add the wave shape to the effect settings
    effectSettings.togglePosition = clippingToggle.ReadToggle();

    // Return the settings
    return effectSettings;
}

void Distortion::SetEffectSettings(EffectSettings effectSettings)
{
    // Update levels from effect settings
    pregainLevel = effectSettings.knobSettings[0];
    gainLevel = effectSettings.knobSettings[1];
    mixLevel = effectSettings.knobSettings[2];

    // Update clipping from effect settings
    currentClip = effectSettings.togglePosition;
    if (currentClip == 0)
    {
        hardClipThreshold = clipThresholdHigh;
    }
    else if (currentClip == 1)
    {
        hardClipThreshold = clipThresholdMid;
    }
    else
    {
        hardClipThreshold = clipThresholdLow;
    }
}