#include <stdint.h>
#include "effect.h"

class EffectFactory {
    public:
        static Effect* createEffect(int effect_id);
        static const char* getEffectName(int effect_id);
        static uint16_t getEffectCount();
};


