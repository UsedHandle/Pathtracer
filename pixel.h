#pragma once

#include <glm/glm.hpp>

struct pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

[[nodiscard]] inline
glm::dvec3 srgbApprox(const glm::dvec3& col){
    constexpr double inv_gamma = 1.0/2.2;
    return glm::dvec3(
        std::pow(col.x, inv_gamma),
        std::pow(col.y, inv_gamma),
        std::pow(col.z, inv_gamma));
}

[[nodiscard]] inline
double luminance(const glm::dvec3& col){
    return dot(col, glm::dvec3(0.2126, 0.7152, 0.0722));
}

[[nodiscard]] inline
glm::dvec3 reinhardTMO(const glm::dvec3& col){
    // this could also use the extended
    // version:
    // L*(1 + L/L_white^2)
    // -----------------
    //       1 + L

    // L_out = L_in/(1.0 + L_in)
    // col = col * L_in/(1.0 + L_in) * 1.0/L_in
    // col = col/(1.0 + L_in)
    const double lum = luminance(col);
    return col/(1.0 + lum);
}

[[nodiscard]] inline
pixel toPixel(glm::dvec3 col){
    
    col = reinhardTMO(col);
    col = srgbApprox(col);
    return pixel{
        static_cast<unsigned char>(col.x*255.0),
        static_cast<unsigned char>(col.y*255.0),
        static_cast<unsigned char>(col.z*255.0)
    };
}
