#ifndef EFFECTS_H
#define EFFECTS_H

#include "IEffect.h"
#include "../lib/Effects/CleanBoost.h"
#include "../lib/Effects/DaisyChorus.h"
#include "../lib/Effects/DaisyFlanger.h"
#include "../lib/Effects/DaisyTremolo.h"
#include "../lib/Effects/Crush.h"
#include "../lib/Effects/Distortion.h"
#include "../lib/Effects/Drive.h"
#include "../lib/Effects/AutoWah.h"
#include "../lib/Effects/Echo.h"
#include "../lib/Effects/DaisyCompressor.h"
#include "../lib/Effects/Reverb.h"
#include "../lib/Effects/DaisyPhaser.h"
#include "../lib/Effects/DaisyFold.h"
#include "../lib/Effects/Shifter.h"

#define AVAIL_EFFECTS 14
enum EffectType
{
    CLEANBOOST,
    CHORUS,
    FLANGER,
    TREMOLO,
    PHASER,
    SHIFTER,
    AUTOWAH,
    FOLD,
    BITCRUSH,
    OVERDRIVE,
    DISTORTION,
    ECHO,
    REVERB,
    COMPRESSOR
};

/**
 * Returns the effect object based on the passed in enum
 */
extern IEffect *GetEffectObject(EffectType type)
{
    switch (type)
    {
    case CHORUS:
        return new DaisyChorus();
    case FLANGER:
        return new DaisyFlanger();
    case TREMOLO:
        return new DaisyTremolo();
    case PHASER:
        return new DaisyPhaser();
    case SHIFTER:
        return new Shifter();
    case AUTOWAH:
        return new AutoWah();
    case FOLD:
        return new DaisyFold();
    case BITCRUSH:
        return new Crush();
    case OVERDRIVE:
        return new Drive();
    case DISTORTION:
        return new Distortion();
    case ECHO:
        return new Echo();
    case REVERB:
        return new Reverb();
    case COMPRESSOR:
        return new DaisyCompressor();
    case CLEANBOOST:
    default:
        return new CleanBoost();
    }
};

/**
 * Returns the effect type based on the effect object
 */
extern EffectType GetEffectType(IEffect *effect)
{
    if (effect->GetEffectName() == "AUTOWAH")
    {
        return EffectType::AUTOWAH;
    }
    else if (effect->GetEffectName() == "BITCRUSH")
    {
        return EffectType::BITCRUSH;
    }
    else if (effect->GetEffectName() == "CHORUS")
    {
        return EffectType::CHORUS;
    }
    else if (effect->GetEffectName() == "COMPRESSOR")
    {
        return EffectType::COMPRESSOR;
    }
    else if (effect->GetEffectName() == "FLANGER")
    {
        return EffectType::FLANGER;
    }
    else if (effect->GetEffectName() == "FOLD")
    {
        return EffectType::FOLD;
    }
    else if (effect->GetEffectName() == "PHASER")
    {
        return EffectType::PHASER;
    }
    else if (effect->GetEffectName() == "TREMOLO")
    {
        return EffectType::TREMOLO;
    }
    else if (effect->GetEffectName() == "DISTORTION")
    {
        return EffectType::DISTORTION;
    }
    else if (effect->GetEffectName() == "OVERDRIVE")
    {
        return EffectType::OVERDRIVE;
    }
    else if (effect->GetEffectName() == "ECHO")
    {
        return EffectType::ECHO;
    }
    else if (effect->GetEffectName() == "REVERB")
    {
        return EffectType::REVERB;
    }
    else if (effect->GetEffectName() == "SHIFTER")
    {
        return EffectType::SHIFTER;
    }
    else
    {
        return EffectType::CLEANBOOST;
    }
}

#endif