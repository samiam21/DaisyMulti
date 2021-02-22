#ifndef DAISY_MULTI_H
#define DAISY_MULTI_H

#include "../include/PedalConfig.h"
#include "../include/IEffect.h"
#include "../include/EffectType.h"
#include "../lib/Inputs/Button.h"
#include "../lib/Inputs/Knob.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;

// Declare a DaisySeed object called hw
DaisySeed *hw;

// Flash storage parameters
uint32_t memBase = 0x90000000;
static IEffect DSY_QSPI_BSS effectsStorage[MAX_EFFECTS];

// Pedal state and effects objects
volatile PedalState currentState = PedalState::PLAY_MODE;
volatile u_int8_t selectedEditEffect = -1;
IEffect *currentEffects[MAX_EFFECTS];
bool currentEffectsState[MAX_EFFECTS] = {false};

// Effect controls
Button effectButtons[MAX_EFFECTS];
Led effectLeds[MAX_EFFECTS];

// Control button and LED
Button controlButton;
Led controlLed;

// Effect Selector
Encoder selector;

// Output volume control
Knob outputVolume;
const float outputLevelMin = 0.0f;
const float outputLevelMax = 5.0f;
float outputLevel = 1.0f;

/**
 * Audio callback to process each enabled effect
 */
void AudioCallback(float **in, float **out, size_t size);

/**
 * Initializes the input controls
 */
void InitializeControls();

/**
 * Initializes the effects
 */
void InitializeEffects();

/**
 * Handles the control button
 */
void HandleControlButton();

/**
 * Handles the effect buttons, turning on and off the effects
 */
void HandleEffectButtons();

/**
 * Handles control of the effect selector, only enabled in edit mode
 */
void HandleEffectSelector();

/**
 * Updates the effect LEDs, turning them on and off based on the current state
 */
void UpdateEffectLeds();

/**
 * Handles reading the volume knob and setting the output level
 */
void HandleOutputVolumeControl();

/**
 * Saves the current effect settings to flash
 */
void SaveCurrentEffectSettings();

/**
 * Reads the effect settings from flash
 */
void ReadCurrentEffectSettings();

#endif