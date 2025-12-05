#include "shapes.h"


template <typename VecType>
void Shape<VecType>::bind_verts() {
    glGenVertexArrays(1, &this->_vao);
    glGenBuffers(1, &this->_vbo);
    glBindVertexArray(this->_vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
}


template <typename VecType>
VecType Box<VecType>::get_blc() const {
    return this->_center - (_dimensions / static_cast<typename VecType::value_type>(2));
}

template <typename VecType>
VecType Box<VecType>::get_trc() const {
    return this->_center + (_dimensions / static_cast<typename VecType::value_type>(2));
}

template <typename VecType>
void Box<VecType>::bind_verts() {
    Shape<VecType>::bind_verts();

    if constexpr (std::is_same<VecType, cato::Vec2T<typename VecType::value_type>>::value){
        // Do nothing. We are using legacy OpenGL immediate mode for 2D boxes.
    } else if constexpr (std::is_same<VecType, cato::Vec3T<typename VecType::value_type>>::value){
        // Bind 3D box vertices (not implemented)
        VecType half_dim = _dimensions / static_cast<typename VecType::value_type>(2);
        cato::Vec3f center(
            static_cast<float>(this->_center.x),
            static_cast<float>(this->_center.y),
            static_cast<float>(this->_center.z)
        );
        // Don't care about normals because wireframe
        cato::Vec3f vertices[24] = {
            // top square
            center + cato::Vec3f(half_dim.x, half_dim.y, half_dim.z),
            center + cato::Vec3f{half_dim.x, half_dim.y, -half_dim.z},
            // Repeat because just drawing lines
            center + cato::Vec3f{half_dim.x, half_dim.y, -half_dim.z},
            center + cato::Vec3f{-half_dim.x, half_dim.y, -half_dim.z},
            
            center + cato::Vec3f{-half_dim.x, half_dim.y, -half_dim.z},
            center + cato::Vec3f{-half_dim.x, half_dim.y, half_dim.z},

            // Close top
            center + cato::Vec3f{-half_dim.x, half_dim.y, half_dim.z},
            center + cato::Vec3f{half_dim.x, half_dim.y, half_dim.z},

            // Draw the sides
            center + cato::Vec3f{half_dim.x, half_dim.y, half_dim.z},
            center + cato::Vec3f{half_dim.x, -half_dim.y, half_dim.z},

            center + cato::Vec3f{half_dim.x, half_dim.y, -half_dim.z},
            center + cato::Vec3f{half_dim.x, -half_dim.y, -half_dim.z},

            center + cato::Vec3f{-half_dim.x, half_dim.y, -half_dim.z},
            center + cato::Vec3f{-half_dim.x, -half_dim.y, -half_dim.z},

            center + cato::Vec3f{-half_dim.x, half_dim.y, half_dim.z},
            center + cato::Vec3f{-half_dim.x, -half_dim.y, half_dim.z},

            // Now bottom face
            center + cato::Vec3f{half_dim.x, -half_dim.y, half_dim.z},
            center + cato::Vec3f{half_dim.x, -half_dim.y, -half_dim.z},

            center + cato::Vec3f{half_dim.x, -half_dim.y, -half_dim.z},
            center + cato::Vec3f{-half_dim.x, -half_dim.y, -half_dim.z},

            center + cato::Vec3f{-half_dim.x, -half_dim.y, -half_dim.z},
            center + cato::Vec3f{-half_dim.x, -half_dim.y, half_dim.z},

            center + cato::Vec3f{-half_dim.x, -half_dim.y, half_dim.z},
            center + cato::Vec3f{half_dim.x, -half_dim.y, half_dim.z},
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    }
}

template <typename VecType>
void Box<VecType>::draw() const {
    // Placeholder implementation for drawing the box
    if constexpr (std::is_same<VecType, cato::Vec2T<typename VecType::value_type>>::value){
        // Draw 2D box
        VecType halfDim = _dimensions / static_cast<typename VecType::value_type>(2);
        VecType bottomLeft = this->_center - halfDim;
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
            glVertex2f(bottomLeft.x, bottomLeft.y);
            glVertex2f(bottomLeft.x + _dimensions.x, bottomLeft.y);
            glVertex2f(bottomLeft.x + _dimensions.x, bottomLeft.y + _dimensions.y);
            glVertex2f(bottomLeft.x, bottomLeft.y + _dimensions.y);
            glVertex2f(bottomLeft.x, bottomLeft.y);
        glEnd();
    } else if constexpr (std::is_same<VecType, cato::Vec3T<typename VecType::value_type>>::value){
        glDrawArrays(GL_LINES, 0, 24);
    }
}

template <typename VecType>
void SolidBox<VecType>::bind_verts() {
    Shape<VecType>::bind_verts();
    if constexpr (std::is_same<VecType, cato::Vec2T<typename VecType::value_type>>::value){
        // Do nothing. We are using legacy OpenGL immediate mode for 2D boxes.
    } else if constexpr (std::is_same<VecType, cato::Vec3T<typename VecType::value_type>>::value){
        // Bind 3D solid box vertices
        // This would typically involve defining the vertices for all 6 faces
        VecType halfDim = this->get_dimensions() / static_cast<typename VecType::value_type>(2);
        std::cout << "Binding solid box vertices. Dimensions: " << this->get_dimensions().x << ", " << this->get_dimensions().y << ", " << this->get_dimensions().z << std::endl;
        cato::Vec3f center(
            static_cast<float>(this->_center.x),
            static_cast<float>(this->_center.y),
            static_cast<float>(this->_center.z)
        );
        cato::Vec3f vertices[72] = {
            // Front face
            center + cato::Vec3f{-halfDim.x, -halfDim.y,  halfDim.z},
            cato::Vec3f{0, 0, 1},
            center + cato::Vec3f{ halfDim.x, -halfDim.y,  halfDim.z},
            cato::Vec3f{0, 0, 1},
            center + cato::Vec3f{ halfDim.x,  halfDim.y,  halfDim.z},
            cato::Vec3f{0, 0, 1},
            
            center + cato::Vec3f{-halfDim.x, -halfDim.y,  halfDim.z},
            cato::Vec3f{0, 0, 1},
            center + cato::Vec3f{ halfDim.x,  halfDim.y,  halfDim.z},
            cato::Vec3f{0, 0, 1},
            center + cato::Vec3f{-halfDim.x,  halfDim.y,  halfDim.z},
            cato::Vec3f{0, 0, 1},
            
            // Back face
            center + cato::Vec3f{-halfDim.x, -halfDim.y, -halfDim.z},
            cato::Vec3f{0, 0, -1},
            center + cato::Vec3f{-halfDim.x,  halfDim.y, -halfDim.z},
            cato::Vec3f{0, 0, -1},
            center + cato::Vec3f{ halfDim.x,  halfDim.y, -halfDim.z},
            cato::Vec3f{0, 0, -1},
            
            center + cato::Vec3f{-halfDim.x, -halfDim.y, -halfDim.z},
            cato::Vec3f{0, 0, -1},
            center + cato::Vec3f{ halfDim.x,  halfDim.y, -halfDim.z},
            cato::Vec3f{0, 0, -1},
            center + cato::Vec3f{ halfDim.x, -halfDim.y, -halfDim.z},
            cato::Vec3f{0, 0, -1},
        
            // Left face
            center + cato::Vec3f{-halfDim.x, -halfDim.y, -halfDim.z},
            cato::Vec3f{-1, 0, 0},
            center + cato::Vec3f{-halfDim.x, -halfDim.y,  halfDim.z},
            cato::Vec3f{-1, 0, 0},
            center + cato::Vec3f{-halfDim.x,  halfDim.y,  halfDim.z},
            cato::Vec3f{-1, 0, 0},
            
            center + cato::Vec3f{-halfDim.x, -halfDim.y, -halfDim.z},
            cato::Vec3f{-1, 0, 0},
            center + cato::Vec3f{-halfDim.x,  halfDim.y,  halfDim.z},
            cato::Vec3f{-1, 0, 0},
            center + cato::Vec3f{-halfDim.x,  halfDim.y, -halfDim.z},
            cato::Vec3f{-1, 0, 0},
            
            // Right face
            center + cato::Vec3f{ halfDim.x, -halfDim.y, -halfDim.z},
            cato::Vec3f{1, 0, 0},
            center + cato::Vec3f{ halfDim.x,  halfDim.y, -halfDim.z},
            cato::Vec3f{1, 0, 0},
            center + cato::Vec3f{ halfDim.x,  halfDim.y,  halfDim.z},
            cato::Vec3f{1, 0, 0},
            
            center + cato::Vec3f{ halfDim.x, -halfDim.y, -halfDim.z},
            cato::Vec3f{1, 0, 0},
            center + cato::Vec3f{ halfDim.x,  halfDim.y,  halfDim.z},
            cato::Vec3f{1, 0, 0},
            center + cato::Vec3f{ halfDim.x, -halfDim.y,  halfDim.z},
            cato::Vec3f{1, 0, 0},
            
            // Top face
            center + cato::Vec3f{-halfDim.x,  halfDim.y, -halfDim.z},
            cato::Vec3f{0, 1, 0},
            center + cato::Vec3f{-halfDim.x,  halfDim.y, halfDim.z},
            cato::Vec3f{0, 1, 0},
            center + cato::Vec3f{ halfDim.x,  halfDim.y, halfDim.z},
            cato::Vec3f{0, 1, 0},
            
            center + cato::Vec3f{-halfDim.x,  halfDim.y, -halfDim.z},
            cato::Vec3f{0, 1, 0},
            center + cato::Vec3f{ halfDim.x,  halfDim.y, halfDim.z},
            cato::Vec3f{0, 1, 0},
            center + cato::Vec3f{ halfDim.x,  halfDim.y, -halfDim.z},
            cato::Vec3f{0, 1, 0},
            
            // Bottom face
            center + cato::Vec3f{-halfDim.x, -halfDim.y, -halfDim.z},
            cato::Vec3f{0, -1, 0},
            center + cato::Vec3f{ halfDim.x, -halfDim.y, -halfDim.z},
            cato::Vec3f{0, -1, 0},
            center + cato::Vec3f{ halfDim.x, -halfDim.y,  halfDim.z},
            cato::Vec3f{0, -1, 0},
            
            center + cato::Vec3f{-halfDim.x, -halfDim.y, -halfDim.z},
            cato::Vec3f{0, -1, 0},
            center + cato::Vec3f{ halfDim.x, -halfDim.y,  halfDim.z},
            cato::Vec3f{0, -1, 0},
            center + cato::Vec3f{-halfDim.x, -halfDim.y,  halfDim.z},
            cato::Vec3f{0, -1, 0}
        };

        std::cout << "Size of verts " << sizeof(vertices) << std::endl;
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Verts
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(cato::Vec3f::value_type), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(cato::Vec3f::value_type), (GLvoid*)(3 * sizeof(cato::Vec3f::value_type)));
        glEnableVertexAttribArray(1);

    }
}

template <typename VecType>
void SolidBox<VecType>::draw() const {
    // Placeholder implementation for drawing the solid box
    if constexpr (std::is_same<VecType, cato::Vec2T<typename VecType::value_type>>::value){
        // Draw solid 2D box
    } else if constexpr (std::is_same<VecType, cato::Vec3T<typename VecType::value_type>>::value){
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //std::cout << "Should be drawing a solid box of size: " << this->get_dimensions().x << ", " << this->get_dimensions().y << ", " << this->get_dimensions().z << std::endl;
        //std::cout << "At position: " << this->_center.x << ", " << this->_center.y << ", " << this->_center.z << std::endl;
    }
}


template class Box<cato::Vec2T<float>>;
template class Box<cato::Vec3T<float>>;
template class Box<cato::Vec2T<double>>;
template class Box<cato::Vec3T<double>>;

template class SolidBox<cato::Vec2T<float>>;
template class SolidBox<cato::Vec3T<float>>;
template class SolidBox<cato::Vec2T<double>>;
template class SolidBox<cato::Vec3T<double>>;