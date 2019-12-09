#pragma once

#include "erroreffect.h"
#include "11_dynamic_square.h"

class EffectsList
{
private:
    int curNum = 0;
    Effect *curEffect;
    unsigned long prev_micros;
    unsigned long fps;

    void init();
    void setEffect(Effect *eff);

public:
    
    int amnt;

    EffectsList();

    void setErrorEffect();
    Effect *getCurEffect();
    Effect *getNewEffectInstance(int num);
    void clearCurEffect();
    void setEffect(int num);
    int getCurEffectNum();
    void nextEffect();
    void prevEffect();
    void reloadCurEff();
    void onTick();
    float getCurFPS();
};
