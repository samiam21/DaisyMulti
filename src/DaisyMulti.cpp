#include "DaisyMulti.h"

/**
 * Audio callback to process each enabled effect
 */
void AudioCallback(float **in, float **out, size_t size)
{
    // Interrupt handler for the control encoder
    ControlEncoderInterrupt();

    for (size_t i = 0; i < size; i++)
    {
        float wet = in[AUDIO_IN_CH][i];

        // Apply each effect that is turned on
        for (int j = 0; j < MAX_EFFECTS; j++)
        {
            if (currentEffectsState[j])
            {
                wet = availableEffects[currentEffects[j]]->Process(wet);
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
    controlEncoder.Init(hw->GetPin(effectSelectorPinA), hw->GetPin(effectSelectorPinB), hw->GetPin(effectSelectorPinSw), hw->AudioCallbackRate());

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

    // Update the LEDs
    UpdateEffectLeds();
}

/**
 * Initializes the effects
 */
void InitializeEffects()
{
    // Configure QSPI mode
    hw->qspi_handle.mode = DSY_QSPI_MODE_DSY_MEMORY_MAPPED;
    dsy_qspi_init(&hw->qspi_handle);

    // Read the current effect objects and settings
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        // Read and set the current effect
        currentEffects[i] = effectsStorage[i].availableEffectsPosition;
        newEffects[i] = effectsStorage[i].availableEffectsPosition;
        currentEffectNames[i] = availableEffects[effectsStorage[i].availableEffectsPosition]->GetEffectName();
        debugPrintlnF(hw, "Effect %d: %s", i, availableEffects[currentEffects[i]]->GetEffectName());

        // Read settings
        for (int j = 0; j < MAX_KNOBS; j++)
        {
            debugPrintlnF(hw, "Knob %d: %f", j, effectsStorage[i].effectSettings.knobSettings[j]);
        }
        debugPrintlnF(hw, "Toggle: %d", effectsStorage[i].effectSettings.togglePosition);

        // Initialize the effect
        availableEffects[currentEffects[i]]->Setup(hw, &display);
        availableEffects[currentEffects[i]]->SetEffectSettings(effectsStorage[i].effectSettings);
    }

    dsy_qspi_deinit();

    // // Show the selected effects in play mode
    // updatePlayModeEffects(display, currentEffectNames);

    // /** DEBUG - Used when flashing to a new board **/
    // for (int i = 0; i < MAX_EFFECTS; i++)
    // {
    //     // Read and set the current effect
    //     currentEffects[i] = 0;
    //     newEffects[i] = 0;
    //     currentEffectNames[i] = availableEffects[0]->GetEffectName();
    //     debugPrintlnF(hw, "Effect %d: %s", i, availableEffects[currentEffects[i]]->GetEffectName());

    //     // Initialize the effect
    //     availableEffects[currentEffects[i]]->Setup(hw, &display);
    //     availableEffects[currentEffects[i]]->SetEffectSettings(effectsStorage[i].effectSettings);
    // }
    // /** DEBUG - Used when flashing to a new board **/
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

                // Update the OLED to display the effect edit screen
                showEditModeEffectScreen(display,
                                         availableEffects[currentEffects[selectedEditEffect]]->GetEffectName(),
                                         availableEffects[currentEffects[selectedEditEffect]]->GetKnobNames());
                availableEffects[currentEffects[selectedEditEffect]]->UpdateToggleDisplay();                                       

                debugPrintlnF(hw, "Editing %s", availableEffects[currentEffects[selectedEditEffect]]->GetEffectName());
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

                debugPrintlnF(hw, "Turned %s %s", availableEffects[currentEffects[i]]->GetEffectName(), currentEffectsState[i] ? "ON" : "OFF");
            }
        }
    }
}

/**
 * Reads the control encoder and triggers actions
 */
void ControlEncoderInterrupt()
{
    // Get the button press state
    controlEncoder.Debounce();
    bool buttonPressed = controlEncoder.RisingEdge();

    // Check if we are currently in play mode and the control button is pressed
    if (currentState == PedalState::PLAY_MODE && buttonPressed)
    {
        // Trigger a switch to edit mode
        newState = PedalState::EDIT_MODE;
    }

    // Check if we are currently in edit mode and the control button is pressed
    else if (currentState == PedalState::EDIT_MODE && buttonPressed)
    {
        // Trigger a switch to play mode
        newState = PedalState::PLAY_MODE;
    }

    // Check for encoder turns in edit mode
    if (currentState == PedalState::EDIT_MODE && selectedEditEffect > -1 && selectedEditEffect < MAX_EFFECTS)
    {
        // Check for a change in the selected effect
        int inc = controlEncoder.Increment();
        if (inc != 0)
        {
            // Check if we have looped around
            int newEffect = currentEffects[selectedEditEffect] + inc;
            if (newEffect < 0)
            {
                newEffect = AVAIL_EFFECTS - 1;
            }
            else if (newEffect >= AVAIL_EFFECTS)
            {
                newEffect = 0;
            }

            // Trigger a change of the effect
            newEffects[selectedEditEffect] = newEffect;
        }
    }

    // Handle output volume whe in play mode
    else if (currentState == PedalState::PLAY_MODE)
    {
        // Check for a change in the selected effect
        int inc = controlEncoder.Increment();
        if (inc != 0)
        {
            // Increment the output volume
            newOutputLevel = outputLevel + ((float)inc * outputLevelIncrement);

            // Check if we have hit an edge
            if (newOutputLevel < outputLevelMin)
            {
                newOutputLevel = outputLevelMin;
            }
            else if (newOutputLevel > outputLevelMax)
            {
                newOutputLevel = outputLevelMax;
            }
        }
    }
}

