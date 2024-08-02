
#include "libs/debug_lib.h"
#include "observer.h"


void Observer::Observer::subscribeModeListEvents(IObservableModeList *inst) {
    if (modeList) out("WARN: Observer: Multipule initialization modeList\n");
    modeList = inst;
}
void Observer::subscribeAutomodeEvents(IObservableAutomode *inst) {
    if (automode) out("WARN: Observer: Multipule initialization automode\n");
    automode = inst;
}
void Observer::subscribePowerEvents(IObservablePower *inst) {
    if (power) out("WARN: Observer: Multipule initialization power\n");
    power = inst;
}
void Observer::subscribeCurModeEvents(IObservableMode *inst) {
    if (curMode) out("WARN: Observer: Multipule initialization curMode\n");
    curMode = inst;
}
// -------------------------------------
void Observer::notityNextMode() {
    modeList->eventNextMode();
}

void Observer::notityPrevMode() {
    modeList->eventPrevMode();
}
// -------------------------------------
void Observer::notityAutomodeSet(bool val) {
    automode->eventAutomodeSet(val);
}

void Observer::notityAutomodeSwitch() {
    automode->eventAutomodeSwitch();
}
// -------------------------------------
void Observer::notityPowerSet(bool val) {
    power->eventPowerSet(val);
}

void Observer::notityPowerSwitch() {
    power->eventPowerSwitch();
}
// -------------------------------------
void Observer::notityBrightnessSet(int val) {
    curMode->eventBrightnessSet(val);
}

void Observer::notityBrightnessIncrement(int val) {
    curMode->eventBrightnessIncrement(val);
}

void Observer::notityBrightnessReduction(int val) {
    curMode->eventBrightnessReduction(val);
}

void Observer::notitySpeedSet(int val) {
    curMode->eventSpeedSet(val);
}

void Observer::notitySpeedIncrement(int val) {
    curMode->eventSpeedIncrement(val);
}

void Observer::notitySpeedReduction(int val) {
    curMode->eventSpeedReduction(val);
}

template<class T>
void Observer::notityModeSetVal(uint8_t argId, T val) {
    curMode->eventModeSetVal(argId, (uint64_t)val);
}
