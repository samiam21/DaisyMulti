#ifndef NFNTOGGLE_H
#define NFNTOGGLE_H

#include "daisy_seed.h"
#include "../../include/PedalConfig.h"

using namespace daisy;

/**
 * On/Off/On toggle class to handle reading the value of a toggle switch
 * This class will initialize the provided pins in the init function
 */
class NFNToggle
{
public:
    /**
     * These functions will initialize the toggle pins
     */
    void Init(dsy_gpio_pin pin1, dsy_gpio_pin pin2);

    /**
     * Reads the toggle pins and reports back the position
     * @return Position value, either 0, 1, or 2
     */
    uint8_t ReadToggle();

private:
    // Class variables
    dsy_gpio_pin togglePin1;
    dsy_gpio_pin togglePin2;

    Switch pinSwitch1;
    Switch pinSwitch2;
};

#endif