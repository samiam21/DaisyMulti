#include "DaisyMulti.h"

/**
 * Audio callback to process each enabled effect
 */
void AudioCallback(float **in, float **out, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        float wet = in[AUDIO_IN_CH][i];

        // Apply each effect that is turned on
        for (int j = 0; j < MAX_EFFECTS; j++)
        {
            if (currentEffectsState[j])
            {
                wet = currentEffects[j]->Process(wet);
            }
        }

        // Output the processed signal with the volume level control
        out[AUDIO_OUT_CH][i] = wet * outputLevel;
    }
}

/**
 * Initializes the input controls
 */
void InitializeControls()
{
    // Initialize the 4 knobs
    AdcChannelConfig adcConfig[4];
    adcConfig[KNOB_1_CHN].InitSingle(hw->GetPin(effectPotPin1));
    adcConfig[KNOB_2_CHN].InitSingle(hw->GetPin(effectPotPin2));
    adcConfig[KNOB_3_CHN].InitSingle(hw->GetPin(effectPotPin3));
    adcConfig[KNOB_4_CHN].InitSingle(hw->GetPin(effectPotPin4));
    hw->adc.Init(adcConfig, 4);
    hw->adc.Start();

    // TODO: Find a better way to do this?
    // Give the ADC time to start up
    System::Delay(500);

    // Initialize the controlEncoder
    controlEncoder.Init(hw, hw->GetPin(effectSelectorPinA), hw->GetPin(effectSelectorPinB), hw->GetPin(effectSelectorPinSw));

    // Initialize the buttons
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        effectButtons[i].Init(hw, hw->GetPin(effectSPSTPins[i]));
    }

    // Initialize the LEDs
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        effectLeds[i].Init(hw->GetPin(effectLedPins[i]), false);
    }

    // Initialize the output volume knob
    outputVolume.Init(hw, KNOB_4_CHN, outputLevel, outputLevelMin, outputLevelMax);

    // Update the LEDs
    UpdateEffectLeds();
}

/**
 * Initializes the effects
 */
void InitializeEffects()
{
    // Initialize the effect objects to clean boost
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        currentEffects[i] = GetEffectObject(EffectType::CLEANBOOST);
    }

    // Setup each effect object
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        currentEffects[i]->Setup(hw);
    }
}

/**
 * Handles the control button
 */
void HandleControlButton()
{
    // Get the button press state
    bool buttonPressed = controlEncoder.IsPressed();

    // Check if we are currently in play mode and the control button is pressed
    if (currentState == PedalState::PLAY_MODE && buttonPressed)
    {
        // Switch to edit mode
        debugPrintln(hw, "Switching to edit mode!");
        currentState = PedalState::EDIT_MODE;

        // Turn on the control LED
        controlLed.Set(1.f);
        controlLed.Update();

        // Update the effect LEDs
        UpdateEffectLeds();
    }

    // Check if we are currently in edit mode and the control button is pressed
    else if (currentState == PedalState::EDIT_MODE && buttonPressed)
    {
        // Switch back to play mode
        debugPrintln(hw, "Switching to play mode!");
        currentState = PedalState::PLAY_MODE;

        // Turn off the control LED
        controlLed.Set(0);
        controlLed.Update();

        // Reset the selected edit effect
        selectedEditEffect = -1;

        // Update the effect LEDs
        UpdateEffectLeds();

        // Persist current effect settings in flash
        SaveCurrentEffectSettings();
    }
}

/**
 * Handles the effect buttons, turning on and off the effects
 */
void HandleEffectButtons()
{
    // If in edit mode, buttons choose which effect to edit
    if (currentState == PedalState::EDIT_MODE)
    {
        // Check each button
        for (int i = 0; i < MAX_EFFECTS; i++)
        {
            if (effectButtons[i].IsPressed())
            {
                // Set edit mode to this effect and update the LEDs
                selectedEditEffect = i;
                UpdateEffectLeds();

                debugPrintlnF(hw, "Editing %s", currentEffects[selectedEditEffect]->GetEffectName());
            }
        }
    }
    // If not in edit mode, buttons enable/disable effects
    else
    {
        // Check each button
        for (int i = 0; i < MAX_EFFECTS; i++)
        {
            if (effectButtons[i].IsPressed())
            {
                // Toggle the effect and update the LEDs
                currentEffectsState[i] = !currentEffectsState[i];
                UpdateEffectLeds();

                debugPrintlnF(hw, "Turned %s %s", currentEffects[i]->GetEffectName(), currentEffectsState[i] ? "ON" : "OFF");
            }
        }
    }
}

/**
 * Handles control of the effect controlEncoder, only enabled in edit mode
 */
