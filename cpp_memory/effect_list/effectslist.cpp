#include "effectslist.h"
#include "erroreffect.h"
#include "stdio.h"
#include "initializer_list"

#include "effects/slow_random.h"
#include "effects/slow_random_2.h"
/*
#include "testmode.h"*/

using EffectFactory = Effect* (*)();


template <class T>
Effect *makeEffect() {
    return new T();
}

template <class T>
constexpr EffectFactory effectFactory() {
    return makeEffect<T>;
}

auto effectsFactories = {
    effectFactory<SlowRandom>(),
    effectFactory<SlowRandom2>(),
};

static Effect *newEffect(const uint8_t& id) {
    if (id >= effectsFactories.size()) {
        return nullptr;
    } else {
        return effectsFactories.begin()[id]();
    }
}

void EffectsList::deleteCurEffect() {
    if (getCurEffect()) {
        delete curEffect;
        curEffect = nullptr;
        curId = -1;
    }
}

Effect *EffectsList::getCurEffect() const {
    return curEffect;
}

uint8_t EffectsList::getCurEffectId() const {
    return curId;
}

EffectsList& EffectsList::getInstance() {
    static EffectsList instance;
    return instance;
}

void EffectsList::setErrorEffect() {
    if (curEffect) {
        delete curEffect;
    }
    curEffect = new ErrorEffect();
    curId = -1;
}

void EffectsList::setEffectWithoutInit(const uint8_t id) {
    if (id >= effectsFactories.size()) {
        printf("EffectList: Effect id out of range\n");
        setErrorEffect();
        return;
    }

    deleteCurEffect();
    curEffect = newEffect(id);

    if (!curEffect) {
        printf("EffectList: Effect not created\n");
        setErrorEffect();
        return;
    }

    curId = id;
}

void EffectsList::initEffect() {
    uint16_t eff_size = 0;
    MemoryManager::instance().load_mod_size(eff_size);
    if (eff_size == PropertyStorage::instance().size()) {
        PropertyStorage::instance().load_all_propertyes();
        curEffect->on_clear();
        curEffect->on_init();
    } else { // всё плохо, почему то изменился размер режима
        out("EffectList: Effect size differs from stored in memory\n");
        removeEffect();
    }
}

// инициализирует эффект получая текущий эффект из памяти
void EffectsList::setEffect() {
    MemoryManager::instance().load_mod_id(curId);
    setEffectWithoutInit(curId);
    initEffect();
}

// инициализирует эффект по указанному номеру 0 .. size
void EffectsList::setEffect(uint8_t num) {
    if (MemoryManager::instance().set_mod(num + 1)) {
        setEffect();
    } else {
        out("EffectList: Try to set non-existent mode: %d\n", num);
    }
}

void EffectsList::nextEffect() {
    MemoryManager::instance().next_mod();
    setEffect();
}

void EffectsList::prevEffect() {
    MemoryManager::instance().prev_mod();
    setEffect();
}

void EffectsList::init() {
    int eff_amount = MemoryManager::instance().get_mod_amount();
    if (eff_amount == 0) { // нет режимов, значит создать все эффекты с 0 до конца списка
        for (int i = 0; i < effectsFactories.size(); ++i) {
            addEffect(i, false);
        }
        MemoryManager::instance().set_mod(1);
    }
    setEffect();
}

//перезапустить текущий эффект
void EffectsList::reloadEff() {
    setEffect();
}

void EffectsList::onTick() {
    curEffect->on_update();
    curEffect->on_render();
}

float EffectsList::getCurFPS() {
    return (float)fps / 10;
}

bool EffectsList::addEffect(uint8_t id, bool is_init) {
    setEffectWithoutInit(id);
    if (curId == -1) {
        return false;
    }
    if (!MemoryManager::instance().add_mod(id, PropertyStorage::instance().size())) {
        out("EffectList: Error adding effect on memory");
        return false;
    }
    PropertyStorage::instance().save_all_propertyes();
    if (is_init) {
        initEffect();
    }
    return true;
}

bool EffectsList::addEffect(uint8_t id) {
    return addEffect(id, true);
}

void EffectsList::removeEffect() {
    MemoryManager::instance().remove_mod();
    setEffect();
}

void EffectsList::removeEffect(uint8_t num) {
    if (MemoryManager::instance().get_cur_mod_num() == num) {
        removeEffect();
    } else {
        MemoryManager::instance().remove_mod(num);
    }
}

void EffectsList::clear() {
    MemoryManager::instance().remove_all_mods();
}

void EffectsList::getEffList() {
    if (curId != -1) {
        uint8_t *ids = MemoryManager::instance().get_mod_list();
        delete[] ids;
    }
}

// getEffList
// getEffAmount
// getCurEffNum

// setPropVal
// getPropVal
