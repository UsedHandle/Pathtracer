#pragma once

#include <glm/glm.hpp>

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
pixel toPixel(glm::vec3 col){
    
    col = reinhardTMO(col);
    col = srgbApprox(col);
    return pixel{
        static_cast<unsigned char>(col.x*255.0),
        static_cast<unsigned char>(col.y*255.0),
        static_cast<unsigned char>(col.z*255.0)
    };
}
