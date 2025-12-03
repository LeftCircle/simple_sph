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

void draw_velocity_circle(double x, double y, double r, double vx, double vy) {
    const int num_segments = 20;
    double speed = std::sqrt(vx * vx + vy * vy);
    double max_speed = 100.0; // Define a maximum speed for color mapping
    double t = std::min(speed / max_speed, 1.0);
    colorRGB color = lerp_color(BLUE, RED, t);

    glColor3f(color.r, color.g, color.b);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < num_segments; ++i) {
        double theta = 2.0 * 3.1415926 * static_cast<double>(i) / static_cast<double>(num_segments);
        double dx = r * cos(theta);
        double dy = r * sin(theta);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}