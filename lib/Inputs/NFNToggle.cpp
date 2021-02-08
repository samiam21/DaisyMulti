#include "NFNToggle.h"

void NFNToggle::Init(dsy_gpio_pin pin1, dsy_gpio_pin pin2)
{
    // Set and initialize the pins
    togglePin1 = pin1;
    togglePin2 = pin2;

    pinSwitch1.Init(togglePin1, 1000, Switch::Type::TYPE_TOGGLE, Switch::Polarity::POLARITY_NORMAL, Switch::Pull::PULL_UP);
    pinSwitch2.Init(togglePin2, 1000, Switch::Type::TYPE_TOGGLE, Switch::Polarity::POLARITY_NORMAL, Switch::Pull::PULL_UP);
}

uint8_t NFNToggle::ReadToggle()
{
    uint8_t ret = -1;

    // Read the pins
    bool pin1reading = pinSwitch1.RawState();
    bool pin2reading = pinSwitch2.RawState();

    // Check the position and return
    if (pin1reading)
    {
        ret = 0;
    }
    else if (pin2reading)
    {
        ret = 2;
    }
    else
    {
        ret = 1;
    }

    return ret;
}