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

// Pedal state and effect objects
volatile PedalState currentState = PedalState::PLAY_MODE;
IEffect *effect1;
IEffect *effect2;
IEffect *effect3;
IEffect *effect4;

// Buttons
Button controlButton;
Button effect1Button;
Button effect2Button;
Button effect3Button;
Button effect4Button;

// LEDs
Led controlLed;
Led effect1Led;
Led effect2Led;
Led effect3Led;
Led effect4Led;

// Output volume control
Knob outputVolume;
const float outputLevelMin = 0.0f;
const float outputLevelMax = 5.0f;
float outputLevel = 1.0f;

// Effect on/off booleans
bool isEffect1On = false;
bool isEffect2On = false;
bool isEffect3On = false;
bool isEffect4On = false;

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
 * Updates the effect LEDs, turning them on and off based on the current state
 */
void UpdateEffectLeds();

/**
 * Handles reading the volume knob and setting the output level
 */
void HandleOutputVolumeControl();

#endif