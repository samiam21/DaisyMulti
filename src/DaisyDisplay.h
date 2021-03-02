#ifndef DAISY_DISPLAY_H
#define DAISY_DISPLAY_H

#include <cstdarg>
#include "daisy_seed.h"
#include "../include/PedalConfig.h"

using namespace daisy;

#define MAX_MESSAGE_LEN 255

/**
 * Class for handling writing data out to the OLED display
 */
class DaisyDisplay
{
public:
    /**
     * Initializes the OLED display
     */
    void Init(DaisySeed *hardware);

    /**
     * Clears the screen and writes the formatted message to the OLED
     */
    void WriteMessage(char *message, ...);

    /**
     * Clears the OLED screen
     */
    void ClearScreen();

private:
    DaisySeed *hw;
    OledDisplay display;
    dsy_gpio_pin oled_pins[OledDisplay::NUM_PINS];
};

#endif