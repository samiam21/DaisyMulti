#include "EncoderKnob.h"

void EncoderKnob::Init(DaisySeed *hardware, dsy_gpio_pin pinA, dsy_gpio_pin pinB, dsy_gpio_pin pinSw, unsigned long debounceTime)
{
    // Set the hardware pointer
    hw = hardware;

    // Set the pins for the class
    _pinA = pinA;
    _pinB = pinB;
    _pinSw = pinSw;

    // Set the debounce time
    buttonDebounce = debounceTime;

    // Initialize the encoder
    encoder.Init(_pinA, _pinB, _pinSw, hw->AudioCallbackRate());
}

bool EncoderKnob::IsPressed(bool debounce)
{
    bool ret = false;

    // Read the button
    encoder.Debounce();
    bool reading = encoder.Pressed();

    // Check for debounce command
    if (debounce)
    {
        // Debounce the button and check for it pressed
        uint32_t millis = System::GetNow();
        if (millis - lastButtonPress > buttonDebounce)
        {
            // Update last pressed time and set the return
            lastButtonPress = millis;
            ret = reading;
        }
    }
    else
    {
        // Check if the button is pressed
        ret = reading;
    }

    return ret;
}

int EncoderKnob::Increment()
{
    int ret = 0;

    // Read the increment
    encoder.Debounce();
    int increment = encoder.Increment();

    ret = increment;

    return ret;
}