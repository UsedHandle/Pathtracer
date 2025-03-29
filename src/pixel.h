#pragma once

#include <glm/glm.hpp>
#include <algorithm>

struct pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

[[nodiscard]] inline
glm::vec3 srgbApprox(const glm::vec3& col){
    constexpr float inv_gamma = 1.f/2.2f;
    return glm::vec3(
        std::pow(col.x, inv_gamma),
        std::pow(col.y, inv_gamma),
        std::pow(col.z, inv_gamma));
}

[[nodiscard]] inline
float luminance(const glm::vec3& col){
    return dot(col, glm::vec3(0.2126, 0.7152, 0.0722));
}

[[nodiscard]] inline
glm::vec3 clamp(const glm::vec3& col) {
    return glm::vec3(
        std::clamp(col.x, 0.f, 1.f),
        std::clamp(col.y, 0.f, 1.f),
        std::clamp(col.z, 0.f, 1.f)
    );
}

[[nodiscard]] inline
glm::vec3 reinhardTMO(const glm::vec3& col){
    // this could also use the extended
    // version:
    // L*(1 + L/L_white^2)
    // -----------------
    //       1 + L

    // L_out = L_in/(1.0 + L_in)
    // col = col * L_in/(1.0 + L_in) * 1.0/L_in
    // col = col/(1.0 + L_in)
    const float lum = luminance(col);
    return col/(1.f + lum);
}

[[nodiscard]] inline
// Krzysztof Narkowicz
glm::vec3 ACES_approx(glm::vec3 col) {
    col = col * 0.6f;
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((col * (a * col + b)) / (col * (c * col + d) + e));
}

[[nodiscard]] inline
pixel toPixel(glm::vec3 col){
    
    //col = reinhardTMO(col);
    col = ACES_approx(col);
    col = srgbApprox(col);
    return pixel{
        static_cast<unsigned char>(col.x*255.0),
        static_cast<unsigned char>(col.y*255.0),
        static_cast<unsigned char>(col.z*255.0)
    };
}
