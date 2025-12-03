#ifndef BASIC_GL_GRAPHICS_H
#define BASIC_GL_GRAPHICS_H



#include <GL/glew.h>
#include <GL/glut.h>

#include "vector.h"
#include "color.h"


void draw_circle(double x, double y, double r);
void draw_solid_circle(double x, double y, double r);

void draw_velocity_circle(double x, double y, double r, double vx, double vy);


#endif