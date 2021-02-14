#include "Crush.h"

void Crush::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

    decimator.Init();

    // Initialize the knobs and effect values
    mixLevelKnob.Init(hw, KNOB_1_CHN, mixLevel);
    downsamplingFactorKnob.Init(hw, KNOB_2_CHN, downsampleLevel);
    bitcrushFactorKnob.Init(hw, KNOB_3_CHN, bitcrushLevel, bitcrushMinValue, bitcrushMaxValue);

    // Set parameters for bitcrusher
    decimator.SetDownsampleFactor(downsampleLevel);
    decimator.SetBitcrushFactor(bitcrushLevel);
}

float Crush::Process(float in)
{
    float wet;

    // Read signal from I/O and apply gain
    wet = in;

    // Get wet signal by adding bitcrush
    wet = decimator.Process(wet);

    // Mix wet and dry and send to I/O
    return wet * mixLevel + in * (1 - mixLevel);
}

void Crush::Cleanup()
{
}

void Crush::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Update the mix level
        if (mixLevelKnob.SetNewValue(mixLevel))
        {
            debugPrintlnF(hw, "Updated the mix level to: %f", mixLevel);
        }

        // Update the bitcrush level
        if (bitcrushFactorKnob.SetNewValue(bitcrushLevel))
        {
            decimator.SetBitcrushFactor(bitcrushLevel);
            debugPrintlnF(hw, "Updated the bitcrush factor to: %f", bitcrushLevel);
        }

        // Update the downsample level
        if (downsamplingFactorKnob.SetNewValue(downsampleLevel))
        {
            decimator.SetDownsampleFactor(downsampleLevel);
            debugPrintlnF(hw, "Updated the downsample factor to: %f", downsampleLevel);
        }
    }
}

char *Crush::GetEffectName()
{
    return (char *)"Bitcrush";
}