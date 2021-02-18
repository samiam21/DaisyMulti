#ifndef IEFFECT_H
#define IEFFECT_H

#include "daisy_seed.h"
#include "PedalConfig.h"

class IEffect
{
public:
    virtual void Setup(daisy::DaisySeed *hardware) {}
    virtual void Cleanup() {}
    virtual float Process(float in) { return 0; }
    virtual void Loop(bool allowEffectControl) {}
    virtual char *GetEffectName() { return (char *)"uh-oh"; }
    virtual EffectSettings GetEffectSettings() { }

protected:
    daisy::DaisySeed *hw;
};

#endif