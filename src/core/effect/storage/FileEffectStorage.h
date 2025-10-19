# pragma once

#include "core/file/IFileHandler.h"
#include "core/Variable/FileSavableVariable.h"
#include "libs/debug_lib.h"
#include "IEffectStorage.h"
#include "StaticEffectStorage.h"

#include <stdint.h>
#include "vector"

class FileEffectStorage : public IEffectStorage
{
private:
    IFileHandler *_fileHandler;
    FileSavableVariable<uint32_t> _currentEffectIndex;
    std::vector<FileSavableVariable<EffectInfo>> _effects;
    uint32_t _offset;
public:
    FileEffectStorage(IFileHandler *fileHandler) : _fileHandler(fileHandler), _currentEffectIndex(fileHandler, 0, 0) {
        _offset = _currentEffectIndex.size();

        size_t savedEffectAmount = (_fileHandler->size() - _currentEffectIndex.size()) / EffectInfo::typeSize();
        for (size_t i = 0; i < savedEffectAmount; i++) {
            internalAddEffect(EffectInfo(), true);
            // удалить режим из памяти можно только занулив savedIndex,
            // поэтому, если встречаем удалённый режим, дальше загружать не нужно
            if (_effects.back().get().id == 0) {
                internalRemoveEffect();
                break;
            }
        }

        if (_effects.empty()) {
            createDefaultEffectsList();
        }

        if (_currentEffectIndex.get() >= _effects.size()) {
            logError("Incorrect currentEffectIndex. Value is out of effects size\n");
            _currentEffectIndex.set(0);
        }
    }
    virtual ~FileEffectStorage() {};

    virtual EffectInfo getEffectInfo(uint32_t index) const override {
        if (index >= _effects.size()) {
            logError("Cannot get effect info: index is out of range\n");
            return EffectInfo();
        }
        return _effects[index].get();
    }

    virtual uint32_t getCurrentIndex() const override {
        return _currentEffectIndex.get();
    }

    virtual void setCurrentIndex(uint32_t index) override {
        if (index >= _effects.size()) {
            logError("Cannot set current index: index is out of range\n");
            return;
        }
        _currentEffectIndex.set(index);
    }

    virtual void addEffect(uint32_t effectId) override {
        if (effectId == 0) {
            logError("Cannot add effect with id 0\n");
            return;
        }
        internalAddEffect(EffectInfo(effectId, generateSavedIndex()), false);
    }

    virtual void addEffect(uint32_t effectId, uint32_t position) override {
        if (position > _effects.size()) {
            logError("Cannot add effect: position is out of range\n");
            return;
        }

        std::vector<EffectInfo> localEffects;
        localEffects.reserve(_effects.size() - position);

        // сохраняем эффекты, которые будут сдвинуты
        for (size_t i = position; i < _effects.size(); i++) {
            localEffects.push_back(_effects[i].get());
        }
        // удаляем их из файла
        for (size_t i = 0; i < localEffects.size(); i++) {
            internalRemoveEffect();
        }
        // добавляем новый эффект
        internalAddEffect(EffectInfo(effectId, generateSavedIndex()), false);
        // возвращаем сдвинутые эффекты обратно
        for(const auto& effect : localEffects) {
            internalAddEffect(effect, false);
        }
    }

    virtual void removeEffect() override {
        if (_effects.empty()) {
            logError("Cannot remove effect: effects list is empty\n");
            return;
        }
        _effects[_effects.size() - 1].set({0, 0});
        internalRemoveEffect();
    }

    virtual void removeEffect(uint32_t position) override {
        if (position >= _effects.size()) {
            logError("Cannot remove effect: position is out of range\n");
            return;
        }

        std::vector<EffectInfo> localEffects;
        localEffects.reserve(_effects.size() - position - 1);
        // сохраняем эффекты, которые будут сдвинуты
        for (size_t i = position + 1; i < _effects.size(); i++) {
            localEffects.push_back(_effects[i].get());
        }
        // помечаем последний эффект как удалённый
        _effects[_effects.size() - 1].set({0, 0});
        // удаляем эффекты из файла
        for (size_t i = 0; i < localEffects.size() + 1; i++) {
            internalRemoveEffect();
        }
        // возвращаем сдвинутые эффекты обратно
        for(const auto& effect : localEffects) {
            internalAddEffect(effect, false);
        }
    }

