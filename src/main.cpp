#include "myapplication.h"

MyApplication app;

void setup() {
    app.onInit();
}

void loop() {
    app.onTick();
}
