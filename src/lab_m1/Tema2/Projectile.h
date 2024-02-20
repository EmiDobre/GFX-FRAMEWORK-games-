
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace implemented
{
    class Projectile
    {
    public:
        Projectile()
        {
        }

        ~Projectile()
        { }
        //1) init proj cu traiectoria turetei:
        void SetProjectile(const glm::vec3 positionTuret, glm::vec3 forwardTurret, float angleTurret) {
               position = positionTuret;
               forward = forwardTurret;
               initialForward = forward;
               angle = angleTurret;
        }

        //1.2) Activare proj: 
        void ActivateProjectile() {
            active = true;
        }
        void DezactivateProjectile() {
            active = false;
        }

        //2) Misc proiectilul pentru un timp cu o distanta
        void MoveForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position -= dir * distance; //directie opusa fata de el
            UpdateMatrices();
            
        }
        //3)Matrice: apelata doar daca proj e activ
        void UpdateMatrices() {
            if (freeze == false ) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, position);
                glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f);
                modelMatrix = glm::rotate(modelMatrix, angle, rotationAxis);
                matrix = modelMatrix;
            }
        }

        void Freeze() {
            freeze = true;
        }

    

    public:
        //matrice:
        glm::mat4 matrix = glm::mat4(1);
        //implementare cu forward si normalizare:
        glm::vec3 initialForward = glm::vec3(1);
        glm::vec3 forward = glm::vec3(1);
        glm::vec3 position = glm::vec3(1);
        bool active = false;
        float angle;

        float radius = 0.1;
        bool freeze = false;
    };

}

