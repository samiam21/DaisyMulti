#ifndef EFFECT_TYPE_H
#define EFFECT_TYPE_H

#include "IEffect.h"
#include "../lib/Effects/CleanBoost.h"
#include "../lib/Effects/DaisyChorus.h"
#include "../lib/Effects/DaisyFlanger.h"
#include "../lib/Effects/DaisyTremolo.h"

// Effect Objects
CleanBoost boost;
DaisyChorus chorus;
DaisyFlanger flanger;
DaisyTremolo tremolo;

enum EffectType
{
    CLEANBOOST = 0,
    CHORUS = 1,
    FLANGER = 2,
    TREMOLO = 3,

    UNSET = 99
};

/**
 * Returns the effect object based on the passed in enum
 */
extern IEffect *GetEffectObject(EffectType type)
{
    switch (type)
    {
    case CHORUS:
        return (IEffect *)&chorus;
    case FLANGER:
        return (IEffect *)&flanger;
    case TREMOLO:
        return (IEffect *)&tremolo;
    case CLEANBOOST:
    case UNSET:
    default:
        return (IEffect *)&boost;
    }
};

#endif