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
    static constexpr uint8_t kFpsMin = 1;
    // Аппаратный потолок: быстрее лента не успевает принять кадр
    static uint8_t fpsMax();

    EffectManager(IEffectStorage& storage, InputHub& inputHub);
    ~EffectManager() = default;

    // Управление эффектами
    void updateEffect(); // обновить эффект, эффектом из хранилища
    void setEffect(uint32_t index); // установить эффект по индексу

    // Цикл отложенного переключения эффектов
    void onTick();

    // Получение текущего FPS
    float getCurrentFPS() const;

    // FPS текущего режима. Значение задаёт сам режим в on_init(), извне его можно
    // перебить до следующей смены режима.
    uint8_t getEffectFps() const;
    void setEffectFps(uint8_t fps);

    // Обработка событий (реализация IObserver)
    void handleEvent(const Event* event) override;

private:

    void onCheckRequestedEffectChange();
    void onTickEffect(const InputSnapshot& snapshot);
    void applyTargetFps();

    IEffectStorage& _storage;
    InputHub& _inputHub;
    std::unique_ptr<Effect> _currentEffect;
    ChangeModeEventRequest _pendingRequest;     // Ожидающий запрос на смену эффекта
    FpsManager _fpsManager;                     // Менеджер FPS
};
