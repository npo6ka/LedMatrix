#pragma once
#include <stdint.h>

class Effect;

class EffectsList
{
private:
    uint8_t curId = 0;
    Effect *curEffect;
    unsigned long prev_micros;
    unsigned long fps;

    // singlton property
    // Конструкторы и оператор присваивания недоступны клиентам
    EffectsList() {};
    EffectsList(const EffectsList& ) = delete;
    EffectsList& operator=(EffectsList& ) = delete;
    Effect *getCurEffect() const;
    void deleteCurEffect();
    void setEffectWithoutInit(const uint8_t id);
    void initEffect();
    void setEffect();
    bool addEffect(uint8_t id, bool is_init);

public:
    static EffectsList& getInstance();
    void setErrorEffect();
    uint8_t getCurEffectId() const;
    void setEffect(uint8_t num);
    void nextEffect();
    void prevEffect();
    void reloadEff();
    void onTick();
    float getCurFPS();
    void getEffList();

    bool addEffect(uint8_t id);
    void removeEffect();
    void removeEffect(uint8_t num);
    void clear();


    void init();
};
