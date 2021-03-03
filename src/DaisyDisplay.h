#ifndef DAISY_DISPLAY_H
#define DAISY_DISPLAY_H

#include <cstdarg>
#include "daisy_seed.h"
#include "../include/PedalConfig.h"

using namespace daisy;

#define MAX_MESSAGE_LEN 255
#define FONT_SIZE Font_6x8
#define LINE_HEIGHT 9
#define CHAR_WIDTH 7

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
     * Displays the currently selected effects as part of play mode
     */
    void DisplayPlayModeEffects(char *currentEffectNames[MAX_EFFECTS]);

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