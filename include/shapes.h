#ifndef SHAPES_H
#define SHAPES_H



#include <GL/glew.h>
#include <GL/glut.h>
#include "vector.h"


template <typename VecType>
class Shape {
public:
    Shape() {}
    Shape(VecType center) : _center(center) {}
    virtual ~Shape() {}

    virtual void bind_verts();
    virtual void draw() const = 0;

protected:
    VecType _center;
    GLuint _vao;
    GLuint _vbo;
};


template <typename VecType>
class Box : public Shape<VecType> {
public:
    Box() {};
    Box(VecType center, VecType dimensions)
        : Shape<VecType>(center), _dimensions(dimensions) {}
    
    ~Box() {};

    VecType get_blc() const;
    VecType get_trc() const;

    const VecType& get_dimensions() const { return _dimensions; }

    
    void draw() const override;
    void bind_verts() override;

private:
    VecType _dimensions;
};


using Box2D = Box<cato::Vec2T<float>>;
using Box3D = Box<cato::Vec3T<float>>;
using Box2Dd = Box<cato::Vec2T<double>>;
using Box3Dd = Box<cato::Vec3T<double>>;



#endif