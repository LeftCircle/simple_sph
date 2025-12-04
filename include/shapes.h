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

private:
    VecType _dimensions;

};




#endif