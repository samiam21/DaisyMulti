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
    display.WriteString(buffer, Font_7x10, true);
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