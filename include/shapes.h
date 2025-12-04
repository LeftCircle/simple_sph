#ifndef SHAPES_H
#define SHAPES_H

#include "vector.h"

template <typename VecType>
class Shape {
public:
    Shape() {}
    Shape(VecType center) : _center(center) {}
    virtual ~Shape() {}

    virtual void draw() const = 0;

private:
    VecType _center;
};


template <typename VecType>
class Box : public Shape<VecType> {
public:
    Box() {};
    Box(VecType center, VecType dimensions)
        : Shape<VecType>(center), _dimensions(dimensions) {}
    
    ~Box() {};

    void draw() const override;

private:
    VecType _dimensions;
};


using Box2D = Box<cato::Vec2T<float>>;
using Box3D = Box<cato::Vec3T<float>>;
using Box2Dd = Box<cato::Vec2T<double>>;
using Box3Dd = Box<cato::Vec3T<double>>;



#endif