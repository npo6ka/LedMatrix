#pragma once

#include "core/input/InputCapability.h"
#include "core/input/InputSnapshot.h"

class IInputProvider {
public:
    virtual ~IInputProvider() = default;

    virtual InputCapability capability() const = 0;
    virtual bool isAvailable() const = 0;

    virtual void onActivate() {}
    virtual void onDeactivate() {}

    virtual void tick() = 0;
    virtual void fill(InputSnapshot &snapshot) const = 0;
};
