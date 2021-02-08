#ifndef EFFECT_TYPE_H
#define EFFECT_TYPE_H

#include "IEffect.h"
#include "../lib/Effects/SimpleBypass.h"
#include "../lib/Effects/ButtonTest.h"
#include "../lib/Effects/KnobTest.h"
#include "../lib/Effects/ToggleTest.h"

// Effect Objects
SimpleBypass simpleBypass;
ButtonTest buttonTest;
KnobTest knobTest;
ToggleTest toggleTest;

/**
 * The rotary encoder is using Gray code, not standard hex.
 * The sequence of decimal numbers that it produces is as follows:
 * 0, 1, 3, 2, 6, 7, 5, 4, 12, 13, 15, 14, 10, 11, 9, 8
 */
enum EffectType
{
    SIMPLEBYPASS = 0,
    BUTTONTEST = 1,
    KNOBTEST = 3,
    TOGGLETEST = 2,

    UNSET = 99
};

/**
 * Returns the effect object based on the passed in enum
 */
extern IEffect *GetEffectObject(EffectType type)
{
    switch (type)
    {
    case BUTTONTEST:
        return (IEffect *)&buttonTest;
    case KNOBTEST:
        return (IEffect *)&knobTest;
    case TOGGLETEST:
        return (IEffect *)&toggleTest;
    case SIMPLEBYPASS:
    case UNSET:
    default:
        return (IEffect *)&simpleBypass;
    }
};

#endif