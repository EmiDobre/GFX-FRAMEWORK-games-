#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "lab_m1/Tema2/Projectile.h"

#define IN_LIMITS 1
#define OFF_LIMITS 0

namespace implemented
{
    class Tank
    {
    public:
        Tank()
        {
        }

        ~Tank()
        { }

        //1)init tank dupa tip  (stanga/dreapta; sus/jos; in fata/in spate)
        void StartTankPosition(const glm::vec3 positionTank,const glm::vec3 center, const float distTarget) {
            position = positionTank;
            //right,up,forward to do in fct de centru:
            forward = glm::normalize(center - position);
            initialForward = glm::normalize(center - position);
           
            this->distanceToTarget = distTarget;
            this->center = center;
            //tureta:
            positionTurret = positionTank;
            forwardTurret = glm::normalize(center - position);

            //init matrice:
            UpdateMatrices();
        }

        //2) W/S:
        void MoveForward(float distance)
        {   
            //body:
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;

            //tureta se afla la alta poz:
            glm::vec3 dirTurret = glm::normalize(glm::vec3(forwardTurret.x, 0, forwardTurret.z));
            positionTurret += dirTurret * distance;

            //update matrice:
            UpdateMatrices();
        }

        //3) A/D
        void RotateTank(float angle, float angleTur) {
            //setez:
            angleBody = angle;
            angleTurret = angleTur;
            //schimbare directie body
            glm::vec4 rotatedForward = glm::rotate(glm::mat4(1.0f), angleBody, glm::vec3(0, 1, 0)) * glm::vec4(initialForward, 0);
            forward = glm::normalize(glm::vec3(rotatedForward));

            //setare matrice:
            UpdateMatrices();
        }
        
        //4) rotatie in plus a turelei:
        void RotateTurret(float angle) {
            angleTurret = angle;

            //actualizare forward tureta
            glm::vec4 rotatedForwardTurret = glm::rotate(glm::mat4(1.0f), angleTurret, glm::vec3(0, 1, 0)) * glm::vec4(initialForward, 0);
            forwardTurret = glm::normalize(glm::vec3(rotatedForwardTurret));
            
            UpdateMatrices();
        }

        //5) Incarcare nou proiectil (obiect init + setare forward ul sau):
        void ShootProjectile(int nrProj) {
            nrProjectiles = nrProj;
            Projectile* projectile = new Projectile();
            projectiles.push_back(projectile);  
            projectiles[nrProjectiles]->SetProjectile(position, forwardTurret, angleTurret);

        }

        //5)update doar daca pozitia respecta limitele scenei:
        void UpdateMatrices() {
            if (life > 0 && freeze == false ) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, position);
                glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f);
                modelMatrix = glm::rotate(modelMatrix, angleBody, rotationAxis);

                glm::mat4 modelTurret = glm::mat4(1);
                modelTurret = glm::translate(modelTurret, position);
                modelTurret = glm::rotate(modelTurret, angleTurret, rotationAxis);

                MatrixSetter(modelMatrix, modelMatrix, modelTurret, modelTurret);
            }
          //  printf("pos tank: x= %f, y = %f, z = %f\n", position.x, position.y, position.z);
        }
        
      
        void MatrixSetter(glm::mat4 railsModel, glm::mat4 bodyModel, glm::mat4 turretModel,
            glm::mat4 gunModel) {
            rails = railsModel;
            body = bodyModel;
            turret = turretModel;
            gun = gunModel;
        }
        
        void Freeze() {
            freeze = true;
        }

    public:
       //matrici:
        glm::mat4 rails = glm::mat4(1);
        glm::mat4 body = glm::mat4(1); 
        glm::mat4 turret = glm::mat4(1);
        glm::mat4 gun = glm::mat4(1);

        //implementare cu forward si normalizare:
        glm::vec3 initialForward;
        glm::vec3 forward;
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 center;

        //rotiri:
        float angleBody = 0;
        float angleTurret = 0;
        glm::vec3 forwardTurret;
        glm::vec3 positionTurret;

        //tankul are un nr de proiectile 
        std::vector<Projectile* > projectiles;
        int nrProjectiles = 0;

        //limite miscare tank pt a nu iesi din scena:
        //x si z nu trb sa fie +- 23 pozitiile, randez cand se afla in limite
        float superiorLimit = 23;
        float inferiorLimit = -23;

        int life = 3;
        float radiusFullBody = 2.0f;
        int score = 0;
        bool freeze = false;
    };



}   

