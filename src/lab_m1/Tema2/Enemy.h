
#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include <cmath>
#include <random>
#define Forward 1
#define Backward 2
#define RotationLeft 3
#define RotationRight 4

namespace implemented
{
    class Enemy
    {
    public:
        Enemy()
        {
        }

        ~Enemy()
        { }

        //1) pozitia initiala:
        void SetPosition(const glm::vec3 positionTank, const glm::vec3 center) {
            position = positionTank;
            //right,up,forward to do in fct de centru:
            forward = glm::normalize(center - position);
            initialForward = glm::normalize(center - position);
            this->center = center;
            //tureta:
            positionTurret = positionTank;
            forwardTurret = glm::normalize(center - position);
            //init matrice:
            UpdateMatrices();
        }
        
        //2)Alegere random miscare next: 
        int GetRandomNumberInRange(const int minInclusive, const int maxInclusive)
        {
            static std::random_device randomDevice;
            static std::default_random_engine randomEngine(randomDevice());

            std::uniform_int_distribution<int> uniformDist(minInclusive, maxInclusive);

            return uniformDist(randomEngine);
        }

        //2.2: Vf si daca ma alfu in limitele scenei cu miscarea aleasa - in update
        //Schimb miscarea dupa un anumit timp random
        void GetRandomMovement(float deltatime) {
            if (timeMovement == 0) {
                movement = GetRandomNumberInRange(1, 4);
                if ( movement == Forward || movement == Backward ) 
                    timeMovement = GetRandomNumberInRange(200, 850);
                else 
                    timeMovement = GetRandomNumberInRange(80, 250);
            }
            else {
                timeMovement--;
            }
            
            if ( movement == Forward )
                MoveForward(deltatime * speed);
            if (movement == Backward)
                MoveForward(-deltatime * speed);
           
            if (movement == RotationLeft) {
                angular += deltatime;
                RotateTank(angular);
            }
            if (movement == RotationRight) {
                angular -= deltatime;
                RotateTank(angular);
            }
        }


        //Miscare inainte:
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

        void RotateTank(float angle) {
            //setez:
            angleBody = angle;
            angleTurret = angle;
            //schimbare directie body
            glm::vec4 rotatedForward = glm::rotate(glm::mat4(1.0f), angleBody, glm::vec3(0, 1, 0)) * glm::vec4(initialForward, 0);
            forward = glm::normalize(glm::vec3(rotatedForward));
            //setare matrice:
            UpdateMatrices();
        }

        //3) Coliziune cu proiectil:
        void CollisionProjectile(Projectile* projectile) {
            //printf("ENEMY: %f, %f,  PROJ %f %f \n", this->position.x, this->position.z ,projectile->position.x, projectile->position.z);
            float Ax = this->position.z;
            float Ay = this->position.x;
            float Az = this->position.y;

            float Bx = projectile->position.z;
            float By = projectile->position.x;
            float Bz = projectile->position.y;

            float A_radius = this->radius;
            float B_radius = projectile->radius;
            float sumSquares = (Ax - Bx) * (Ax - Bx) +
                (Ay - By) * (Ay - By) +
                (Az - Bz) * (Az - Bz);
            float distance = sqrt(sumSquares);
            if (distance < A_radius + B_radius) {
                if (projectile->freeze == false) {
                    projectile->DezactivateProjectile();
                    life--;
                }
            }
           // printf(" dist = %f, sumRad = %f \n", distance, A_radius + B_radius);
        }

        //4) coliziune propriu zisa tank-tank:
        void CollisionEnemy(Enemy * otherEnemy, float deltaTime) {
            float Ax = this->position.z;
            float Ay = this->position.x;
            float Az = this->position.y;

            float Bx = otherEnemy->position.z;
            float By = otherEnemy->position.x;
            float Bz = otherEnemy->position.y;

            float A_radius = this->radius;
            float B_radius = otherEnemy->radius;
            float sumSquares = (Ax - Bx) * (Ax - Bx) +
                (Ay - By) * (Ay - By) +
                (Az - Bz) * (Az - Bz);
            float distance = sqrt(sumSquares);
            if (distance < A_radius + B_radius + 1 ) {
                glm::vec3 center1 = otherEnemy->center;
                glm::vec3 center2 = this->center;

                glm::vec3 dif = center2 - center1;
                float distMove = this->radius + otherEnemy->radius - distance;
                glm::vec3 P = distMove * glm::normalize(dif);
                center2 += P * 0.6f;
                center1 += P * -0.6f;

                //schimb directia tancurilor 
                this->forward = glm::normalize(center2 - this->position);
                otherEnemy->forward = glm::normalize(center1 - otherEnemy->position);
            }
        }

