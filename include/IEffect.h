#ifndef IEFFECT_H
#define IEFFECT_H

#include "daisy_seed.h"

class IEffect
{
public:
    virtual void Setup(daisy::DaisySeed* hardware) {}
    virtual void Cleanup() {}
    virtual void AudioCallback(float **in, float **out, size_t size) {}
    virtual void Loop() {}
    virtual char *GetEffectName() { return (char*)"uh-oh"; }

protected:
    daisy::DaisySeed* hw;
};

#endif