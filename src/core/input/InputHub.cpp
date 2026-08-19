#include "InputHub.h"

#include <Arduino.h>

void InputHub::registerProvider(IInputProvider *provider) {
    if (!provider || _providerCount >= kMaxProviders) {
        return;
    }
    _providers[_providerCount++] = provider;
}

void InputHub::setActiveCapabilities(InputCapability caps) {
    if (caps == _active) {
        return;
    }

    const InputCapability prev = _active;

    for (size_t i = 0; i < _providerCount; ++i) {
        IInputProvider *provider = _providers[i];
        if (!provider || !provider->isAvailable()) {
            continue;
        }

        const InputCapability cap = provider->capability();
        if (hasCapability(prev, cap) && !hasCapability(caps, cap)) {
            provider->onDeactivate();
        }
    }

    _active = caps;

    for (size_t i = 0; i < _providerCount; ++i) {
        IInputProvider *provider = _providers[i];
        if (!provider || !provider->isAvailable()) {
            continue;
        }

        const InputCapability cap = provider->capability();
        if (!hasCapability(prev, cap) && hasCapability(caps, cap)) {
            provider->onActivate();
        }
    }
}

void InputHub::tick() {
    _snapshot.clear();
    _snapshot.frameMs = millis();

    for (size_t i = 0; i < _providerCount; ++i) {
        IInputProvider *provider = _providers[i];
        if (!provider || !provider->isAvailable()) {
            continue;
        }
        if (!hasCapability(_active, provider->capability())) {
            continue;
        }

        provider->tick();
        provider->fill(_snapshot);
    }
}
