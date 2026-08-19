#pragma once

#include "core/input/IInputProvider.h"
#include "core/input/InputSnapshot.h"

#include <cstddef>

class InputHub {
public:
    static constexpr size_t kMaxProviders = 1;

    void registerProvider(IInputProvider *provider);

    void setActiveCapabilities(InputCapability caps);
    InputCapability activeCapabilities() const { return _active; }

    void tick();
    const InputSnapshot &snapshot() const { return _snapshot; }

private:
    IInputProvider *_providers[kMaxProviders] = {};
    size_t _providerCount = 0;
    InputCapability _active = InputCapability::None;
    InputSnapshot _snapshot;
};
