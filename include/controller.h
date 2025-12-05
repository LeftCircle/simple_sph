#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>

#include "model.h"
#include "view.h"
#include "vector.h"


// A singleton controller class that will be used to drive GLUT inputs
class Controller
{
public:

	static Controller* instance()
	{
		if(pController==nullptr)
		{
			pController = new Controller();
		}
		return pController;
	}

	~Controller();

	void keyboard( unsigned char key, int x, int y );
	void special_keys(int key, int x, int y);
	void print_controls();
	void mouse(int button, int state, int x, int y);
	void mouse_motion(int x, int y);

	bool is_left_mouse_button_down() const { return _left_mouse_button_down; }
	bool is_right_mouse_button_down() const { return _right_mouse_button_down; }
	cato::Vec2i get_current_mouse_position() const { return _current_mouse_position; }

private:

	static Controller* pController;

	bool _left_mouse_button_down = false;
	bool _right_mouse_button_down = false;
	cato::Vec2i _last_mouse_position;
	cato::Vec2i _current_mouse_position;

	Controller();
	Controller( const Controller& );
	Controller& operator= (const Controller& );

};


Controller* create_controller();

#endif