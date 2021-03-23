#ifndef STORAGE_H
#define STORAGE_H

#include "PedalConfig.h"

struct EffectSettings
{
    float knobSettings[MAX_KNOBS];
    u_int8_t togglePosition;
};

struct EffectStorage
{
    int effectType;
    EffectSettings effectSettings;
};

#endif