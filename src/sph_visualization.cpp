#include "sph_visualization.h"


void SPHVisualization2D::organize_particles_in_grid(
    const cato::Vec2i& origin,
    const cato::Vec2i& spacing,
    const cato::Vec2i& resolution
) {
    int n_particles = resolution.x * resolution.y;
    _sph_system_data->resize(n_particles);
    cato::Vec2i blc = origin - cato::Vec2i{
        (resolution.x - 1) * spacing.x / 2,
        (resolution.y - 1) * spacing.y / 2
    };
    for (int j = 0; j < resolution.y; ++j) {
        for (int i = 0; i < resolution.x; ++i) {
            int index = j * resolution.x + i;
            cato::Vec2i pos = blc + cato::Vec2i{ i * spacing.x, j * spacing.y };
            _sph_system_data->set_particle_position(index, cato::Vec2d{ static_cast<double>(pos.x), static_cast<double>(pos.y) });
        }
    }
    special_particle_index = (int)(n_particles / 1.75);
}

void SPHVisualization2D::update_graphics() {
    for (size_t i = 0; i < _sph_system_data->n_particles(); ++i) {
        cato::Vec2d pos = _sph_system_data->get_position(i);
        draw_solid_circle(pos.x, pos.y, 3.0);
        if (i == special_particle_index){
            draw_circle(pos.x, pos.y, _sph_system_data->radius());
        }
    }
}