        //5) "aproape" colziiune cu player => shoot proiectile in el
        void CollisionPlayer(Tank* playerTank, CameraTank* camera, float deltaTime) {
            float Ax = this->position.z;
            float Ay = this->position.x;
            float Az = this->position.y;

            float Bx = playerTank->position.z;
            float By = playerTank->position.x;
            float Bz = playerTank->position.y;

            float A_radius = this->radiusFullBody;
            float B_radius = playerTank->radiusFullBody;
            float sumSquares = (Ax - Bx) * (Ax - Bx) +
                (Ay - By) * (Ay - By) +
                (Az - Bz) * (Az - Bz);
            float distance = sqrt(sumSquares);
            if (distance < A_radius + B_radius + 3) {
   
                ShootProjectile();
                nrProjectiles++;
            }
        }

        // ------ Functii proiectile/ tureta --------
        void ShootProjectile() {
            Projectile* projectile = new Projectile();
            projectiles.push_back(projectile);
            projectiles[nrProjectiles]->SetProjectile(position, forward, angleBody);
            projectiles[nrProjectiles]->ActivateProjectile();
            //start timer
        }

        void RotateTurret(float angle) {
            angleTurret = angle;

            //actualizare forward tureta
            glm::vec4 rotatedForwardTurret = glm::rotate(glm::mat4(1.0f), angleTurret, glm::vec3(0, 1, 0)) * glm::vec4(initialForward, 0);
            forwardTurret = glm::normalize(glm::vec3(rotatedForwardTurret));

            UpdateMatrices();
        }



        //Matrice: update doar daca ma aflu in limite
        //si daca mai am viata - altfel ma opresc 
        void UpdateMatrices() {
            if (life > 0 && freeze == false) {
                if (CheckPositionLimits() == IN_LIMITS) {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, position);
                    glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f);
                    modelMatrix = glm::rotate(modelMatrix, angleBody, rotationAxis);
                    matrixBody = modelMatrix;

                    glm::mat4 modelTurret = glm::mat4(1);
                    modelTurret = glm::translate(modelTurret, position);
                                                            //la alt unghi
                    modelTurret = glm::rotate(modelTurret, angleTurret, rotationAxis);
                    matrixTurret = modelTurret;
                }
                else {
                    //aleg alta miscare indiferent:
                    //in fata sau in spate in fct de unde m am blocat:
                    movement = GetRandomNumberInRange(1, 2);
                    timeMovement = GetRandomNumberInRange(200, 850);

                }
            }
        }

        //Miscarea aleatorie trb sa nu depaseasca limitele:
        int CheckPositionLimits() {
            if (position.x >= -23.0f && position.x <= 23.0f) {
                if (position.z >= -23.0f && position.z <= 23.0f)
                    return IN_LIMITS;
            }
            return OFF_LIMITS;
        }

        void Freeze() {
            freeze = true;
        }

    public:
       //matrici:
        glm::mat4 matrixBody;
        glm::mat4 matrixTurret;
        //implementare cu forward si normalizare:
        glm::vec3 initialForward;
        glm::vec3 forward;
        glm::vec3 position;
        glm::vec3 center;

        //rotiri:
        float angleBody = 0;

        //viteza enemy:
        float speed = 3.0f;
        int timeMovement = 0;
        int movement;
        float angular = 0;
        float rotationTime = 0;

        //sfera de aproximat:
        float radius = 1.5f;
        float radiusFullBody = 2.0f;
        //viata:
        int life = 3;

        bool freeze = false;
        //Proiectile:
        float angleTurret = 0;
        glm::vec3 forwardTurret;
        glm::vec3 positionTurret;
        std::vector<Projectile* > projectiles;
        int nrProjectiles = 0;
    };
   
}   

