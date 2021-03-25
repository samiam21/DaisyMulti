#include "Button.h"

void Button::Init(DaisySeed *hardware, dsy_gpio_pin pin, u_int16_t holdTime, unsigned long debounceTime)
{
    // Set the hardware pointer
    hw = hardware;

    // Set the button pin for the class
    buttonPin = pin;

    // Set the hold time and debounce time
    buttonHoldTime = holdTime;
    buttonDebounce = debounceTime;

    // Configure the button pin with the specific mode
    button.Init(pin, hw->AudioCallbackRate(), Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, Switch::Pull::PULL_UP);
}

bool Button::IsPressed(bool debounce)
{
    bool ret = false;

    // Read the button
    bool reading = button.RawState();

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

bool Button::IsHeld()
{
    bool ret = false;

    // Debounce the button
    button.Debounce();

    // Check for the button being held
    float heldTime = button.TimeHeldMs();
    if ((u_int16_t)heldTime >= buttonHoldTime)
    {
        ret = true;
    }

    return ret;
}