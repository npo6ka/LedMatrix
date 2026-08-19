#pragma once

#include "events/observer.h"
#include "storage/IEffectStorage.h"
#include "EffectInfo.h"
#include "EffectFactory/EffectFactory.h"
#include "FpsManager.h"
#include "effects/effect.h"
#include "events/events.h"
#include "core/input/InputHub.h"

#include <stdint.h>

class EffectManager : public IObserver
{
public:
    EffectManager(IEffectStorage& storage, InputHub& inputHub);
    ~EffectManager() = default;

    // Управление эффектами
    void updateEffect(); // обновить эффект, эффектом из хранилища
    void setEffect(uint32_t index); // установить эффект по индексу

    // Цикл отложенного переключения эффектов
    void onTick();

    // Получение текущего FPS
    float getCurrentFPS() const;

    // Обработка событий (реализация IObserver)
    void handleEvent(const Event* event) override;

private:

    void onCheckRequestedEffectChange();
    void onTickEffect(const InputSnapshot& snapshot);

    IEffectStorage& _storage;
    InputHub& _inputHub;
    std::unique_ptr<Effect> _currentEffect;
    ChangeModeEventRequest _pendingRequest;     // Ожидающий запрос на смену эффекта
    FpsManager _fpsManager;                     // Менеджер FPS
};
