#ifndef SELECTOR_H
#define SELECTOR_H

#include "daisy_seed.h"
#include "../../include/PedalConfig.h"

using namespace daisy;

/**
 * Selector class to handle selecting an effect
 * This class will initialize the provided pins in the init function
 */
class Selector
{
public:
    /**
     * This function will initialize the selector pins
     */
    void Init(DaisySeed *hardware, dsy_gpio_pin pin1, dsy_gpio_pin pin2, dsy_gpio_pin pin3, dsy_gpio_pin pin4);

    /**
     * Reads the selector pins and reports back the associated effect
     * @return int of the selected effect
     */
    int GetSelectedEffect();

private:
    DaisySeed *hw;

    Switch selectorPin1;
    Switch selectorPin2;
    Switch selectorPin3;
    Switch selectorPin4;
};

#endif