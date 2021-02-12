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
        if (isEffect1On)
        {
            wet = effect1->Process(wet);
        }
        if (isEffect2On)
        {
            wet = effect2->Process(wet);
        }
        if (isEffect3On)
        {
            wet = effect3->Process(wet);
        }
        if (isEffect4On)
        {
            wet = effect4->Process(wet);
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

    // Initialize the control button and LED
    controlButton.Init(hw, hw->GetPin(effectSPSTPin4), 3000, 300);
    controlLed.Init(hw->GetPin(effectLedPin4), false);

    // Initialize the buttons
    effect1Button.Init(hw, hw->GetPin(effectSPSTPin1));
    effect2Button.Init(hw, hw->GetPin(effectSPSTPin2));
    effect3Button.Init(hw, hw->GetPin(effectSPSTPin3));
    //effect4Button.Init(hw, hw->GetPin(effectSPSTPin4));

    // Initialize the LEDs
    effect1Led.Init(hw->GetPin(effectLedPin1), false);
    effect2Led.Init(hw->GetPin(effectLedPin2), false);
    effect3Led.Init(hw->GetPin(effectLedPin3), false);
    //effect4Led.Init(hw->GetPin(effectLedPin4), false);

    // Initialize the output volume knob
    outputVolume.Init(hw, KNOB_4_CHN, outputLevel, outputLevelMin, outputLevelMax);

    // Set the LEDs based on whether the effect is on/off
    effect1Led.Set(isEffect1On ? 1.f : 0);
    effect1Led.Update();
    effect2Led.Set(isEffect2On ? 1.f : 0);
    effect2Led.Update();
    effect3Led.Set(isEffect3On ? 1.f : 0);
    effect3Led.Update();
    effect4Led.Set(isEffect4On ? 1.f : 0);
    effect4Led.Update();
}

/**
 * Initializes the effects
 */
void InitializeEffects()
{
    // Get the effect objects
    effect1 = GetEffectObject(EffectType::DISTORTION);
    effect2 = GetEffectObject(EffectType::OVERDRIVE);
    effect3 = GetEffectObject(EffectType::FLANGER);
    effect4 = GetEffectObject(EffectType::CLEANBOOST);

    // Setup each effect object
    effect1->Setup(hw);
    effect2->Setup(hw);
    effect3->Setup(hw);
    effect4->Setup(hw);
}

/**
 * Handles the control button
 */
void HandleControlButton()
{
    // Get the button states (held must be checked before pressed)
    bool buttonHeld = controlButton.IsHeld();
    bool buttonPressed = controlButton.IsPressed(false);

    // Check if we are currently in play mode and the control button is held
    if (currentState == PedalState::PLAY_MODE && buttonHeld)
    {
        // Switch to edit mode
        debugPrintln(hw, "Switching to transition mode!");
        currentState = PedalState::TRANSITION_MODE;

        // Turn on the control LED
        controlLed.Set(1.f);
        controlLed.Update();
    }

    // Check for the button being released to transition into edit mode
    if (currentState == PedalState::TRANSITION_MODE && !buttonPressed)
    {
        // Switch to edit mode
        debugPrintln(hw, "Switching to edit mode!");
        currentState = PedalState::EDIT_MODE;
    }

    // Check if we are currently in edit mode and the control button is pressed
    if (currentState == PedalState::EDIT_MODE && buttonPressed)
    {
        // Switch back to play mode
        debugPrintln(hw, "Switching to play mode!");
        currentState = PedalState::PLAY_MODE;

        // Turn off the control LED
        controlLed.Set(0);
        controlLed.Update();
    }
}

/**
 * Handles the effect buttons, turning on and off the effects
 */
void HandleEffectButtons()
{
    // Poll effect button 1 to toggle effect 1
    if (effect1Button.IsPressed())
    {
        isEffect1On = !isEffect1On;
        effect1Led.Set(isEffect1On ? 1.f : 0);
        effect1Led.Update();

        debugPrintlnF(hw, "Turned %s %s", effect1->GetEffectName(), isEffect1On ? "ON" : "OFF");
    }

    // Poll effect button 2 to toggle effect 2
    if (effect2Button.IsPressed())
    {
        isEffect2On = !isEffect2On;
        effect2Led.Set(isEffect2On ? 1.f : 0);
        effect2Led.Update();

        debugPrintlnF(hw, "Turned %s %s", effect2->GetEffectName(), isEffect2On ? "ON" : "OFF");
    }

    // Poll effect button 3 to toggle effect 3
    if (effect3Button.IsPressed())
    {
        isEffect3On = !isEffect3On;
        effect3Led.Set(isEffect3On ? 1.f : 0);
        effect3Led.Update();

        debugPrintlnF(hw, "Turned %s %s", effect3->GetEffectName(), isEffect3On ? "ON" : "OFF");
    }

    // // Poll effect button 4 to toggle effect 4
    // if (effect4Button.IsPressed())
    // {
    //     isEffect4On = !isEffect4On;
    //     effect4Led.Set(isEffect4On ? 1.f : 0);
    //     effect4Led.Update();

    //     debugPrintlnF(hw, "Turned %s %s", effect4->GetEffectName(), isEffect4On ? "ON" : "OFF");
    // }
}

/**
 * Handles reading the volume knob and setting the output level
 */
void HandleOutputVolumeControl()
{
    // Knob 1 controls the boost level
    if (outputVolume.SetNewValue(outputLevel))
    {
        debugPrintlnF(hw, "Updated the output level to: %f", outputLevel);
    }
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

        // Handle the effect buttons
        HandleEffectButtons();

        // Handle the output volume
        HandleOutputVolumeControl();

        // Execute the effect loop commands
        if (isEffect1On)
        {
            effect1->Loop(currentState);
        }
        if (isEffect2On)
        {
            effect2->Loop(currentState);
        }
        if (isEffect3On)
        {
            effect3->Loop(currentState);
        }
        if (isEffect4On)
        {
            effect4->Loop(currentState);
        }
    }
}