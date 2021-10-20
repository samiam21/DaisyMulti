#ifndef DAISY_MULTI_H
#define DAISY_MULTI_H

#include "../include/PedalConfig.h"
#include "DaisyDisplay.h"
#include "../lib/Helpers/TempoArray.h"
#include "../lib/DaisyEffects/IEffect.h"
#include "../include/Effects.h"
#include "../lib/Inputs/Button.h"
#include "../lib/Inputs/Knob.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;

// Declare a DaisySeed object called hw
DaisySeed *hw;

// Flash storage parameters
uint32_t memBase = 0x90000000;
static EffectStorage DSY_QSPI_BSS effectsStorage[MAX_EFFECTS];
EffectStorage effectsStorageBuffer[MAX_EFFECTS];

// State enum for the system
enum PedalState
{
    PLAY_MODE = 0,
    PLAY_CHANGE_VOLUME = 1,
    EDIT_MODE = 10,
    EDIT_CHANGE_EFFECT = 11
};

// Pedal state and effects objects
volatile PedalState currentState = PedalState::PLAY_MODE;
volatile PedalState newState = PedalState::PLAY_MODE;
volatile u_int8_t selectedEditEffect = -1;
char *currentEffectNames[MAX_EFFECTS];
IEffect *currentEffects[MAX_EFFECTS];
EffectType newEffects[MAX_EFFECTS];
bool currentEffectsState[MAX_EFFECTS] = {false};
volatile float effectChangeInc = 0.0f;

// Effect controls
Button effectButtons[MAX_EFFECTS];
Led effectLeds[MAX_EFFECTS];

// Tap tempo
Button tapTempoButton;
TempoArray tempoArray;
unsigned long tapTempoTime = 0;
unsigned long tapTempoAvg = 0;
int tapTempoBpm = 0;

// Control Encoder
Encoder controlEncoder;

// OLED display
DaisyDisplay display;

// Output volume control
const float outputLevelIncrement = 0.2f;
const float outputLevelMin = 0.0f;
const float outputLevelMax = 5.0f;
float outputLevel = 1.0f;
volatile float outputChangeInc = 0.0f;

/**
 * Audio callback to process each enabled effect
 */
void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size);

/**
 * Initializes the input controls
 */
void InitializeControls();

/**
 * Initializes the effects
 */
void InitializeEffects();

/**
 * Handles the effect buttons, turning on and off the effects
 */
void HandleEffectButtons();

/**
 * Handles control of the pedal state
 */
void HandlePedalState();

/**
 * Reads the control encoder and triggers actions
 */
void ControlEncoderInterrupt();

/**
 * Updates the effect LEDs, turning them on and off based on the current state
 */
void UpdateEffectLeds();

/**
 * Saves the current effect settings to flash
 */
void SaveCurrentEffectSettings();

#endif