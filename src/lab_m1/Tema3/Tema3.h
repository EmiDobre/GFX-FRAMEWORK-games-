#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/Tema3/Boat.h"

#define ANGLE_SPEEDUP		(20.f)

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void Tema3::InitPositions();

        std::unordered_map<std::string, Texture2D*> mapTextures;

        //obiecte: barcute
        std::vector<implemented::Boat*> boats;
        int nrBoats = 4;
        std::vector<glm::vec3> positions;

        //lumina:
        glm::vec3 lightPosition;
        glm::vec3 lightDirection;
        glm::vec3 boat_lightPosition;
        //directii lumini de la barci:
        std::vector<glm::vec3>  boats_lightDirection;
        glm::vec3 lighthouseDirection1;
        glm::vec3 lighthouseDirection2;
        float angleHouse = 0;
        glm::vec3 lightHousePosition;

        //material:
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        GLint typeOfLight = 1;
        GLfloat angleOX, angleOY;
        GLfloat cutoffAngle;

        //culori lumina:
        glm::vec3 red  = glm::vec3(1, 0, 0);
        glm::vec3 orange = glm::vec3(1, 0.384, 0);
        glm::vec3 purple = glm::vec3(0.596, 0, 1);
        glm::vec3 green = glm::vec3(0.039, 0.749, 0.212);
        glm::vec3 white = glm::vec3(1);
        glm::vec3 insideBoat = glm::vec3(0.941, 0.847, 0.475);
        glm::vec3 relfectorLight = white;
        glm::vec3 blue = glm::vec3(0.51, 0.835, 0.969);
        
    };
}
