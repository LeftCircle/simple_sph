#ifndef COLOR_H
#define COLOR_H

struct colorRGB{
    float r;
    float g;
    float b;
};

const colorRGB BLUE = {0.0f, 0.0f, 1.0f};
const colorRGB RED = {1.0f, 0.0f, 0.0f};



colorRGB lerp_color(const colorRGB& c1, const colorRGB& c2, double t);




#endif