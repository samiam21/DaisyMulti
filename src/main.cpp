#include "../include/PedalConfig.h"
#include "../include/IEffect.h"
#include "../include/EffectType.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;

// Declare a DaisySeed object called hw
DaisySeed *hw;

// Effect switching parameters
volatile EffectType selectedEffectType = UNSET;
volatile PedalState currentState = PedalState::PLAY_MODE;
IEffect *currentEffect;

// HIDs
Switch selectorPin1;
Switch selectorPin2;
Switch selectorPin3;
Switch selectorPin4;
Button controlButton;
Led controlLed;

/**
 * Sets the selected effect type based on reading the selector
 */
bool ReadSelectedEffect()
{
    // Read the state of the encoder pins
    uint32_t pin1 = (uint32_t)selectorPin1.RawState();
    uint32_t pin2 = (uint32_t)selectorPin2.RawState();
    uint32_t pin3 = (uint32_t)selectorPin3.RawState();
    uint32_t pin4 = (uint32_t)selectorPin4.RawState();

    // Get the combined value and set the effect type
    uint32_t combined = pin4 | (pin3 << 1) | (pin2 << 2) | (pin1 << 3);

    if ((EffectType)combined != selectedEffectType)
    {
        selectedEffectType = (EffectType)(combined);
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Audio callback to process each enabled effect
 */
void AudioCallback(float **in, float **out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        out[AUDIO_OUT_CH][i] = currentEffect->Process(in[AUDIO_IN_CH][i]);
    }
}

/**
 * Handles the control button
 */
void HandleControlButton()
{
    // // Check if we are currently in play mode and the control button is held
    // if (currentState == PedalState::PLAY_MODE && controlButton.IsHeld())
    // {
    //     // Switch to edit mode
    //     debugPrintln(hw, "Switching to edit mode!");
    //     currentState = PedalState::EDIT_MODE;

    //     // Turn on the control LED
    //     controlLed.Set(1.f);
    //     controlLed.Update();
    // }
    // // Check if we are currently in edit mode and the control button is pressed
    // else if (currentState == PedalState::EDIT_MODE && controlButton.IsPressed())
    // {
    //     // Switch back to play mode
    //     debugPrintln(hw, "Switching to play mode!");
    //     currentState = PedalState::PLAY_MODE;

    //     // Turn off the control LED
    //     controlLed.Set(0);
    //     controlLed.Update();
    // }
}

/**
 * Main Loop
 */
int main(void)
{
    // Configure and Initialize the Daisy Seed
    hw->Configure();
    hw->Init();

    // Initialize debug printing (true = wait for COM connection before continuing)
    initDebugPrint(hw, WAIT_FOR_SERIAL);
    debugPrintln(hw, "Starting DaisyPedal...");

    // Update the block size and sample rate to minimize noise
    hw->SetAudioBlockSize(BLOCKSIZE);
    hw->SetAudioSampleRate(DAISY_SAMPLE_RATE);

    // Initialize the 4 knobs
    AdcChannelConfig adcConfig[4];
    adcConfig[KNOB_1_CHN].InitSingle(hw->GetPin(effectPotPin1));
    adcConfig[KNOB_2_CHN].InitSingle(hw->GetPin(effectPotPin2));
    adcConfig[KNOB_3_CHN].InitSingle(hw->GetPin(effectPotPin3));
    adcConfig[KNOB_4_CHN].InitSingle(hw->GetPin(effectPotPin4));
    hw->adc.Init(adcConfig, 4);
    hw->adc.Start();

    // Initialize the control button and LED
    // controlButton.Init(hw, hw->GetPin(effectSPSTPin4), 3000);
    // controlLed.Init(hw->GetPin(effectLedPin4), false);

#ifndef BYPASS_SELECTOR
    // Initialize the selector pins
    selectorPin1.Init(hw->GetPin(effectSelectorPin1), 1, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, Switch::Pull::PULL_UP);
    selectorPin2.Init(hw->GetPin(effectSelectorPin2), 1, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, Switch::Pull::PULL_UP);
    selectorPin3.Init(hw->GetPin(effectSelectorPin3), 1, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, Switch::Pull::PULL_UP);
    selectorPin4.Init(hw->GetPin(effectSelectorPin4), 1, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, Switch::Pull::PULL_UP);

    // Read the selected effect
    ReadSelectedEffect();
#endif

    // Set the current effect
    currentEffect = GetEffectObject(selectedEffectType);

    // Start the audio processing
    debugPrintln(hw, "Starting Audio");
    hw->StartAudio(AudioCallback);

    // Turn on the onboard LED
    hw->SetLed(true);

    // Loop forever
    for (;;)
    {
#ifndef BYPASS_SELECTOR
        // Check if we have a new effect type and switch to the new state
        if (ReadSelectedEffect())
        {
            // Clean up and stop the old effect
            currentEffect->Cleanup();

            // Get the new effect object
            currentEffect = GetEffectObject(selectedEffectType);

            // Start the new effect
            debugPrintlnF(hw, "Switching To: %s", currentEffect->GetEffectName());
            currentEffect->Setup(hw);
        }
#endif

        // Handle the control button
        HandleControlButton();

        // Execute the effect loop commands
        currentEffect->Loop(currentState);
    }
}