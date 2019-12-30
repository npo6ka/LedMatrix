#pragma once

#include "Effects/23_shadereffect.h"

class TestShader : public ShaderEffect
{
public:
    TestShader() = default;

    void on_fragment() override {
        vec2 position = (fragCoord.xy() / resolution.xy()) - 0.5;
        position *= 1.25;
        position.y *= dot(position, position);

        float den = 0.05;
        float amp = 0.2;
        float freq = 16.;
        float offset = 0.1;

        vec3 colour = vec3 (0.33, 0.2, 0.4) *
             ((1.0f / gl_abs((position.y + (amp * sin((position.x + time * offset) *freq)))) * den)
            + (1.0f / gl_abs((position.y + (amp * sin((position.x + time * offset) *freq+.7f)))) * den)
            + (1.0f / gl_abs((position.y + (amp * sin((position.x + time * offset) *freq-.7f)))) * den));

        fragColor = colour;
    }
};
