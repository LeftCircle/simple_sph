#include <iostream>
#include <cstdlib>
#include "sph_animation.h"

SPHAnimationPtr sph_animation;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	sph_animation->update(0.016f);

	glutSwapBuffers();
	glutPostRedisplay();
}

// From https://people.computing.clemson.edu/~dhouse/courses/817/index.html examples
void doReshape(int w, int h){
  int vpw, vph;
  
  float aspect = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
  if(float(w) / float(h) > aspect){
    vph = h;
    vpw = int(aspect * h + 0.5);
  }
  else{
    vpw = w;
    vph = int(w / aspect + 0.5);
  }
  
  glViewport(0, 0, vpw, vph);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
  
  glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv) {
	if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <num_particles>\n";
        return 1;
    }
    int num_particles = std::atoi(argv[1]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("SPH Simulation");

	glewInit();

	sph_animation = std::make_shared<SPHAnimation>();
	sph_animation->get_particle_system()->resize(num_particles);
	sph_animation->get_particle_system()->randomize_particles(0.0f, 800.0f, 0.0f, 600.0f);

	glutReshapeFunc(doReshape);
	glutDisplayFunc(display);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glutMainLoop();

	sph_animation.reset();
	return 0;

}