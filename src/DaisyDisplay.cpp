#include "DaisyDisplay.h"

/**
 * Initializes the OLED display
 */
void DaisyDisplay::Init(DaisySeed *hardware)
{
    hw = hardware;

    /** Configure the Display */
    MyOledDisplay::Config disp_cfg;
    disp_cfg.driver_config.transport_config.pin_config.dc = hw->GetPin(oledDCPin);
    disp_cfg.driver_config.transport_config.pin_config.reset = hw->GetPin(oledResetPin);
    /** And Initialize */
    display.Init(disp_cfg);
}

/**
 * Displays the currently selected effects as part of play mode
 */
void DaisyDisplay::UpdatePlayModeEffects(char *currentEffectNames[MAX_EFFECTS])
{
    // Erase the effects portion of the screen
    display.DrawRect(0, 0, SSD1309_WIDTH, 54, false, true);

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
    display.SetCursor(15, 23);
    display.WriteString("EDIT MODE", FONT_SIZE_LARGE, true);

    // Display the edit mode subtitle text
    display.SetCursor(16, 44);
    display.WriteString("SELECT AN EFFECT", FONT_SIZE, true);

    display.Update();
}

/**
 * Displays the screen for editing an effect
 */
void DaisyDisplay::ShowEditModeEffectScreen(char *effectName, char **knobNames)
{
    // Clear the entire screen
    display.Fill(false);

    // Display the effect name
    display.SetCursor(0, 0);
    display.WriteString(effectName, FONT_SIZE, true);

    // Display the edit mode indicator
    display.SetCursor(104, 0);
    display.WriteString("EDIT", FONT_SIZE, true);

    // Title separator
    display.DrawLine(0, 9, SSD1309_WIDTH, 9, true);

    // Knob A info
    display.DrawCircle(10, 20, 6, true);
    display.SetCursor(8, 17);
    display.WriteChar('A', FONT_SIZE, true);
    display.SetCursor(19, 17);
    display.WriteString(knobNames[0], FONT_SIZE, true);

    // Knob B info
    display.DrawCircle(72, 20, 6, true);
    display.SetCursor(70, 17);
    display.WriteChar('B', FONT_SIZE, true);
    display.SetCursor(81, 17);
    display.WriteString(knobNames[1], FONT_SIZE, true);

    // Knob C info
    display.DrawCircle(10, 37, 6, true);
    display.SetCursor(8, 34);
    display.WriteChar('C', FONT_SIZE, true);
    display.SetCursor(19, 34);
    display.WriteString(knobNames[2], FONT_SIZE, true);

    // Knob D info
    display.DrawCircle(72, 37, 6, true);
    display.SetCursor(70, 34);
    display.WriteChar('D', FONT_SIZE, true);
    display.SetCursor(81, 34);
    display.WriteString(knobNames[3], FONT_SIZE, true);

    // Toggle info
    display.DrawCircle(10, 54, 4, true);
    display.DrawLine(5, 49, 10, 54, true);

    // Clear the value section
    display.DrawRect(SSD1309_WIDTH - 44, 51, SSD1309_WIDTH, SSD1309_HEIGHT, false, true);

    display.Update();
}

/**
 * Updates the toggle value in the edit effect screen
 */
void DaisyDisplay::UpdateEditModeToggleValue(char *newValue)
{
    // Erase the current value
    display.DrawRect(19, 51, SSD1309_WIDTH - 45, SSD1309_HEIGHT, false, true);

    // Draw new value
    display.SetCursor(19, 51);
    display.WriteString(newValue, FONT_SIZE, true);

    display.Update();
}

/**
 * Updates the knob value display in the edit effect screen
 */
void DaisyDisplay::UpdateEditModeKnobValue(int knobPosition, float newValue)
{
    // Erase the current value
    display.DrawRect(SSD1309_WIDTH - 44, 51, SSD1309_WIDTH, SSD1309_HEIGHT, false, true);

    // Write the knob indicator
    display.SetCursor(SSD1309_WIDTH - 44, 51);
    switch (knobPosition)
    {
    case 0:
        display.WriteChar('A', FONT_SIZE, true);
        break;
    case 1:
        display.WriteChar('B', FONT_SIZE, true);
        break;
    case 2:
        display.WriteChar('C', FONT_SIZE, true);
        break;
    case 3:
        display.WriteChar('D', FONT_SIZE, true);
        break;
    }
    display.SetCursor(SSD1309_WIDTH - 38, 51);
    display.WriteChar('-', FONT_SIZE, true);

    // Write the new value
    char buffer[6];
    sprintf(buffer, "%.2f", newValue);
    display.SetCursor(SSD1309_WIDTH - 32, 51);
    display.WriteString(buffer, FONT_SIZE, true);

    display.Update();
}

/**
 * Updates the knob value display in the edit effect screen
 */
void DaisyDisplay::UpdateEditModeKnobValueI(int knobPosition, int newValue)
{
    // Erase the current value
    display.DrawRect(SSD1309_WIDTH - 44, 51, SSD1309_WIDTH, SSD1309_HEIGHT, false, true);

    // Write the knob indicator
    display.SetCursor(SSD1309_WIDTH - 44, 51);
    switch (knobPosition)
    {
    case 0:
        display.WriteChar('A', FONT_SIZE, true);
        break;
    case 1:
        display.WriteChar('B', FONT_SIZE, true);
        break;
    case 2:
        display.WriteChar('C', FONT_SIZE, true);
        break;
    case 3:
        display.WriteChar('D', FONT_SIZE, true);
        break;
    }
    display.SetCursor(SSD1309_WIDTH - 38, 51);
    display.WriteChar('-', FONT_SIZE, true);

    // Write the new value
    char buffer[6];
    sprintf(buffer, "%d", newValue);
    display.SetCursor(SSD1309_WIDTH - 32, 51);
    display.WriteString(buffer, FONT_SIZE, true);

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