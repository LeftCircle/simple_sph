#include "color.h"


colorRGB lerp_color(const colorRGB& c1, const colorRGB& c2, double t){
    colorRGB result;
    result.r = static_cast<float>(c1.r + (c2.r - c1.r) * t);
    result.g = static_cast<float>(c1.g + (c2.g - c1.g) * t);
    result.b = static_cast<float>(c1.b + (c2.b - c1.b) * t);
    return result;
}

