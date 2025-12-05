#ifndef VIEW_H
#define VIEW_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>

#include "sph_system_solver.h"
#include "controller.h"
#include "model.h"
#include "camera.h"
#include "gl_utils.h"

class View
{
public:

	static View* instance()
	{
		if(pView==nullptr){
			pView = new View();
		}
		return pView;
	}

	~View();

	void init( int argc, char** argv, int _width, int _height);
	void display();
	void reshape( int w, int h );
	void idle();
	void main_loop();
	Camera camera;

private:
	
	static View* pView;

	View();
	View( const View& );
	View& operator= (const View&);
	void _init_camera();

	
	int width, height;
	GLuint particle_shader, boundary_shader;
};

View* create_view();

#endif