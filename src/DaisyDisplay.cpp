#include "DaisyDisplay.h"

/**
 * Initializes the OLED display
 */
void DaisyDisplay::Init(DaisySeed *hardware)
{
    hw = hardware;

    oled_pins[OledDisplay::DATA_COMMAND] = hw->GetPin(oledDCPin);
    oled_pins[OledDisplay::RESET] = hw->GetPin(oledResetPin);
    display.Init(oled_pins);
}

/**
 * Displays the currently selected effects as part of play mode
 */
void DaisyDisplay::DisplayPlayModeEffects(char *currentEffectNames[MAX_EFFECTS])
{
    // Erase the screen
    display.Fill(false);

    // Write each effect name
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        char buffer[MAX_MESSAGE_LEN];
        snprintf(buffer, MAX_MESSAGE_LEN, "%d - ", i + 1);
        display.SetCursor(0, i * LINE_HEIGHT);
        display.WriteString(buffer, FONT_SIZE, true);
        display.SetCursor(CHAR_WIDTH * 4, i * LINE_HEIGHT);
        display.WriteString(currentEffectNames[i], FONT_SIZE, true);
    }

    // Update the view
    display.Update();

    // Write out the output volume
}

/**
 * Clears the screen and writes the formatted message to the OLED
 */
void DaisyDisplay::WriteMessage(char *message, ...)
{
    char buffer[MAX_MESSAGE_LEN];
    va_list args;
    va_start(args, message);
    vsnprintf(buffer, MAX_MESSAGE_LEN, message, args);
    va_end(args);

    display.Fill(false);
    display.SetCursor(0, 0);
    display.WriteString(buffer, FONT_SIZE, true);
    display.Update();
}

/**
 * Clears the OLED screen
 */
void DaisyDisplay::ClearScreen()
{
    display.Fill(false);
    display.Update();
}