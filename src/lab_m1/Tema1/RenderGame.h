#pragma once

#include "lab_m1/Tema1/Game.h"

class RenderGame : public gfxc::SimpleScene {
public: 
    void RenderGameElements(float deltaTimeSeconds);
    void RenderRandomEnemy(float deltaTimeSeconds);
    void RenderLifeSquares();
    void RenderEverythingLeft(float deltaTimeSeconds);

    //doar atributele de modelare
    //mesh uile le am in joc
    float resolutionX;
    glm::mat3 modelMatrix;
    glm::mat3 modelLifeSquares = glm::mat3(1);
    int startRendEnemy = 0;
};