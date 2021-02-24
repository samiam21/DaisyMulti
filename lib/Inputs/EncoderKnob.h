#ifndef ENCODER_KNOB_H
#define ENCODER_KNOB_H

#include "daisy_seed.h"
#include "../../include/PedalConfig.h"

using namespace daisy;

/**
 * EncoderKnob class to handle reading an encoder while debouncing it
 * This class will initialize the provided pins in the init function
 */
class EncoderKnob
{
public:
    /**
     * Initialize the knob
     */
    void Init(DaisySeed *hardware, dsy_gpio_pin pinA, dsy_gpio_pin pinB, dsy_gpio_pin pinSw, unsigned long debounceTime = 200);

    /**
     * Checks if the knob is pressed, accounting for debounce
     * @return Returns true if the knob is pressed, false if not
     */
    bool IsPressed(bool debounce = true);

private:
    dsy_gpio_pin _pinA;
    dsy_gpio_pin _pinB;
    dsy_gpio_pin _pinSw;
    DaisySeed *hw;

    unsigned long buttonDebounce = 200;
    unsigned long lastButtonPress = 0;

    Encoder encoder;
};

#endif