/**
 * Handles control of the pedal state
 */
void HandlePedalState()
{
    // Has a new state been triggered?
    if (currentState != newState)
    {
        // Switching to edit mode
        if (newState == PedalState::EDIT_MODE)
        {
            // Switch to edit mode
            debugPrintln(hw, "Switching to edit mode!");
            currentState = PedalState::EDIT_MODE;

            // Update the effect LEDs
            UpdateEffectLeds();

            // Update the OLED display
            showEditModeStartupScreen(display);
        }

        // Switching to play mode
        else if (newState == PedalState::PLAY_MODE)
        {
            // Switch back to play mode
            debugPrintln(hw, "Switching to play mode!");
            currentState = PedalState::PLAY_MODE;

            // Reset the selected edit effect
            selectedEditEffect = -1;

            // Update the effect LEDs
            UpdateEffectLeds();

            // Update the OLED display
            updatePlayModeEffects(display, currentEffectNames);
            updateOutputLevel(display, outputLevel);

            // Persist current effect settings in flash
            SaveCurrentEffectSettings();
        }
    }
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
 * Saves the current effect settings to flash
 */
void SaveCurrentEffectSettings()
{
    // Initialize flash for writing
    hw->qspi_handle.mode = DSY_QSPI_MODE_INDIRECT_POLLING;
    dsy_qspi_init(&hw->qspi_handle);

    // Fill in the effect storage buffer
    for (int i = 0; i < MAX_EFFECTS; i++)
    {
        effectsStorageBuffer[i].availableEffectsPosition = currentEffects[i];
        effectsStorageBuffer[i].effectSettings = availableEffects[currentEffects[i]]->GetEffectSettings();
    }

    // Write the current effects array to flash
    uint32_t writesize = MAX_EFFECTS * sizeof(effectsStorage[0]);
    dsy_qspi_erase(memBase, memBase + writesize);
    int success = dsy_qspi_write(memBase, writesize, (uint8_t *)effectsStorageBuffer);

    if (success == DSY_MEMORY_ERROR)
    {
        debugPrintln(hw, "Failed to write to memory!");
    }

    dsy_qspi_deinit();
}

/**
 * Main Loop
 */
int main(void)
{
    // Configure and Initialize the Daisy Seed
    hw->Configure();
    hw->Init();

    // Initialize the OLED display
    initDisplay(hw, display);

    // Initialize debug printing (true = wait for COM connection before continuing)
    initDebugPrint(hw, WAIT_FOR_SERIAL);
    debugPrintln(hw, "Starting DaisyPedal...");

    // Update the block size and sample rate to minimize noise
    hw->SetAudioBlockSize(DAISY_BLOCKSIZE);
    hw->SetAudioSampleRate(DAISY_SAMPLE_RATE);

    // Initialize the input controls
    InitializeControls();

    // Initialize the effect objects
    InitializeEffects();

    // Start the audio processing
    debugPrintln(hw, "Starting Audio");
    hw->StartAudio(AudioCallback);

    // Turn on the onboard LED
    hw->SetLed(true);

    // Update the OLED display
    updatePlayModeEffects(display, currentEffectNames);
    updateOutputLevel(display, outputLevel);

    // Loop forever
    for (;;)
    {
        // Handle the pedal state
        HandlePedalState();

        // Handle the effect buttons
        HandleEffectButtons();

        // Check for a change in output level
        if (outputLevel != newOutputLevel)
        {
            outputLevel = newOutputLevel;
            debugPrintlnF(hw, "Changed output level to: %.2f", outputLevel);
            updateOutputLevel(display, outputLevel);
        }

        // Execute the effect loop commands
        for (int i = 0; i < MAX_EFFECTS; i++)
        {
            // Check for a changed effect
            if (currentEffects[i] != newEffects[i])
            {
                // Clean up the previous effect
                availableEffects[currentEffects[i]]->Cleanup();

                // Set the new effect
                currentEffects[i] = newEffects[i];
                currentEffectNames[i] = availableEffects[currentEffects[i]]->GetEffectName();

                // Update display for changed effect
                showEditModeEffectScreen(display,
                                         availableEffects[currentEffects[i]]->GetEffectName(),
                                         availableEffects[currentEffects[i]]->GetKnobNames());

                // Setup the new effect
                availableEffects[currentEffects[i]]->Setup(hw, &display);

                debugPrintlnF(hw, "Set effect %d to %s", selectedEditEffect, availableEffects[currentEffects[selectedEditEffect]]->GetEffectName());
            }

            // Call the effect's loop
            availableEffects[currentEffects[i]]->Loop(currentState == PedalState::EDIT_MODE && selectedEditEffect == i);
        }
    }
}