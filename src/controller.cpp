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
	auto particle_system = model->sph_system_solver->sphSystemData();
	switch (key)
	{
		case 27: // esc
			exit(0);
			break;
		case 'd':{
			// Decrase the target density
			double new_density = particle_system->get_target_density() * 0.9;
			particle_system->set_target_density(new_density);
			std::cout << "Decreased target density to " << particle_system->get_target_density() << "\n";
			break;
		}
		case 'D':{
			// Increase the target density
			double new_density = particle_system->get_target_density() * 1.1;
			particle_system->set_target_density(new_density);
			std::cout << "Increased target density to " << particle_system->get_target_density() << "\n";
			break;
		}
		case 'e':{
			// decrease eos exponent
			double new_eos = model->sph_system_solver->get_eos_exponent() * 0.9;
			model->sph_system_solver->set_eos_exponent(new_eos);
			std::cout << "Decreased EoS exponent to " << model->sph_system_solver->get_eos_exponent() << "\n";
			break;
		}
		case 'E':{
			// increase eos exponent
			double new_eos = model->sph_system_solver->get_eos_exponent() * 1.1;
			model->sph_system_solver->set_eos_exponent(new_eos);
			std::cout << "Increased EoS exponent to " << model->sph_system_solver->get_eos_exponent() << "\n";
			break;
		}
		case 'g':{
			// decrase gravity
			double new_gravity = model->sph_system_solver->get_gravity() * 0.9;
			model->sph_system_solver->set_gravity(new_gravity);
			std::cout << "Decreased gravity to " << model->sph_system_solver->get_gravity() << "\n";
			break;
		}
		case 'G':{
			// increase gravity
			double new_gravity = model->sph_system_solver->get_gravity() * 1.1;
			model->sph_system_solver->set_gravity(new_gravity);
			std::cout << "Increased gravity to " << model->sph_system_solver->get_gravity() << "\n";
			break;
		}
		case 'k':{
			// decrase particle radius
			particle_system->set_radius(particle_system->radius() * 0.9);
			std::cout << "Decreased particle radius to " << particle_system->radius() << "\n";
			break;
		}
		case 'K':{
			// increase particle radius
			particle_system->set_radius(particle_system->radius() * 1.1);
			std::cout << "Increased particle radius to " << particle_system->radius() << "\n";
			break;
		}
		case 'm':{
			// Decrase the mass of the particles
			particle_system->set_mass(particle_system->mass() * 0.9);
			std::cout << "Decreased particle mass to " << particle_system->mass() << "\n";
			break;
		}
		case 'M':{
			// Increase the mass of the particles
			particle_system->set_mass(particle_system->mass() * 1.1);
			std::cout << "Increased particle mass to " << particle_system->mass() << "\n";
			break;
		}
		case 'r':{
			// Reset particle positions
			particle_system->organize_particles_in_grid(
				cato::Vec2i(400, 300),
				cato::Vec2i(10, 10),
				cato::Vec2i(30, 30)
			);
			particle_system->clear_velocities();
			std::cout << "Reset particle positions\n";
			break;
		}
		case 's':{
			// decrease speed of sound
			double new_sos = model->sph_system_solver->get_speed_of_sound() * 0.9;
			model->sph_system_solver->set_speed_of_sound(new_sos);
			std::cout << "Decreased speed of sound to " << model->sph_system_solver->get_speed_of_sound() << "\n";
			break;
		}
		case 'S':{
			// increase speed of sound
			double new_sos = model->sph_system_solver->get_speed_of_sound() * 1.1;
			model->sph_system_solver->set_speed_of_sound(new_sos);
			std::cout << "Increased speed of sound to " << model->sph_system_solver->get_speed_of_sound() << "\n";
			break;
		}
		
		
		case 'v':{
			// decrease viscosity
			double new_viscosity = model->sph_system_solver->get_viscosity_coefficient() * 0.9;
			model->sph_system_solver->set_viscosity_coefficient(new_viscosity);
			std::cout << "Decreased viscosity to " << model->sph_system_solver->get_viscosity_coefficient() << "\n";
			break;
		}
		case 'V':{
			// increase viscosity
			double new_viscosity = model->sph_system_solver->get_viscosity_coefficient() * 1.1;
			model->sph_system_solver->set_viscosity_coefficient(new_viscosity);
			std::cout << "Increased viscosity to " << model->sph_system_solver->get_viscosity_coefficient() << "\n";
			break;
		}
		
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

void Controller::mouse(int button, int state, int x, int y){
	// Check for left mouse click
	switch(button){
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN){
				_left_mouse_button_down = true;
			} else if (state == GLUT_UP){
				_left_mouse_button_down = false;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN){
				_right_mouse_button_down = true;
			} else if (state == GLUT_UP){
				_right_mouse_button_down = false;
			}
			break;
	}
	_last_mouse_position = _current_mouse_position;
	_current_mouse_position = cato::Vec2i(x, y);
}

void Controller::print_controlls(){
	std::cout << "Controls:\n";
	std::cout << "  Esc: Exit the program\n";
	std::cout << "  m/M: Decrease/Increase particle mass\n";
	std::cout << "  k/K: Decrease/Increase particle radius\n";
	std::cout << "  d/D: Decrease/Increase target density\n";
	std::cout << "  g/G: Decrease/Increase gravity\n";
	std::cout << "  v/V: Decrease/Increase viscosity coefficient\n";
	std::cout << "  s/S: Decrease/Increase speed of sound\n";
	std::cout << "  e/E: Decrease/Increase EoS exponent\n";
	std::cout << "  r: Reset particle positions\n";
}