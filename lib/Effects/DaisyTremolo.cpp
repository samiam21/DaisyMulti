#include "DaisyTremolo.h"

void DaisyTremolo::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

    // Initialize the knobs
    boostKnob.Init(hw, KNOB_1_CHN, boostLevel, boostLevelMin, boostLevelMax);
    rateKnob.Init(hw, KNOB_2_CHN, rate, rateMin, rateMax);
    widthKnob.Init(hw, KNOB_3_CHN, width);

    // Initialize the toggle
    waveformSelector.Init(hw->GetPin(effectSPDT1Pin1), hw->GetPin(effectSPDT1Pin2));

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
    dry = in * boostLevel;

    // Process the tremolo
    wet = tremolo.Process(dry);

    // Output the processed signal
    return wet;
}

void DaisyTremolo::Cleanup()
{
}

void DaisyTremolo::Loop(PedalState state)
{
    // Knob 1 controls the boost level
    if (boostKnob.SetNewValue(boostLevel))
    {
        debugPrintlnF(hw, "Updated the boost level to: %f", boostLevel);
    }

    // Knob 2 controls the LFO rate
    if (rateKnob.SetNewValue(rate))
    {
        tremolo.SetFreq(rate);

        debugPrintlnF(hw, "Updated the rate to: %f", rate);
    }

    // Knob 3 controls the LFO width
    if (widthKnob.SetNewValue(width))
    {
        tremolo.SetDepth(width);

        debugPrintlnF(hw, "Updated the width to: %f", width);
    }

    // Read the toggle to set the tremolo waveform
    if (waveformSelector.ReadToggle() == 0)
    {
        if (waveform != Oscillator::WAVE_SIN)
        {
            waveform = Oscillator::WAVE_SIN;
            tremolo.SetWaveform(waveform);
            debugPrintln(hw, "Setting waveform to WAVE_SIN");
        }
    }
    else if (waveformSelector.ReadToggle() == 1)
    {
        if (waveform != Oscillator::WAVE_SQUARE)
        {
            waveform = Oscillator::WAVE_SQUARE;
            tremolo.SetWaveform(waveform);
            debugPrintln(hw, "Setting waveform to WAVE_SQUARE");
        }
    }
    else
    {
        if (waveform != Oscillator::WAVE_RAMP)
        {
            waveform = Oscillator::WAVE_RAMP;
            tremolo.SetWaveform(waveform);
            debugPrintln(hw, "Setting waveform to WAVE_RAMP");
        }
    }
}

char *DaisyTremolo::GetEffectName()
{
    return (char *)"TREMOLO";
}