#include "KnobTest.h"

void KnobTest::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

    // Initialize the knobs
    knob1.Init(hw, KNOB_1_CHN, led1Value);
    knob2.Init(hw, KNOB_2_CHN, led2Value);
    knob3.Init(hw, KNOB_3_CHN, knob3Value, minKnob3Value, maxKnob3Value);
    knob4.Init(hw, KNOB_4_CHN, knob4Value, minKnob4Value, maxKnob4Value);

    // Initialize the LEDs
    led1.Init(hw->GetPin(effectLedPin1), false);
    led2.Init(hw->GetPin(effectLedPin2), false);
}

void KnobTest::AudioCallback(float **in, float **out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        out[AUDIO_OUT_CH][i] = in[AUDIO_IN_CH][i];
    }
}

void KnobTest::Cleanup()
{
    // Turn off the LEDs
    led1.Set(0);
    led1.Update();
    led2.Set(0);
    led2.Update();
}

void KnobTest::Loop()
{
    // Knob 1 controls intensity of LED 1
    if (knob1.SetNewValue(led1Value))
    {
        // Write the new value to the LED
        led1.Set(led1Value);
        led1.Update();

        debugPrintlnF(hw, "Updated the LED 1 level to: %f", led1Value);
    }

    // Knob 2 controls intensity of LED 2
    if (knob2.SetNewValue(led2Value))
    {
        // Write the new value to the LED
        led2.Set(led2Value);
        led2.Update();

        debugPrintlnF(hw, "Updated the LED 2 level to: %f", led2Value);
    }

    // Knob 3 prints out a value
    if (knob3.SetNewValue(knob3Value))
    {
        debugPrintlnF(hw, "Updated Knob 3 to: %f", knob3Value);
    }

    // Knob 4 prints out a value
    if (knob4.SetNewValue(knob4Value))
    {
        debugPrintlnF(hw, "Updated Knob 4 to: %f", knob4Value);
    }
}

char *KnobTest::GetEffectName()
{
    return (char *)"Knob Test";
}