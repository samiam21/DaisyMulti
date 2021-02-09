#ifndef BUTTONTEST_H
#define BUTTONTEST_H

#include "daisy_seed.h"
#include "../../include/IEffect.h"
#include "../../include/PedalConfig.h"
#include "../Inputs/Button.h"

using namespace daisy;

/**********************************************
 * ButtonTest Effect
 * 
 * SPST 1 - 
 * SPST 2 - 
 * SPST 3 - 
 * SPST 4 - 
 * 
 * SPDT 1 - 
 * SPDT 2 - 
 * 
 * Knob 1 - 
 * Knob 2 - 
 * Knob 3 - 
 * Knob 4 - 
 * 
 * LED 1 - 
 * LED 2 - 
 * LED 3 - 
 * LED 4 - 
 **********************************************/

class ButtonTest : public IEffect
{
public:
    void Setup(daisy::DaisySeed *hardware);
    void Cleanup();
    float Process(float in);
    void Loop(PEDAL_STATE state);
    char *GetEffectName();

private:
    bool isMuted = false;

    Button button1;
    Button button2;
    Button button3;
    Button button4;

    Led led1;
    Led led2;
    Led led3;
    Led led4;
};

#endif