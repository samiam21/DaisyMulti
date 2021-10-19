#ifndef DAISY_DISPLAY_H
#define DAISY_DISPLAY_H

#include <cstdarg>
#include "daisy_seed.h"
#include "dev/oled_ssd130x.h"
#include "../include/PedalConfig.h"

using namespace daisy;
using MyOledDisplay = OledDisplay<SSD130x4WireSpi128x64Driver>;

#define MAX_MESSAGE_LEN 255
#define FONT_SIZE Font_6x8
#define FONT_SIZE_LARGE Font_11x18
#define LINE_HEIGHT 9
#define CHAR_WIDTH 7
#define SSD1309_WIDTH 128
#define SSD1309_HEIGHT 64

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
    void UpdatePlayModeEffects(char *currentEffectNames[MAX_EFFECTS]);

    /**
     * Displays the output level in the corner of the display
     */
    void UpdateOutputLevel(float outputLevel);

    /**
     * Displays the startup screen for edit mode
     */
    void ShowEditModeStartupScreen();

    /**
     * Displays the screen for editing an effect
     */
    void ShowEditModeEffectScreen(char *effectName, char **knobNames);

    /**
     * Updates the toggle value in the edit effect screen
     */
    void UpdateEditModeToggleValue(char *newValue);

    /**
     * Updates the knob value display in the edit effect screen
     */
    void UpdateEditModeKnobValue(int knobPosition, float newValue);

    /**
     * Updates the knob value display in the edit effect screen
     */
    void UpdateEditModeKnobValueI(int knobPosition, int newValue);

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
    MyOledDisplay display;
};

#endif