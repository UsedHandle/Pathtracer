#pragma once

#include <cinttypes>
#include <memory>

#include <glm/glm.hpp>

#include "sampler.h"
#include "scene.h"

class Pathtracer{
    // used in radiance()
    // prevents re-initialization of arrays
    // when radiance() is called
    std::unique_ptr<glm::vec3[]> direct;
    std::unique_ptr<glm::vec3[]> emission;
    std::unique_ptr<glm::vec3[]> col;
    std::unique_ptr<float[]> LiWeight;
public:
    Sampler sampler;
    uint32_t numBounces{}; 
    


    Pathtracer(){  }
    Pathtracer(Sampler new_sampler, uint32_t new_numBounces);
    Pathtracer(const Pathtracer& b) = default;
    Pathtracer(Pathtracer&& b) noexcept = default;
    Pathtracer& operator=(const Pathtracer& b) = default;
    Pathtracer& operator=(Pathtracer&& b) noexcept = default;

    glm::vec3 radiance(Ray indirectRay, const Scene* const scene);

};
