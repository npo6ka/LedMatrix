#pragma once
#include <stdint.h>

class Effect;

class EffectsList
{
private:
    uint8_t curNum = 0;
    Effect *curEffect;
    unsigned long prev_micros;
    unsigned long fps;

    // singlton property
    // Конструкторы и оператор присваивания недоступны клиентам
    EffectsList() {};
    EffectsList(const EffectsList& );
    EffectsList& operator=(EffectsList& );
    Effect *getCurEffect() const;

public:
    static EffectsList& getInstance();
    void setErrorEffect();
    uint8_t getCurEffectNum() const;
    void clearCurEffect();
    void setEffect(const uint8_t &num);
    void nextEffect();
    void prevEffect();
    void reloadCurEff();
    void onTick();
    float getCurFPS();
};
