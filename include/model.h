#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <iostream>


//#include "sph_system_solver.h"

// Forward declaration to avoid circular dependency
template<typename T>
class SPHSystemSolver2T;
using SPHSystemSolver2d = SPHSystemSolver2T<double>;

class Model
{
public:
	enum State {
		DEFAULT,
		JULIA_SET
	};

    static Model* instance() {
		if(pModel==nullptr)
		{
			pModel = new Model();
		}
		return pModel;
	}
    
	~Model();
	
	void on_J_pressed();
	void on_up_arrow_pressed();
	void on_down_arrow_pressed();
	void on_right_arrow_pressed();
	void on_left_arrow_pressed();

    std::unique_ptr<SPHSystemSolver2d> sph_system_solver;


private:

	static Model* pModel;
	State _current_state = DEFAULT;

	Model();
	Model( const Model& );
	Model& operator= (const Model& );

};


Model* create_model();

#endif