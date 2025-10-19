#include "debug_lib.h"

#include "configs/constants.h"

#if DEBUG
    #include <SoftwareSerial.h>

    void debugSetup()
    {
        Serial.begin(115200);
    }

    const size_t out(const char *szFormat, ...)
    {
        va_list argptr;
        va_start(argptr, szFormat);
        char *szBuffer = 0;
        const size_t nBufferLength = vsnprintf(szBuffer, 0, szFormat, argptr) + 1;
        if (nBufferLength == 1) return 0;
        szBuffer = (char *) malloc(nBufferLength);
        if (! szBuffer) return - nBufferLength;
        vsnprintf(szBuffer, nBufferLength, szFormat, argptr);
        Serial.print(szBuffer);
        free(szBuffer);
        return nBufferLength - 1;
    }
#else
    void debugSetup() {};
    const size_t out(const char *szFormat, ...) {};
#endif
