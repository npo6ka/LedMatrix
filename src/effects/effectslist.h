#pragma once

#include "erroreffect.h"

#define MAX_EFFECTS 25

class EffectsList
{
private:
    // singlton property
    // Конструкторы и оператор присваивания недоступны клиентам
    EffectsList();
    EffectsList( const EffectsList& );
    EffectsList& operator=( EffectsList& );

    int amnt;
    int curNum = 0;
    Effect *curEffect;
    unsigned long prev_micros;
    unsigned long fps;
    uint8_t mods_size[MAX_EFFECTS];

    void init();
    void setEffect(Effect *eff);

public:
    static EffectsList& getInstance();

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
