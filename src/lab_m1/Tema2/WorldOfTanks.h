#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"
#include "lab_m1/Tema2/Tank.h"
#include "lab_m1/Tema2/Enemy.h"
#include "lab_m1/Tema2/Building.h"
#include <chrono>



namespace m1
{
    class WOfTanks : public gfxc::SimpleScene
    {
     public:
         WOfTanks();
        ~WOfTanks();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;


        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        //functii noi:
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int life);
        void InitEnemies();
        void InitBuldings();

     protected:
        implemented::CameraTank *camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        GLfloat right;
        GLfloat left;
        GLfloat bottom;
        GLfloat top;
        GLfloat fov;
        GLfloat near, far;
        bool projectionType;

        //tank:
        implemented::Tank *playerTank;
        glm::vec3 gray = glm::vec3(0.82, 0.851, 0.835);
        glm::vec3 darkPlayer = glm::vec3(0.106, 0.227, 0.71);
        glm::vec3 lightPlayer = glm::vec3(0.243, 0.545, 0.831);
        glm::vec3 black = glm::vec3(0.949, 0.035, 0.035);

        //viteze:
        float speed = 3.0f;
        float cameraSpeed = 3.0f;
        float angular = 0;
        float angularCamera = 0;

        float angularTurret = 0;
        int lastMouseOy = 0;
        int lastMouseOx = 0;
        int firstMovement = 0;

        //pentru a nu schimba pov tank
        int pressedA = 0;
        int pressedD = 0;

        //proiectil:
        std::vector<int> timerProjectile;
        int nrProj = 0;
        int lastProjIndex = -1;
        //timpi reali in joc:
        double currentTime = 0;
        double last_currentTime = 0;

        //inamici
        std::vector<implemented::Enemy* > enemies;
        std::vector<glm::vec3> enemyPositions;
        std::vector<glm::vec3> enemyCenters;
        int nrEnemies = 0;
        glm::vec3 darkEnemy = glm::vec3(0.486, 0.027, 0.678);
        glm::vec3 lightEnemy = glm::vec3(0.91, 0.522, 0.878);


        //cladiri:
        std::vector<implemented::Building*> buildings;
        int nrBuildings = 0;
        std::vector<glm::vec3> bpositions;

        //stop joc:
        bool stopGame = false;
        std::vector<bool> enemyChecked;
        double timeGame = 0;

    };
}   // namespace m1
