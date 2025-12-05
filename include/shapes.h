#ifndef SHAPES_H
#define SHAPES_H



#include <GL/glew.h>
#include <GL/glut.h>
#include "vector.h"

const int CIRCLE_SEGMENTS = 32;

template <typename VecType>
class Shape {
public:
    Shape() {}
    Shape(VecType center) : _center(center) {}
    virtual ~Shape() {}

    virtual void bind_verts();
    virtual void draw() const = 0;
    void bind_vao() const { glBindVertexArray(this->_vao); }

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
    void set_dimensions(const VecType& dimensions) { _dimensions = dimensions; }

    
    virtual void draw() const override;
    virtual void bind_verts() override;

private:
    VecType _dimensions;
};

template <typename VecType>
class SolidBox : public Box<VecType> {
public:
    SolidBox() {};
    SolidBox(VecType center, VecType dimensions)
        : Box<VecType>(center, dimensions) {}

    ~SolidBox() {};

    virtual void draw() const override;
    virtual void bind_verts() override;

};

using Box2D = Box<cato::Vec2T<float>>;
using Box3D = Box<cato::Vec3T<float>>;
using Box2Dd = Box<cato::Vec2T<double>>;
using Box3Dd = Box<cato::Vec3T<double>>;

using SolidBox2D = SolidBox<cato::Vec2T<float>>;
using SolidBox3D = SolidBox<cato::Vec3T<float>>;



#endif