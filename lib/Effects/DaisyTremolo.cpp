#include "DaisyTremolo.h"

void DaisyTremolo::Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, unsigned long *avgTempo)
{
    hw = hardware;
    display = daisyDisplay;

    // Initialize the knobs
    mixKnob.Init(hw, KNOB_1_CHN, mixLevel);
    rateKnob.Init(hw, KNOB_2_CHN, rate, rateMin, rateMax);
    widthKnob.Init(hw, KNOB_3_CHN, width);

    // Initialize the toggle
    waveformSelector.Init(hw->GetPin(effectSPDT1Pin1), hw->GetPin(effectSPDT1Pin2));
    UpdateToggleDisplay();

    // Initialize the Tremolo
    float sample_rate = hw->AudioSampleRate();
    tremolo.Init(sample_rate);
    tremolo.SetFreq(rate);
    tremolo.SetDepth(width);
    tremolo.SetWaveform(waveform);
}

float DaisyTremolo::Process(float in)
{
    float wet, dry;

    // Adjust the input signal by the boost
    dry = in;

    // Process the tremolo
    wet = tremolo.Process(dry);

    // Mix wet and dry and send to I/O
    return wet * mixLevel + in * (1 - mixLevel);
}

void DaisyTremolo::Cleanup()
{
}

void DaisyTremolo::Loop(bool allowEffectControl)
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

        // Knob 2 controls the LFO rate
        if (rateKnob.SetNewValue(rate))
        {
            tremolo.SetFreq(rate);

            debugPrintlnF(hw, "Updated the rate to: %f", rate);
            updateEditModeKnobValue(display, 1, rate);
        }

        // Knob 3 controls the LFO width
        if (widthKnob.SetNewValue(width))
        {
            tremolo.SetDepth(width);

            debugPrintlnF(hw, "Updated the width to: %f", width);
            updateEditModeKnobValue(display, 2, width);
        }

        // Read the toggle to set the tremolo waveform
        if (waveformSelector.ReadToggle() == 0)
        {
            if (waveform != Oscillator::WAVE_SIN)
            {
                waveform = Oscillator::WAVE_SIN;
                tremolo.SetWaveform(waveform);
                debugPrintln(hw, "Setting waveform to WAVE_SIN");
                UpdateToggleDisplay();
            }
        }
        else if (waveformSelector.ReadToggle() == 1)
        {
            if (waveform != Oscillator::WAVE_SQUARE)
            {
                waveform = Oscillator::WAVE_SQUARE;
                tremolo.SetWaveform(waveform);
                debugPrintln(hw, "Setting waveform to WAVE_SQUARE");
                UpdateToggleDisplay();
            }
        }
        else
        {
            if (waveform != Oscillator::WAVE_RAMP)
            {
                waveform = Oscillator::WAVE_RAMP;
                tremolo.SetWaveform(waveform);
                debugPrintln(hw, "Setting waveform to WAVE_RAMP");
                UpdateToggleDisplay();
            }
        }
    }
}

char *DaisyTremolo::GetEffectName()
{
    return (char *)"TREMOLO";
}

char **DaisyTremolo::GetKnobNames()
{
    return (char **)knobNames;
}

void DaisyTremolo::UpdateToggleDisplay()
{
    switch (waveform)
    {
    case Oscillator::WAVE_SIN:
        updateEditModeToggleValue(display, (char *)"SIN");
        break;
    case Oscillator::WAVE_SQUARE:
        updateEditModeToggleValue(display, (char *)"SQUARE");
        break;
    case Oscillator::WAVE_RAMP:
        updateEditModeToggleValue(display, (char *)"RAMP");
        break;
    }
}

EffectSettings DaisyTremolo::GetEffectSettings()
{
    // Add levels to the effect settings
    effectSettings.knobSettings[0] = mixLevel;
    effectSettings.knobSettings[1] = rate;
    effectSettings.knobSettings[2] = width;

    // Add the wave shape to the effect settings
    effectSettings.togglePosition = waveformSelector.ReadToggle();

    // Return the settings
    return effectSettings;
}

void DaisyTremolo::SetEffectSettings(EffectSettings effectSettings)
{
    // Update levels from effect settings
    mixLevel = effectSettings.knobSettings[0];
    rate = effectSettings.knobSettings[1];
    tremolo.SetFreq(rate);
    width = effectSettings.knobSettings[2];
    tremolo.SetDepth(width);

    // Update wave shape from effect settings
    if (effectSettings.togglePosition == 0)
    {
        waveform = Oscillator::WAVE_SIN;
        tremolo.SetWaveform(waveform);
    }
    else if (effectSettings.togglePosition == 1)
    {
        waveform = Oscillator::WAVE_SQUARE;
        tremolo.SetWaveform(waveform);
    }
    else
    {
        waveform = Oscillator::WAVE_RAMP;
        tremolo.SetWaveform(waveform);
    }
}