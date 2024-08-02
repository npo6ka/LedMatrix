#include "stdint.h"

class IObservableModeList {
public:
    virtual void eventNextMode() = 0;
    virtual void eventPrevMode() = 0;
};

class IObservableAutomode {
public:
    virtual void eventAutomodeSet(bool val) = 0;
    virtual void eventAutomodeSwitch() = 0;
};

class IObservablePower {
public:
    virtual void eventPowerSet(bool val) = 0;
    virtual void eventPowerSwitch() = 0;
};

class IObservableMode {
public:
    virtual void eventBrightnessSet(int val) = 0;
    virtual void eventBrightnessIncrement(int val) = 0;
    virtual void eventBrightnessReduction(int val) = 0;
    virtual void eventSpeedSet(int val) = 0;
    virtual void eventSpeedIncrement(int val) = 0;
    virtual void eventSpeedReduction(int val) = 0;
    virtual void eventModeSetVal(uint8_t argId, uint64_t val) = 0;
};

class Observer {
    IObservableModeList *modeList = nullptr;
    IObservableAutomode *automode = nullptr;
    IObservablePower *power = nullptr;
    IObservableMode *curMode = nullptr;
public:
    Observer() {}
    ~Observer() {}
    void subscribeModeListEvents(IObservableModeList *inst);
    void subscribeAutomodeEvents(IObservableAutomode *inst);
    void subscribePowerEvents(IObservablePower *inst);
    void subscribeCurModeEvents(IObservableMode *inst);
// Event notification system
// -----------IObservableModeList-----------
    void notityNextMode();
    void notityPrevMode();
// -----------IObservableAutomode-----------
    void notityAutomodeSet(bool val);
    void notityAutomodeSwitch();
// -----------IObservablePower-----------
    void notityPowerSet(bool val);
    void notityPowerSwitch();
// -----------IObservableMode-----------
    void notityBrightnessSet(int val);
    void notityBrightnessIncrement(int val);
    void notityBrightnessReduction(int val);
    void notitySpeedSet(int val);
    void notitySpeedIncrement(int val);
    void notitySpeedReduction(int val);
    template<class T>
    void notityModeSetVal(uint8_t argId, T val);
};