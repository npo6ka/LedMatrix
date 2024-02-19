#pragma once

enum class ControlState {
    None,
    NextMode,
    PrevMode,
    AutoControlEnable,
    AutoControlDisable,
    AutoControlSwitch,
};

class IControl {
public:
    virtual ControlState tick();
};

