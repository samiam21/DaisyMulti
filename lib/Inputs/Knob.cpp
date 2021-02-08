#include "Knob.h"

void Knob::Init(DaisySeed *hardware, uint8_t chn, float &valueToSet)
{
    Init(hardware, chn, valueToSet, minValue, maxValue);
}

void Knob::Init(DaisySeed *hardware, uint8_t chn, float &valueToSet, float pMinValue, float pMaxValue)
{
    // Set the seed hardware object
    hw = hardware;

    // Set the pin to read
    knobChannel = chn;

    // Update the new min and max values
    minValue = pMinValue;
    maxValue = pMaxValue;

    // Set the initial value
    knobReading = hw->adc.GetFloat(knobChannel);
    valueToSet = GetNewValue(knobReading);
}

bool Knob::SetNewValue(float &valueToSet)
{
    bool ret = false;

    // Read the knob
    float newKnobReading = hw->adc.GetFloat(knobChannel);

    // Account for jitter so we aren't constantly changing the value
    if (newKnobReading > (knobReading + knobJitter) || newKnobReading < (knobReading - knobJitter))
    {
        // Update the value
        valueToSet = GetNewValue(newKnobReading);

        // A new value was set, return true
        ret = true;
    }

    return ret;
}

float Knob::GetNewValue(float newKnobReading)
{
    float newValue = minValue;

    // Check for min value, accounting for jitter
    if (newKnobReading <= (minKnobReadingValue + knobJitter))
    {
        // Set the reading to min
        knobReading = minKnobReadingValue;
        debugPrintln(hw, "MIN!");
    }
    // Check for max value, accounting for jitter
    else if (newKnobReading >= (maxKnobReadingValue - knobJitter))
    {
        // Set the reading to max
        knobReading = maxKnobReadingValue;
        debugPrintln(hw, "MAX!");
    }
    // Standard reading
    else
    {
        knobReading = newKnobReading;
        //debugPrintlnF(hw, "%f", newKnobReading);
    }

    // Get the new value
    if (knobReading == maxKnobReadingValue)
    {
        newValue = maxValue;
    }
    else if (knobReading == minKnobReadingValue)
    {
        newValue = minValue;
    }
    else
    {
        newValue = (knobReading / maxKnobReadingValue) * (maxValue - minValue) + minValue;
    }

    // Return the new value
    return newValue;
}