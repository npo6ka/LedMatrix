#ifndef SHADEREFFECT_H
#define SHADEREFFECT_H

#include "Effects/effect.h"
#include "Effects/lib_glsl.h"

#define IN
#define OUT

using namespace glsl;

class ShaderEffect : public Effect {
public:
    IN vec2 fragCoord;
    IN const vec2 resolution = vec2(WIDTH, HEIGHT);
    IN float time = 0.0;

    OUT vec3 fragColor;

    void on_update() override
    {}

    virtual void on_render()
    {
        time += 1.0 / 60.0;
        for (uint8_t x = 0; x < HEIGHT; x++) {
            for (uint8_t y = 0; y < WIDTH; y++) {
                fragCoord.x = x;
                fragCoord.y = WIDTH - y;
                on_fragment();
                getPix(y, x) = clamp(fragColor, 0.f, 1.f);
            }
        }
    }

    virtual void on_fragment() = 0;
};

#endif // SHADEREFFECT_H
