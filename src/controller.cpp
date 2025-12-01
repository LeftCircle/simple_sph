#include "controller.h"


Controller* Controller::pController = nullptr;

Controller* create_controller()
{
	Controller* ctrl = Controller::instance();
	return ctrl;
}

Controller::Controller() {}

Controller::~Controller() {}

void Controller::keyboard( unsigned char key, int x, int y )
{
	Model* model = Model::instance();
	switch (key)
	{
		case 27: // esc
			exit(0);
			break;
	}
}

void Controller::special_keys(int key, int x, int y){
	Model* model = Model::instance();
	switch (key){
		case GLUT_KEY_UP:
			std::cout << "Up arrow pressed" << std::endl;
			model->on_up_arrow_pressed();
			break;
		case GLUT_KEY_DOWN:
			std::cout << "Down arrow pressed" << std::endl;
			model->on_down_arrow_pressed();
			break;
		case GLUT_KEY_RIGHT:
			std::cout << "Right arrow pressed" << std::endl;
			model->on_right_arrow_pressed();
			break;
		case GLUT_KEY_LEFT:
			std::cout << "Left arrow pressed" << std::endl;	
			model->on_left_arrow_pressed();
			break;
	}
}
