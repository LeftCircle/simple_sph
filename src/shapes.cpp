#include "shapes.h"



template <typename VecType>
void Box<VecType>::draw() const {
    // Placeholder implementation for drawing the box
}


template class Box<cato::Vec2T<float>>;
template class Box<cato::Vec3T<float>>;
template class Box<cato::Vec2T<double>>;
template class Box<cato::Vec3T<double>>;