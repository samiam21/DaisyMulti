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
IEffect *currentEffect;

Switch selectorPin1;
Switch selectorPin2;
Switch selectorPin3;
Switch selectorPin4;

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

    // Start the effect
    debugPrintlnF(hw, "Starting: %s", currentEffect->GetEffectName());
    currentEffect->Setup(hw);
    hw->StartAudio((AudioHandle::AudioCallback)[](float **in, float **out, size_t size) { return currentEffect->AudioCallback(in, out, size); });

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
            hw->StartAudio((AudioHandle::AudioCallback)[](float **in, float **out, size_t size) { return currentEffect->AudioCallback(in, out, size); });
        }
#endif

        // Execute the effect loop commands
        currentEffect->Loop();
    }
}