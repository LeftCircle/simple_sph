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

	glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        exit(1);
    }

	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glEnable( GL_DEPTH_TEST );

	glutKeyboardFunc([](unsigned char key, int x, int y) { Controller::instance()->keyboard(key, x, y); });
	glutSpecialFunc([](int key, int x, int y) { Controller::instance()->special_keys(key, x, y); });
	glutMouseFunc([](int button, int state, int x, int y) { Controller::instance()->mouse(button, state, x, y); });
	glutPassiveMotionFunc([](int x, int y) { Controller::instance()->mouse_motion(x, y); });
	glutMotionFunc([](int x, int y) { Controller::instance()->mouse_motion(x, y); });
	glutDisplayFunc( [](void){ View::instance() -> display(); } );
	glutReshapeFunc( [](int w, int h){ View::instance() -> reshape(w,h); } );
	//glutIdleFunc( [](){ View::instance() -> idle(); } );

	particle_shader = create_programme_from_files(
		"shaders/vs.glsl",
		"shaders/fbasic.glsl"
	);
	boundary_shader = create_programme_from_files(
		"shaders/vs.glsl",
		"shaders/fbasic.glsl"
	);
	_init_camera();

	glutTimerFunc(16, [](int){ View::instance()->idle(); }, 0);
}

void View::_init_camera()
{
	float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
	camera.set_perspective_projection(35.0f, aspect_ratio, 0.1f, 10000.0f);
	camera.lookat(camera.position, camera.target, cato::Vec3f(0, 1, 0));
}

void View::display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

    //Model::instance()->sph_system_solver->update_graphics();
	camera.lookat(camera.position, cato::Vec3f(0.0), cato::Vec3f(0, 1, 0));
	camera.set_perspective_projection(35.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 10000.0f);
	Model::instance()->sph_system_solver->update_graphics(
		particle_shader,
		boundary_shader,
		camera.view_mat,
		camera.projection_mat
	);
	// The fun visualization stuff
	//Model::instance()->sph_visualization->update_graphics();
	
	glutSwapBuffers();
}

void View::idle()
{
	//auto start_time = glutGet(GLUT_ELAPSED_TIME);
	
	Model::instance()->simulate();
	
	//auto end_time = glutGet(GLUT_ELAPSED_TIME);
	//std::cout << "Frame time: " << (end_time - start_time) << " ms\n";
	glutPostRedisplay();
	//Model::instance()->sph_system_solver->update(0.005);
	glutTimerFunc(16, [](int){ View::instance()->idle(); }, 0);

}

void View::reshape( int w, int h )
{
	width = w;
	height = h;

	glViewport( 0, 0, (GLsizei) width, (GLsizei) height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	// Needed for drawing the 2D particles
	gluOrtho2D(0, width, 0, height);

}

void View::main_loop()
{
	glutMainLoop();
}
