#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>

#include "model.h"

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

private:

	static Controller* pController;

	Controller();
	Controller( const Controller& );
	Controller& operator= (const Controller& );

};


Controller* create_controller();

#endif