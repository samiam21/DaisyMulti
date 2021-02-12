#include "DaisyTremolo.h"

void DaisyTremolo::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

    // Initialize the knobs
    mixKnob.Init(hw, KNOB_1_CHN, mixLevel);
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
    dry = in * mixLevel;

    // Process the tremolo
    wet = tremolo.Process(dry);

    // Mix wet and dry and send to I/O
    return wet * mixLevel + in * (1 - mixLevel);
}

void DaisyTremolo::Cleanup()
{
}

void DaisyTremolo::Loop(PedalState state)
{
    // Only adjust if we are in edit mode
    if (state == PedalState::EDIT_MODE)
    {
        // Knob 1 controls the mix level
        if (mixKnob.SetNewValue(mixLevel))
        {
            debugPrintlnF(hw, "Updated the mix level to: %f", mixLevel);
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
}

char *DaisyTremolo::GetEffectName()
{
    return (char *)"TREMOLO";
}