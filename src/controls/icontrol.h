#pragma once

enum class ControlState {
    None,
    PowerOn,
    PowerOff,
    PowerSwich,
    NextMode,
    PrevMode,
    AutoControlEnable,
    AutoControlDisable,
    AutoControlSwitch,
};

class IControl {
public:
    virtual ControlState tick() = 0;
};

