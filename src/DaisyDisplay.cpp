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
 * Clears the screen and writes a message to the OLED
 */
void DaisyDisplay::WriteMessage(char *message)
{
    display.Fill(true);
    display.SetCursor(0, 0);
    display.WriteString(message, Font_7x10, false);
    display.Update();
}

/**
 * Clears the OLED screen
 */
void DaisyDisplay::ClearScreen()
{
    display.Fill(true);
    display.Update();
}