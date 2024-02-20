
#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include <cmath>
#include <random>

namespace implemented
{
    class Boat
    {
    public:
        Boat()
        {
        }

        ~Boat()
        { }

        //TODO:  de sters nefolosita
        void SetPosition(glm::vec3 position, float radius, float speed ) {
            this->positionBase = position;
            positionSupport.x = position.x;
            positionSupport.y = -0.1f;
            positionSupport.z = position.z;


            //directie:
           // forward = glm::normalize(center);
           // initialForward = glm::normalize(center);
            this->angleDir = angleDir;

            //viteza:
            this->speed = speed;

        }

        void SetRadiusSpeed(float rad, float speed) {
            this->radius = rad;
            this->speed = speed;
        }
        void SetColor(glm::vec3 color) {
            this->color = color;
        }

        //glm::vec3(-9, 0.5, -3)
        void UpdateMatrices(float deltaTime) {
        
            //traiectorie: cerc centrat de centrul farului si raza data
            angle += deltaTime * speed;
            glm::vec3 circlePosition;
            positionBase.x = circleCenter.x + radius * glm::cos(angle);
            positionBase.y = 0.5;
            positionBase.z = circleCenter.z + radius * glm::sin(angle);
            
            //suportul:
            positionSupport = positionBase;
            positionSupport.y = -0.1;

            //unghiul de rotatie al corpului barcii de-a lungul traiectoriei:
            float rotationBase;
            float rotationSupport;
            
            //directie buna
            if (radius > 0) {
                rotationBase = -angle;
                rotationSupport = -angle;
            }
            //directie opusa
            else {
                rotationBase = glm::radians(180.0f) - angle;
                rotationSupport = glm::radians(180.0f) - angle;
            }
            baseMatrix = glm::mat4(1);
            baseMatrix = glm::translate(baseMatrix, positionBase);
            baseMatrix = glm::rotate(baseMatrix, rotationBase, glm::vec3(0.0f, 1.0f, 0.0f));
            baseMatrix = glm::scale(baseMatrix, glm::vec3(0.25));

            supportMatrix = glm::mat4(1);
            supportMatrix = glm::translate(supportMatrix, positionSupport);
            supportMatrix = glm::rotate(supportMatrix, rotationSupport, glm::vec3(0.0f, 1.0f, 0.0f));
            supportMatrix = glm::scale(supportMatrix, glm::vec3(0.25));

        }

    public:
        glm::mat4 baseMatrix = glm::mat4(1);
        glm::mat4 supportMatrix = glm::mat4(1);
        glm::vec3 positionBase;
        glm::vec3 positionSupport;
        glm::vec3 initialForward;
        glm::vec3 forward;
        glm::vec3 center;
        float angle = 0;
        float speed;
        glm::vec3 color;

        glm::vec3 rotationCenter = glm::vec3(0, 0, -3);
        float angleDir;

        float radius;
        glm::vec3 circleCenter = glm::vec3(0, 0, -3);
    };
   
}   

