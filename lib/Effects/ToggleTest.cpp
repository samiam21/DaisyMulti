#include "ToggleTest.h"

void ToggleTest::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

    // Initialize the toggle
    toggle.Init(hw->GetPin(effectSPDT1Pin1), hw->GetPin(effectSPDT1Pin2));

    // Initialize the LEDs
    led1.Init(hw->GetPin(effectLedPin1), false);
    led2.Init(hw->GetPin(effectLedPin2), false);
    led3.Init(hw->GetPin(effectLedPin3), false);
}

void ToggleTest::AudioCallback(float **in, float **out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        out[AUDIO_OUT_CH][i] = in[AUDIO_IN_CH][i];
    }
}

void ToggleTest::Cleanup()
{
    // Turn off the LEDs
    led1.Set(0);
    led1.Update();
    led2.Set(0);
    led2.Update();
    led3.Set(0);
    led3.Update();
}

void ToggleTest::Loop()
{
    // Read the toggle
    if (toggle.ReadToggle() == 0)
    {
        // Turn on LED 1
        led1.Set(1.0f);
        led1.Update();

        // Turn off LED 2 & 3
        led2.Set(0.0f);
        led2.Update();
        led3.Set(0.0f);
        led3.Update();
    }
    else if (toggle.ReadToggle() == 2)
    {
        // Turn on LED 3
        led3.Set(1.0f);
        led3.Update();

        // Turn off LED 1 & 2
        led1.Set(0.0f);
        led1.Update();
        led2.Set(0.0f);
        led2.Update();
    }
    else
    {
        // Turn on LED 2
        led2.Set(1.0f);
        led2.Update();

        // Turn off LED 1 & 3
        led1.Set(0.0f);
        led1.Update();
        led3.Set(0.0f);
        led3.Update();
    }
}

char *ToggleTest::GetEffectName()
{
    return (char *)"Toggle Test";
}