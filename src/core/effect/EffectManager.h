#pragma once

#include "events/observer.h"
#include "storage/IEffectStorage.h"
#include "EffectInfo.h"
#include "EffectFactory.h"
#include "FpsManager.h"
#include "effects/effect.h"
#include "events/events.h"

#include <stdint.h>

class EffectManager : public IObserver
{
public:
    EffectManager(IEffectStorage& storage);
    ~EffectManager() = default;

    // Управление эффектами
    void updateEffect(); // обновить эффект, эффектом из хранилища
    void setEffect(uint32_t index); // установить эффект по индексу

    // Цикл отложенного переключения эффектов
    void onTick();

    // Получение текущего FPS
    float getCurrentFPS() const;

    // Обработка событий (реализация IObserver)
    void handleEvent(Event* event) override;

private:

    void onCheckRequestedEffectChange();
    void onTickEffect();

    IEffectStorage& _storage;                   // Ссылка на хранилище эффектов
    std::unique_ptr<Effect> _currentEffect;     // Указатель на текущий активный эффект
    ChangeModeEventRequest _pendingRequest;     // Ожидающий запрос на смену эффекта
    FpsManager _fpsManager;                     // Менеджер FPS
};
