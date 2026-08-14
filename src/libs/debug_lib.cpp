#include "debug_lib.h"

#include "configs/constants.h"

#if DEBUG
    #include <Arduino.h>
    #include <stdarg.h>
    #include <stdio.h>

    void debugSetup()
    {
        Serial.begin(115200);
    }

    const size_t out(const char *szFormat, ...)
    {
        char buffer[256];
        va_list argptr;
        va_start(argptr, szFormat);
        const int written = vsnprintf(buffer, sizeof(buffer), szFormat, argptr);
        va_end(argptr);
        if (written <= 0) {
            return 0;
        }
        Serial.print(buffer);
        return static_cast<size_t>(written);
    }
#else
    void debugSetup() {};
    const size_t out(const char *szFormat, ...) {};
#endif
