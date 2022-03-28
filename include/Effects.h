#ifndef EFFECTS_H
#define EFFECTS_H

#include "../lib/DaisyEffects/IEffect.h"
#include "../lib/DaisyEffects/CleanBoost.h"
#include "../lib/DaisyEffects/DaisyChorus.h"
#include "../lib/DaisyEffects/DaisyFlanger.h"
#include "../lib/DaisyEffects/DaisyTremolo.h"
#include "../lib/DaisyEffects/Crush.h"
#include "../lib/DaisyEffects/Distortion.h"
#include "../lib/DaisyEffects/Drive.h"
#include "../lib/DaisyEffects/AutoWah.h"
#include "../lib/DaisyEffects/Echo.h"
#include "../lib/DaisyEffects/DaisyCompressor.h"
#include "../lib/DaisyEffects/Reverb.h"
#include "../lib/DaisyEffects/DaisyPhaser.h"
#include "../lib/DaisyEffects/DaisyFold.h"
#include "../lib/DaisyEffects/Shifter.h"
#include "../lib/DaisyEffects/Metronome.h"
#include "../lib/DaisyEffects/DaisyResonator.h"
#include "../lib/DaisyEffects/StompBox.h"

#define AVAIL_EFFECTS 17
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
    RESONATOR,
    METRONOME,
    COMPRESSOR,
    STOMPBOX
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
    case RESONATOR:
        return new DaisyResonator();
    case COMPRESSOR:
        return new DaisyCompressor();
    case METRONOME:
        return new Metronome();
    case STOMPBOX:
        return new StompBox();
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
    else if (strncmp(effect->GetEffectName(), "METRONOME", 9) == 0)
    {
        return EffectType::METRONOME;
    }
    else if (strncmp(effect->GetEffectName(), "RESONATOR", 9) == 0)
    {
        return EffectType::RESONATOR;
    }
    else if (strncmp(effect->GetEffectName(), "STOMPBOX", 8) == 0)
    {
        return EffectType::STOMPBOX;
    }
    else
    {
        return EffectType::CLEANBOOST;
    }
}

#endif