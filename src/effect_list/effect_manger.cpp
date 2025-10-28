/*#include "effect_manger.h"

#include "fps_manager.h"
#include "effect_factory.h"
#include "properties/memory_manager.h"
#include "properties/property_storage.h"

EffectsManager::EffectsManager() {
    Observable::subscribe(EventType::ChangeMode, this);
};

EffectsManager::~EffectsManager() {
    Observable::unsubscribe(EventType::ChangeMode, this);
};

void EffectsManager::init() {
    setEffect(MemoryManager::instance().get_cur_mod_num());
}

void EffectsManager::clearEffect() {
    if (_effect) {
        delete _effect;
        _effect = nullptr;
    }
}

void EffectsManager::setErrorEffect() {
    clearEffect();
    _effect = EffectFactory::createEffect(-1);
    _effect->on_init();
}

uint8_t EffectsManager::getEffectId() const {
    return MemoryManager::instance().load_mod_id();
}

uint8_t EffectsManager::getEffectNum() const {
    return MemoryManager::instance().get_cur_mod_num();
}

std::string EffectsManager::getEffectName() const {
    uint8_t id = getEffectId();
    if (id > 0 && id < EffectFactory::getEffectCount()) {
        return "";
    }

    return EffectFactory::getEffectName(id);
}


void EffectsManager::syncEffect() {
    clearEffect();
    _effect = EffectFactory::createEffect(MemoryManager::instance().load_mod_id());

    if (!_effect) {
        out("ERROR: Effect not crated\n");
        setErrorEffect();
        return;
    }

    PropertyStorage::instance().load_all_propertyes();

    _effect->on_clear();
    _effect->on_init();

    auto ev = ModChangedEvent({
        EventType::ModChanged,
        MemoryManager::instance().load_mod_id(),
        MemoryManager::instance().get_cur_mod_num()
    });
    Observable::notify(&ev);
}

void EffectsManager::setEffect(const uint8_t num) {
    if (!MemoryManager::instance().set_mod(num)) {
        out("ERROR: Effect number out of range\n");
        setErrorEffect();
        return;
    }

    syncEffect();
}

void EffectsManager::nextEffect() {
    MemoryManager::instance().next_mod();
    syncEffect();
}

void EffectsManager::prevEffect() {
    MemoryManager::instance().prev_mod();
    syncEffect();
}


void EffectsManager::onTick() {
    if (_reqEffect.type != Request::Type::None && (_effect->is_end() || _reqEffect.hardReset)) {
        if (_reqEffect.type == Request::Type::Next) {
            nextEffect();
        } else if (_reqEffect.type == Request::Type::Previous) {
            prevEffect();
        } else if (_reqEffect.type == Request::Type::Set) {
            setEffect(_reqEffect.num);
        }
        _reqEffect.type = Request::Type::None;
    }

    if (!_effect)
        return;

    if (_fpsManager.needUpdate()) {
        _effect->on_update();
        _effect->on_render();
        FastLED.show();
    }
}

float EffectsManager::getCurFPS() {
    return _fpsManager.getRealFPS();
}

void EffectsManager::handleEvent(Event *event) {
    if (event->type == EventType::ChangeMode) {
        ChangeModEvent *ev = static_cast<ChangeModEvent *>(event);
        if (ev->type == ChangeModEvent::Type::Next) {
            _reqEffect = { Request::Type::Next, 0, ev->hardReset };
        } else if (ev->type == ChangeModEvent::Type::Previous) {
            _reqEffect = { Request::Type::Previous, 0, ev->hardReset };
        } else if (ev->type == ChangeModEvent::Type::Set) {
            _reqEffect = { Request::Type::Set, ev->id, ev->hardReset };
        }
    }
}*/