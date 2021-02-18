#include "Echo.h"

void Echo::Setup(daisy::DaisySeed *hardware)
{
    // Init Delay Line
    del_line.Init();

    // Set Delay Time in Samples
    currentTempoSamples = ((96000 / initialTempoBpm) * 30) * tempoModifier;
    del_line.SetDelay(currentTempoSamples);

    // TODO: Initialize the tap tempo button

    // Initialize the knobs
    decayKnob.Init(hw, KNOB_1_CHN, decayValue, minDecayValue, maxDecayValue);
    mixLevelKnob.Init(hw, KNOB_2_CHN, mixLevel, mixLevelMin, mixLevelMax);
    speedKnob.Init(hw, KNOB_3_CHN, speed, speedMin, speedMax);

    // Initialize the type
    typeSwitcher.Init(hw->GetPin(effectSPDT1Pin1), hw->GetPin(effectSPDT1Pin2));
    TypeSwitcherLoopControl();
}

float Echo::Process(float in)
{
    float dry, wet;

    // Read Dry from I/O
    dry = in;

    // Read Wet from Delay Line
    wet = del_line.Read();

    // Write to Delay with a controlled decay time
    del_line.Write((wet * decayValue) + dry);

    // Mix Dry and Wet and send to I/O
    return wet * mixLevel + dry * (1 - mixLevel);
}

void Echo::Cleanup()
{
    del_line.Reset();
}

void Echo::Loop(bool allowEffectControl)
{
    // Only adjust if we are in edit mode
    if (allowEffectControl)
    {
        // Update the decay if the knob has been moved
        if (decayKnob.SetNewValue(decayValue))
        {
            debugPrintlnF(hw, "Updated the decay level to: %f", decayValue);
        }

        // Update the effect level if the knob has been moved
        if (mixLevelKnob.SetNewValue(mixLevel))
        {
            debugPrintlnF(hw, "Updated the mix level to: %f", mixLevel);
        }

        // Update the speed if the knob has been moved
        if (speedKnob.SetNewValue(speed))
        {
            currentTempoSamples = ((96000 / speed) * 30) * tempoModifier;
            del_line.SetDelay(currentTempoSamples);

            debugPrintlnF(hw, "Updated the speed to: %f", speed);
        }

        // Handle type
        TypeSwitcherLoopControl();
    }
}

// Handle reading the SPDT switch and setting the delay type
void Echo::TypeSwitcherLoopControl()
{
    // Determine which type is selected
    if (typeSwitcher.ReadToggle() == 0)
    {
        // Only set the type if we have a new one
        if (currentDelayType != QUARTER)
        {
            debugPrintln(hw, "Changing to quarter note delay");

            // Set the delay type and tempo modifier
            currentDelayType = QUARTER;
            tempoModifier = 1.0f;

            // Update the delay tempo
            del_line.SetDelay(currentTempoSamples * tempoModifier);
        }
    }
    else if (typeSwitcher.ReadToggle() == 2)
    {
        // Only set the type if we have a new one
        if (currentDelayType != TRIPLET)
        {
            debugPrintln(hw, "Changing to triplet note delay");

            // Set the delay type and tempo modifier
            currentDelayType = TRIPLET;
            tempoModifier = 0.333f;

            // Update the delay tempo
            del_line.SetDelay(currentTempoSamples * tempoModifier);
        }
    }
    else
    {
        // Only set the type if we have a new one
        if (currentDelayType != DOTTED_EIGHTH)
        {
            debugPrintln(hw, "Changing to dotted eighth note delay");

            // Set the delay type and tempo modifier
            currentDelayType = DOTTED_EIGHTH;
            tempoModifier = 0.75f;

            // Update the delay tempo
            del_line.SetDelay(currentTempoSamples * tempoModifier);
        }
    }
}

char *Echo::GetEffectName()
{
    return (char *)"ECHO";
}