#include "ButtonTest.h"

void ButtonTest::Setup(daisy::DaisySeed *hardware)
{
    hw = hardware;

    // Initialize the buttons
    button1.Init(hw, hw->GetPin(effectSPSTPin1));
    button2.Init(hw, hw->GetPin(effectSPSTPin2));
    button3.Init(hw, hw->GetPin(effectSPSTPin3));
    button4.Init(hw, hw->GetPin(effectSPSTPin4));

    // Initialize the LEDs
    led1.Init(hw->GetPin(effectLedPin1), false);
    led2.Init(hw->GetPin(effectLedPin2), false);
    led3.Init(hw->GetPin(effectLedPin3), false);
    led4.Init(hw->GetPin(effectLedPin4), false);

    led4.Set(isMuted ? 0 : 1.f);
    led4.Update();
}

void ButtonTest::AudioCallback(float **in, float **out, size_t size)
{
    if (!isMuted)
    {
        for (size_t i = 0; i < size; i++)
        {
            out[AUDIO_OUT_CH][i] = in[AUDIO_IN_CH][i];
        }
    }
}

void ButtonTest::Cleanup()
{
    // Turn off the LEDs
    led1.Set(0);
    led1.Update();
    led2.Set(0);
    led2.Update();
    led3.Set(0);
    led3.Update();
    led4.Set(0);
    led4.Update();
}

void ButtonTest::Loop()
{
    // Button 1 turns on LED 1
    if (button1.IsPressed(false))
    {
        led1.Set(1.0f);
        led1.Update();
    }
    else
    {
        led1.Set(0);
        led1.Update();
    }

    // Button 2 turns on LED 2
    if (button2.IsPressed(false))
    {
        led2.Set(1.0f);
        led2.Update();
    }
    else
    {
        led2.Set(0);
        led2.Update();
    }

    // Button 3 toggles LED 3 with a long press
    if (button3.IsHeld())
    {
        led3.Set(1.0f);
        led3.Update();

        debugPrintln(hw, "Button 3 held");
    }
    else
    {
        led3.Set(0);
        led3.Update();
    }

    // Button 4 toggles LED 4
    if (button4.IsPressed())
    {
        isMuted = !isMuted;
        led4.Set(isMuted ? 0 : 1.f);
        led4.Update();
    }
}

char *ButtonTest::GetEffectName()
{
    return (char *)"Button Test";
}