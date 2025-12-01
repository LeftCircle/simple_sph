#include "view.h"

View* View::pView = nullptr;

View* create_view()
{
	return View::instance();
}

View::View() {
	width = 0;
	height = 0;
}


View::~View() {
	// Cleanup if needed
}

void View::init( int argc, char** argv, int _width, int _height)
{
	width = _width;
	height = _height;

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize( width, height );
	glutCreateWindow( "View" );

	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glEnable( GL_DEPTH_TEST );

	glutKeyboardFunc([](unsigned char key, int x, int y) { Controller::instance()->keyboard(key, x, y); });
	glutSpecialFunc([](int key, int x, int y) { Controller::instance()->special_keys(key, x, y); });
	glutDisplayFunc( [](void){ View::instance() -> display(); } );
	glutReshapeFunc( [](int w, int h){ View::instance() -> reshape(w,h); } );
	glutIdleFunc( [](){ View::instance() -> idle(); } );
}

void View::display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

    Model::instance()->sph_system_solver->update_graphics();

	glutSwapBuffers();
	glutPostRedisplay();
}

void View::idle()
{
	Model::instance()->sph_system_solver->update(0.005);
}

void View::reshape( int w, int h )
{
	width = w;
	height = h;

	glViewport( 0, 0, (GLsizei) width, (GLsizei) height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

}

void View::main_loop()
{
	glutMainLoop();
}


void View::draw_circle(const double x, const double y, const double r) const {
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