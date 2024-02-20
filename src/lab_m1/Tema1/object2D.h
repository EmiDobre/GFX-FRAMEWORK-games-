#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#include "lab_m1/Tema1/Game.h"

namespace object2D
{
    
    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRomb(const std::string& name, glm::vec3 leftBottomCorner, float length1, float length2, glm::vec4 color);
    Mesh* CreateHex(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec4 color);
    Mesh* CreateStar(const std::string& name, glm::vec3 leftBottomCorner, float proportion, float z,glm::vec4 color);
    glm::vec3 findStarCenter(float prop);
    //matrice de scalare/trans/rotatie:
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        
        return glm::mat3(1, 0, 0, 0, 1, 0, translateX, translateY, 1);

    }
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        return glm::transpose(glm::mat3(scaleX, 0, 0,
            0, scaleY, 0,
            0, 0, 1));
    }
    inline glm::mat3 Rotate(float radians)
    {
        return glm::transpose(glm::mat3(cos(radians), -sin(radians), 0,
            sin(radians), cos(radians), 0,
            0, 0, 1));
    }

}
