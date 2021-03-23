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
    if (strncmp(effect->GetEffectName(), "AUTOWAH", 7) == 0)
    {
        return EffectType::AUTOWAH;
    }
    else if (strncmp(effect->GetEffectName(), "BITCRUSH", 8) == 0)
    {
        return EffectType::BITCRUSH;
    }
    else if (strncmp(effect->GetEffectName(), "CHORUS", 6) == 0)
    {
        return EffectType::CHORUS;
    }
    else if (strncmp(effect->GetEffectName(), "COMPRESSOR", 10) == 0)
    {
        return EffectType::COMPRESSOR;
    }
    else if (strncmp(effect->GetEffectName(), "FLANGER", 7) == 0)
    {
        return EffectType::FLANGER;
    }
    else if (strncmp(effect->GetEffectName(), "FOLD", 4) == 0)
    {
        return EffectType::FOLD;
    }
    else if (strncmp(effect->GetEffectName(), "PHASER", 6) == 0)
    {
        return EffectType::PHASER;
    }
    else if (strncmp(effect->GetEffectName(), "TREMOLO", 7) == 0)
    {
        return EffectType::TREMOLO;
    }
    else if (strncmp(effect->GetEffectName(), "DISTORTION", 10) == 0)
    {
        return EffectType::DISTORTION;
    }
    else if (strncmp(effect->GetEffectName(), "OVERDRIVE", 9) == 0)
    {
        return EffectType::OVERDRIVE;
    }
    else if (strncmp(effect->GetEffectName(), "ECHO", 4) == 0)
    {
        return EffectType::ECHO;
    }
    else if (strncmp(effect->GetEffectName(), "REVERB", 6) == 0)
    {
        return EffectType::REVERB;
    }
    else if (strncmp(effect->GetEffectName(), "SHIFTER", 7) == 0)
    {
        return EffectType::SHIFTER;
    }
    else
    {
        return EffectType::CLEANBOOST;
    }
}

#endif