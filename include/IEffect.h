#ifndef IEFFECT_H
#define IEFFECT_H

#include "daisy_seed.h"
#include "PedalConfig.h"
#include "Storage.h"
#include "../src/DaisyDisplay.h"

class IEffect
{
public:
    IEffect() {}
    virtual ~IEffect() {}
    virtual void Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay, unsigned long *avgTempo = nullptr) {}
    virtual void Cleanup() {}
    virtual float Process(float in) { return 0; }
    virtual void Loop(bool allowEffectControl) {}
    virtual char *GetEffectName() { return (char *)"uh-oh"; }
    virtual char **GetKnobNames() { return (char **)""; }
    virtual EffectSettings GetEffectSettings() { return effectSettings; }
    virtual void SetEffectSettings(EffectSettings effectSettings) {}
    virtual void UpdateToggleDisplay() {}

protected:
    daisy::DaisySeed *hw;
    DaisyDisplay *display;
    EffectSettings effectSettings;
};

#endif