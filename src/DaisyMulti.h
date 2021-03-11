#ifndef DAISY_MULTI_H
#define DAISY_MULTI_H

#include "../include/PedalConfig.h"
#include "DaisyDisplay.h"
#include "../include/IEffect.h"
#include "../lib/Inputs/Button.h"
#include "../lib/Inputs/Knob.h"
#include "../lib/Effects/CleanBoost.h"
#include "../lib/Effects/DaisyChorus.h"
#include "../lib/Effects/DaisyFlanger.h"
#include "../lib/Effects/DaisyTremolo.h"
#include "../lib/Effects/Crush.h"
#include "../lib/Effects/Distortion.h"
#include "../lib/Effects/Drive.h"
#include "../lib/Effects/AutoWah.h"
#include "../lib/Effects/Echo.h"
#include "../lib/Effects/DaisyCompressor.h"
#include "../lib/Effects/Reverb.h"
#include "../lib/Effects/DaisyPhaser.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;

// Declare a DaisySeed object called hw
DaisySeed *hw;

// Available Effects
#define AVAIL_EFFECTS 12
IEffect *availableEffects[AVAIL_EFFECTS] = {
    new CleanBoost(),
    new DaisyChorus(),
    new DaisyFlanger(),
    new DaisyTremolo(),
    new DaisyPhaser(),
    new AutoWah(),
    new Crush(),
    new Distortion(),
    new Drive(),
    new Echo(),
    new DaisyCompressor(),
    new Reverb()};

// Flash storage parameters
uint32_t memBase = 0x90000000;
static EffectStorage DSY_QSPI_BSS effectsStorage[MAX_EFFECTS];
EffectStorage effectsStorageBuffer[MAX_EFFECTS];

// Pedal state and effects objects
volatile PedalState currentState = PedalState::PLAY_MODE;
volatile PedalState newState = PedalState::PLAY_MODE;
volatile u_int8_t selectedEditEffect = -1;
char *currentEffectNames[MAX_EFFECTS];
volatile int currentEffects[MAX_EFFECTS];
volatile int newEffects[MAX_EFFECTS];
bool currentEffectsState[MAX_EFFECTS] = {false};

// Effect controls
Button effectButtons[MAX_EFFECTS];
Led effectLeds[MAX_EFFECTS];

// Control Encoder
Encoder controlEncoder;

// OLED display
DaisyDisplay display;

// Output volume control
const float outputLevelIncrement = 0.4f;
const float outputLevelMin = 0.0f;
const float outputLevelMax = 10.0f;
float outputLevel = 5.0f;
float newOutputLevel = 5.0f;

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