    virtual size_t size() const override {
        return _effects.size();
    }

    virtual void reset() override {
        clear();
        createDefaultEffectsList();
    }

    virtual void clear() override {
        for (size_t i = 0; i < _effects.size(); i++) {
            _effects[i].set({0, 0});
        }
        _effects.clear();
        _offset = _currentEffectIndex.size();
        _currentEffectIndex.set(0);
    }
private:
    uint32_t generateSavedIndex(uint32_t tryCount = 0) {
        uint32_t generated_index = random(0xFFFFFFFE) + 1;
        for (const auto& effect : _effects) {
            if (effect.get().savedIndex == generated_index && tryCount < 10) {
                tryCount++;
                generated_index = generateSavedIndex(tryCount);
            }
        }
        return generated_index;
    }

    void internalAddEffect(const EffectInfo& effectInfo, bool loadOnCreate) {
        _effects.emplace_back(_fileHandler, _offset, std::move(effectInfo), loadOnCreate);
        _offset += EffectInfo::typeSize();
    }

    void internalRemoveEffect() {
        if (_effects.empty()) {
            logError("Cannot remove effect: effects list is empty\n");
            return;
        }

        _effects.pop_back();
        _offset -= EffectInfo::typeSize();

        if (_currentEffectIndex.get() >= _effects.size()) {
            _currentEffectIndex.set(0);
        }
    }

    void createDefaultEffectsList() {
        logInfo("Creating default effects list\n");
        StaticEffectStorage defaultStorage;
        for (size_t i = 0; i < defaultStorage.size(); i++) {
            internalAddEffect(EffectInfo(defaultStorage.getEffectInfo(i).id, generateSavedIndex()), false);
        }
        _currentEffectIndex.set(0);
    }
};

// Test code:
//         LsfFileHandler fileHandler;
//         //LittleFS.remove("/mods.txt");
//         listFiles();

//         fileHandler.open("mods.txt");

//         FileEffectStorage storage(&fileHandler);

//         logInfo("Current index: %u\n", storage.getCurrentIndex());
//         logInfo("Total effects: %zu\n", storage.size());
//         printAllMods(storage);

//         storage.reset();
//         logInfo("After reset:\n");
//         logInfo("Current index: %u\n", storage.getCurrentIndex());
//         logInfo("Total effects: %zu\n", storage.size());
//         printAllMods(storage);

//         storage.removeEffect(5);
//         storage.removeEffect(5);
//         storage.removeEffect(100);
//         storage.removeEffect(storage.size());
//         logInfo("After removeEffect(5) and removeEffect(28):\n");
//         logInfo("Current index: %u\n", storage.getCurrentIndex());
//         logInfo("Total effects: %zu\n", storage.size());
//         printAllMods(storage);

//         for (size_t i = storage.size() + 1; i > 0; i--) {
//             storage.removeEffect();
//         }
//         logInfo("After removing all but one effect:\n");
//         logInfo("Current index: %u\n", storage.getCurrentIndex());
//         logInfo("Total effects: %zu\n", storage.size());
//         printAllMods(storage);

//         storage.addEffect(5);
//         storage.addEffect(10);
//         storage.addEffect(15, 1);
//         storage.addEffect(8, 0);
//         storage.addEffect(12, 10);
//         logInfo("After adding effects:\n");
//         logInfo("Current index: %u\n", storage.getCurrentIndex());
//         logInfo("Total effects: %zu\n", storage.size());
//         printAllMods(storage);

//         storage.setCurrentIndex(2);
//         storage.setCurrentIndex(100);
//         logInfo("After setting current index to 2 and 100:\n");
//         logInfo("Current index: %u\n", storage.getCurrentIndex());

//         EffectInfo info = storage.getEffectInfo(2);
//         logInfo("Effect at index 2: ID=%u, SavedIndex=%u\n", info.id, info.savedIndex);

//         storage.clear();
//         logInfo("After clear:\n");
//         logInfo("Current index: %u\n", storage.getCurrentIndex());
//         logInfo("Total effects: %zu\n", storage.size());