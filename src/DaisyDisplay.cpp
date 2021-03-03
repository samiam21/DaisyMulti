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
void DaisyDisplay::UpdatePlayModeEffects(char *currentEffectNames[MAX_EFFECTS])
{
    // Erase the effects portion of the screen
    display.DrawRect(0, 0, SSD1309_WIDTH, LINE_HEIGHT * MAX_EFFECTS, false, true);

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
}

/**
 * Displays the output level in the corner of the display
 */
void DaisyDisplay::UpdateOutputLevel(float outputLevel)
{
    // Clear the output volume box
    display.DrawRect(0, 55, SSD1309_WIDTH, SSD1309_HEIGHT, false, true);

    // Draw the outer box
    display.DrawRect(1, 56, SSD1309_WIDTH - 1, SSD1309_HEIGHT - 1, true, false);

    // Draw the output level
    int levelWidth = (int)(24.4f * outputLevel);
    display.DrawRect(3, 58, levelWidth + 3, SSD1309_HEIGHT - 3, true, true);

    // Update the view
    display.Update();
}

/**
 * Displays the startup screen for edit mode
 */
void DaisyDisplay::ShowEditModeStartupScreen()
{
    // Clear the entire screen
    display.Fill(false);

    // Display the edit mode title text
    display.SetCursor(15,23);
    display.WriteString("EDIT MODE", FONT_SIZE_LARGE, true);

    // Display the edit mode subtitle text
    display.SetCursor(16,44);
    display.WriteString("SELECT AN EFFECT", FONT_SIZE, true);

    display.Update();
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