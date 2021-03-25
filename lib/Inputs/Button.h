#ifndef BUTTON_H
#define BUTTON_H

#include "daisy_seed.h"
#include "../../include/PedalConfig.h"

using namespace daisy;

/**
 * Button class to handle reading a button value while debouncing it
 * This class will initialize the provided pin in the init function
 */
class Button
{
public:
    /**
     * Initialize the button with print capabilities
     */
    void Init(DaisySeed *hardware, dsy_gpio_pin pin, u_int16_t holdTime = 2000, unsigned long debounceTime = 200);

    /**
     * Checks if the button is pressed, accounting for debounce
     * @return Returns true if the button is pressed, false if not
     */
    bool IsPressed(bool debounce = true);

    /**
     * Checks if the button has been held longer than the configured hold time
     * @return Returns true if the button is held, false if not
     */
    bool IsHeld();

private:
    dsy_gpio_pin buttonPin;
    DaisySeed *hw;

    unsigned long buttonDebounce = 200;
    unsigned long lastButtonPress = 0;
    u_int16_t buttonHoldTime = 2000;

    Switch button;
};

#endif