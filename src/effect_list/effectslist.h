#pragma once
#include <stdint.h>
#include "events/observer.h"

class Effect;

class EffectsList : public IObserver
{
private:
    uint8_t curNum = 0;
    Effect *curEffect;
    unsigned long prev_micros;
    unsigned long fps;

    // singlton property
    // Конструкторы и оператор присваивания недоступны клиентам
    EffectsList();
    ~EffectsList();
    EffectsList(const EffectsList& );
    EffectsList& operator=(EffectsList& );
    Effect *getCurEffect() const;
    void clearCurEffect();

public:
    static EffectsList& getInstance();
    void setErrorEffect();
    uint8_t getCurEffectNum() const;
    void setEffect(const uint8_t &num);
    void nextEffect();
    void prevEffect();
    void reloadCurEff();
    void onTick();
    float getCurFPS();
    bool effectIsEnd();
    virtual void handleEvent(Event *event) override;
};