void HandleControlEncoder()
{
    // if (currentState == PedalState::EDIT_MODE)
    // {
    //     // // Read the currently selected effect
    //     // EffectType selected = (EffectType)effectSelector.GetSelectedEffect();
    //     // IEffect *selectedEffect = GetEffectObject(selected);

    //     // // Check if the selected effect is different than the one we are editing
    //     // if (selectedEditEffect > -1 && selectedEditEffect < MAX_EFFECTS && currentEffects[selectedEditEffect] != selectedEffect)
    //     // {
    //     //     // New effect selected
    //     //     currentEffects[selectedEditEffect]->Cleanup();
    //     //     currentEffects[selectedEditEffect] = selectedEffect;
    //     //     currentEffects[selectedEditEffect]->Setup(hw);

    //     //     debugPrintlnF(hw, "Set effect %d to %s", selectedEditEffect, currentEffects[selectedEditEffect]->GetEffectName());
    //     // }

    //     controlEncoder.Debounce();
    //     if (controlEncoder.Increment() == -1)
    //     {
    //         debugPrintln(hw, "Turned left");
    //     }
    //     else if (controlEncoder.Increment() == 1)
    //     {
    //         debugPrintln(hw, "Turned right");
    //     }
    // }
}

/**
 * Updates the effect LEDs, turning them on and off based on the current state
 */
void UpdateEffectLeds()
{
    // If in edit mode, only the effect being edited is turned on
    if (currentState == PedalState::EDIT_MODE)
    {
        for (int i = 0; i < MAX_EFFECTS; i++)
        {
            // Turn on the selected edit effect, turn everything else off
            if (i == selectedEditEffect)
            {
                effectLeds[i].Set(1.f);
                effectLeds[i].Update();
            }
            else
            {
                effectLeds[i].Set(0);
                effectLeds[i].Update();
            }
        }
    }
    // If not in edit mode, turn on LEDs for effects that are enabled
    else
    {
        for (int i = 0; i < MAX_EFFECTS; i++)
        {
            // Turn on the LED if the effect is enabled
            effectLeds[i].Set(currentEffectsState[i] ? 1.f : 0);
            effectLeds[i].Update();
        }
    }
}

/**
 * Handles reading the volume knob and setting the output level
 */
void HandleOutputVolumeControl()
{
    if (outputVolume.SetNewValue(outputLevel))
    {
        debugPrintlnF(hw, "Updated the output level to: %f", outputLevel);
    }
}

/**
 * Saves the current effect settings to flash
 */
void SaveCurrentEffectSettings()
{
    // //currentEffects[0]->PrintEffectSettings();

    // // Initialize flash for writing
    // hw->qspi_handle.mode = DSY_QSPI_MODE_INDIRECT_POLLING;
    // dsy_qspi_init(&hw->qspi_handle);

    // IEffect temp[MAX_EFFECTS];
    // for (int i = 0; i < MAX_EFFECTS; i++)
    // {
    //     temp[i] = *currentEffects[i];
    // }

    // // Write the current effects array to flash
    // uint32_t writesize = MAX_EFFECTS * sizeof(effectsStorage[0]);
    // dsy_qspi_erase(memBase, memBase + writesize);
    // int success = dsy_qspi_write(memBase, writesize, (uint8_t *)temp);

    // if (success == DSY_MEMORY_ERROR)
    // {
    //     debugPrintln(hw, "Failed to write to memory!");
    // }

    // dsy_qspi_deinit();

    // //debugPrintlnF(hw, "Testing2 %f", effectsStorage[0]->GetEffectSettings().knobSettings[0]);
}

/**
 * Reads the effect settings from flash
 */
void ReadCurrentEffectSettings()
{
    // // Configure QSPI mode
    // hw->qspi_handle.mode = DSY_QSPI_MODE_DSY_MEMORY_MAPPED;
    // dsy_qspi_init(&hw->qspi_handle);

    // // for (int i = 0; i < MAX_EFFECTS; i++)
    // // {
    // //     currentEffects[i] = effectsStorage[i];
    // // }

    // //effectsStorage[0].PrintEffectSettings();

    // // Read the current effect objects and settings
    // for (int i = 0; i < MAX_EFFECTS; i++)
    // {
    //     // Read effect objects
    //     //debugPrintlnF(hw, "Effect %d: %s", i, effectsStorage[i].GetEffectName());

    //     // // Read settings
    //     // for (int j = 0; j < MAX_KNOBS; j++)
    //     // {
    //     //     debugPrintlnF(hw, "Knob %d: %f", j, effectsStorage[i]->GetEffectSettings().knobSettings[j]);
    //     // }
    //     // debugPrintlnF(hw, "Toggle: %d", effectsStorage[i]->GetEffectSettings().togglePosition);
    // }

    // dsy_qspi_deinit();
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

    // Initialize the input controls
    InitializeControls();

    // Initialize the effect objects
    ReadCurrentEffectSettings();
    InitializeEffects();

    // Start the audio processing
    debugPrintln(hw, "Starting Audio");
    hw->StartAudio(AudioCallback);

    // Turn on the onboard LED
    hw->SetLed(true);

    // Loop forever
    for (;;)
    {
        // Handle the control button
        HandleControlButton();

        // Handle the control encoder
        HandleControlEncoder();

        // Handle the effect buttons
        HandleEffectButtons();

        // Handle the output volume
        HandleOutputVolumeControl();

        // Execute the effect loop commands
        for (int i = 0; i < MAX_EFFECTS; i++)
        {
            currentEffects[i]->Loop(currentState == PedalState::EDIT_MODE && selectedEditEffect == i);
        }
    }
}