#ifndef SPH_VISUALIZATION_H
#define SPH_VISUALIZATION_H


#include <GL/glew.h>
#include <GL/glut.h>
#include <memory>

#include "basic_gl_graphics.h"
#include "vector.h"
#include "sph_system_data2.h"

class SPHVisualization2D {
public:
    SPHVisualization2D() {_sph_system_data = std::make_shared<SPHSystemData2d>(); }
    ~SPHVisualization2D() {}

    void organize_particles_in_grid(
        const cato::Vec2i& origin,
        const cato::Vec2i& spacing,
        const cato::Vec2i& resolution
    );

    void update_graphics();
    
    std::shared_ptr<SPHSystemData2d> sph_data() { return _sph_system_data; }

    double get_special_particle_density() { return _sph_system_data->get_densities()[special_particle_index]; }
    int special_particle_index = 0;

private:
    std::shared_ptr<SPHSystemData2d> _sph_system_data;
};




#endif