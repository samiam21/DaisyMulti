#ifndef PEDAL_CONFIG_H
#define PEDAL_CONFIG_H

#include "daisy_seed.h"

#define DEBUG 1

#define WAIT_FOR_SERIAL false
#define ENABLE_DISPLAY true

#define DAISY_BLOCKSIZE 48
#define DAISY_SAMPLE_RATE daisy::SaiHandle::Config::SampleRate::SAI_48KHZ

// Audio channels at 96KHz
//#define AUDIO_IN_CH 1
//#define AUDIO_OUT_CH 0

// Audio channels at 48KHz
#define AUDIO_IN_CH 0
#define AUDIO_OUT_CH 0

#define MAX_EFFECTS 6

#define MAX_KNOBS 4
#define KNOB_1_CHN 0
#define KNOB_2_CHN 1
#define KNOB_3_CHN 2
#define KNOB_4_CHN 3

#define PI_VAL 3.14159265

/** Serial debug print macros **/
#define initDebugPrint(hw, pcBlock)                                                                    \
    {                                                                                                  \
        if (DEBUG)                                                                                     \
            hw->StartLog(pcBlock); /* true == wait for PC: will block until a terminal is connected */ \
    }
#define debugPrintln(hw, msg)   \
    {                           \
        if (DEBUG)              \
            hw->PrintLine(msg); \
    }
#define debugPrint(hw, msg) \
    {                       \
        if (DEBUG)          \
            hw->Print(msg); \
    }
#define debugPrintlnF(hw, msg, args...) \
    {                                   \
        if (DEBUG)                      \
            hw->PrintLine(msg, args);   \
    }
#define debugPrintF(hw, msg, args...) \
    {                                 \
        if (DEBUG)                    \
            hw->Print(msg, args);     \
    }

/** OLED display macros **/
#define initDisplay(hw, display) \
    {                            \
        if (ENABLE_DISPLAY)      \
            display.Init(hw);    \
    }
#define updateOutputLevel(display, outputLevel)     \
    {                                               \
        if (ENABLE_DISPLAY)                         \
            display.UpdateOutputLevel(outputLevel); \
    }
#define updatePlayModeEffects(display, currentEffectNames)     \
    {                                                          \
        if (ENABLE_DISPLAY)                                    \
            display.UpdatePlayModeEffects(currentEffectNames); \
    }
#define showEditModeStartupScreen(display)       \
    {                                            \
        if (ENABLE_DISPLAY)                      \
            display.ShowEditModeStartupScreen(); \
    }
#define showEditModeEffectScreen(display, effectName, knobNames)     \
    {                                                                \
        if (ENABLE_DISPLAY)                                          \
            display.ShowEditModeEffectScreen(effectName, knobNames); \
    }
#define updateEditModeToggleValue(display, value)      \
    {                                                  \
        if (ENABLE_DISPLAY)                            \
            display->UpdateEditModeToggleValue(value); \
    }

// State enum for the system
enum PedalState
{
    PLAY_MODE = 0,
    EDIT_MODE = 10
};

struct EffectSettings
{
    float knobSettings[MAX_KNOBS];
    u_int8_t togglePosition;
};

struct EffectStorage
{
    int availableEffectsPosition;
    EffectSettings effectSettings;
};

// /** DEBUG **/
// const int effectSelectorPinA = 13;
// const int effectSelectorPinB = 12;
// const int effectSelectorPinSw = 14;

// const int effectSPDT1Pin1 = 15;
// const int effectSPDT1Pin2 = 17;

// const int effectSPSTPins[MAX_EFFECTS] = {23};
// const int effectLedPins[MAX_EFFECTS] = {16};
// /** DEBUG **/

// Pin Definitions - Selector
const int effectSelectorPinA = 27;
const int effectSelectorPinB = 26;
const int effectSelectorPinSw = 28;

// Pin Definitions - SPST
const int effectSPSTPins[MAX_EFFECTS] = {1, 3, 5, 0, 2, 4};
const int tapButtonPin = 30;

// Pin Definitions - SPDT
const int effectSPDT1Pin1 = 12;
const int effectSPDT1Pin2 = 13;

// Pin Definitions - POT
const int effectPotPin1 = 21;
const int effectPotPin2 = 22;
const int effectPotPin3 = 23;
const int effectPotPin4 = 24;

// Pin Definitions - LED
const int effectLedPins[MAX_EFFECTS] = {19, 17, 15, 20, 18, 16};

// Pin Definitions - OLED
const int oledResetPin = 11;
const int oledDCPin = 9;

#endif