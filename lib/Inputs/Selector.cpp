#include "Selector.h"

void Selector::Init(DaisySeed *hardware, dsy_gpio_pin pin1, dsy_gpio_pin pin2, dsy_gpio_pin pin3, dsy_gpio_pin pin4)
{
    hw = hardware;

    // Initialize the selector pins
    selectorPin1.Init(hw->GetPin(effectSelectorPin1), 1, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, Switch::Pull::PULL_UP);
    selectorPin2.Init(hw->GetPin(effectSelectorPin2), 1, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, Switch::Pull::PULL_UP);
    selectorPin3.Init(hw->GetPin(effectSelectorPin3), 1, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, Switch::Pull::PULL_UP);
    selectorPin4.Init(hw->GetPin(effectSelectorPin4), 1, Switch::Type::TYPE_MOMENTARY, Switch::Polarity::POLARITY_NORMAL, Switch::Pull::PULL_UP);
}

int Selector::GetSelectedEffect()
{
    // Read the state of the encoder pins
    uint32_t pin1 = (uint32_t)selectorPin1.RawState();
    uint32_t pin2 = (uint32_t)selectorPin2.RawState();
    uint32_t pin3 = (uint32_t)selectorPin3.RawState();
    uint32_t pin4 = (uint32_t)selectorPin4.RawState();

    // Get the combined value and set the effect type
    uint32_t combined = pin4 | (pin3 << 1) | (pin2 << 2) | (pin1 << 3);

    return combined;
}