#include "basic_gl_graphics.h"


void draw_circle(double x, double y, double r) {
	const int num_segments = 20;
	glColor3f(1.0f, 0.0f, 0.0f); // Red color
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < num_segments; ++i) {
		double theta = 2.0 * 3.1415926 * static_cast<double>(i) / static_cast<double>(num_segments);
		double dx = r * cos(theta);
		double dy = r * sin(theta);
		glVertex2f(x + dx, y + dy);
	}
	glEnd();
}

// Function to draw a solid circle
void draw_solid_circle(double x, double y, double r) {
    const int num_segments = 20;
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // Center of circle
    for (int i = 0; i <= num_segments; ++i) {
        double theta = 2.0 * 3.1415926 * static_cast<double>(i) / static_cast<double>(num_segments);
        double dx = r * cos(theta);
        double dy = r * sin(theta);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}