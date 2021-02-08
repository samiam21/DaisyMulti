#ifndef IEFFECT_H
#define IEFFECT_H

#include "daisy_seed.h"
#include "PedalConfig.h"

class IEffect
{
public:
    virtual void Setup(daisy::DaisySeed* hardware) {}
    virtual void Cleanup() {}
    virtual void Process(float **in, float **out, size_t size) {}
    virtual void Loop(PEDAL_STATE state) {}
    virtual char *GetEffectName() { return (char*)"uh-oh"; }

protected:
    daisy::DaisySeed* hw;
};

#endif