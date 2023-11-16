#pragma once
#include "glm/glm.hpp"
namespace MatGl {
    struct SpotLight {
        glm::vec3 Position;
        glm::vec3 Direction;
        float CutOff;
        float OuterCutOff;

        float Constant;
        float Linear;
        float Quadratic;

        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;
    };
}