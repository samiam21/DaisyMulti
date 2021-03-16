#ifndef IEFFECT_H
#define IEFFECT_H

#include "daisy_seed.h"
#include "PedalConfig.h"
#include "../src/DaisyDisplay.h"

class IEffect
{
public:
    virtual void Setup(daisy::DaisySeed *hardware, DaisyDisplay *daisyDisplay) {}
    virtual void Cleanup() {}
    virtual float Process(float in) { return 0; }
    virtual void Loop(bool allowEffectControl) {}
    virtual char *GetEffectName() { return (char *)"uh-oh"; }
    virtual char **GetKnobNames() { return (char **)""; }
    virtual EffectSettings GetEffectSettings() { return effectSettings; }
    virtual void SetEffectSettings(EffectSettings effectSettings) {}
    virtual void UpdateToggleDisplay() { }

protected:
    daisy::DaisySeed *hw;
    DaisyDisplay *display;
    EffectSettings effectSettings;
};

#endif