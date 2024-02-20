#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace implemented
{
    class CameraTank
    {
     public:
        CameraTank()
        {
            position = glm::vec3(0, 2, 5);
            forward = glm::vec3(0, 0, -1);
            up = glm::vec3(0, 1, 0);
            right = glm::vec3(1, 0, 0);
            distanceToTarget = 4.0f;
        }

        ~CameraTank()
        { }

        // Update camera
        void Set(const glm::vec3& position, const glm::vec3 center, const glm::vec3& up)
        {
            this->position = position;
            forward = glm::normalize(center - position);
            initialForward = forward;
            right = glm::cross(forward, up);
            this->up = glm::cross(right, forward);
            this->center = center;
        }

        void MoveForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void RotateFirstPerson_OY(float angle)
        {
            //glm::vec4 rotatedForward = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(initialForward, 0);
           // forward = glm::normalize(glm::vec3(rotatedForward));
            glm::vec4 aux = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0);
            forward = glm::normalize(glm::vec3(aux));

            aux = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0);
            right = glm::normalize(glm::vec3(aux));

            up = glm::cross(right, forward);

        }


        void RotateAroundTank(float angle) {
            MoveForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            MoveForward(-distanceToTarget);
        }

        

        glm::mat4 GetViewMatrix()
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

     public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;

        glm::vec3 initialForward;
        glm::vec3 center;
        
    };
}   // namespace implemented
