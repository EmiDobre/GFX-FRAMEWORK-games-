#include "lab_m1/Tema2/WorldOfTanks.h"
#include <vector>
#include <string>
#include <iostream>

#include "utils/glm_utils.h"

#define IN_LIMITS 1
#define OFF_LIMITS 0

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


WOfTanks::WOfTanks()
{
}


WOfTanks::~WOfTanks()
{
}


void WOfTanks::Init()
{
    //1)Meshuri
    {
        Mesh* mesh = new Mesh("baza");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "bazaWOW.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("turela");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "turelaWOW.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("tun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "armaWOW.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("rail");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sineWOW.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("projectile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "proiectil.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cladire1");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cladire5.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    

    //2) Shadere
    {
        Shader* shader = new Shader("TankShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }


    //2)Camera:
    camera = new implemented::CameraTank();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    
    //3)Obiecte din scena:
    playerTank = new implemented::Tank();
    glm::vec3 positionTank = camera->GetTargetPosition();
    playerTank->StartTankPosition(positionTank,glm::vec3(0, 1, 0), 0);

    
    //4) Inamici: - start positions:
    InitEnemies();
    for (int i = 0; i < nrEnemies; i++) {
        implemented::Enemy* enemy = new implemented::Enemy();
        enemy->SetPosition(enemyPositions[i], enemyCenters[i]);
        enemies.push_back(enemy);
        enemyChecked.push_back(false);
    }

    //5) Cladiri: start pos:
    InitBuldings();
    for (int i = 0; i < nrBuildings; i++) {
        implemented::Building* building = new implemented::Building();
        building->SetBuilding(bpositions[i]);
        buildings.push_back(building);
    }

    //6) Matrice proiectie:
    top = 10.f;
    fov = 40.f;
    near = 0.01f;
    far = 200.0f;

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, near, far);

    //7) dimensiuni fereastra:
    glm::ivec2 resolution = window->GetResolution();
    glm::ivec2 centerWindow = glm::ivec2(resolution.x / 2, resolution.y / 2);
    
}

void WOfTanks::InitBuldings() {
    glm::vec3 position = camera->GetTargetPosition();
    glm::vec3 bpos;
    bpos.x = 30;
    bpos.y = 0;
    bpos.z = 1;
    bpositions.push_back(bpos);
    nrBuildings++;
}

void WOfTanks::InitEnemies() {
    glm::vec3 positionTank = camera->GetTargetPosition();
    glm::vec3 enemyPosition;
    glm::vec3 centerEnemy;
    //1:
    enemyPosition.x = -8.02176f;
    enemyPosition.y = positionTank.y;
    enemyPosition.z = positionTank.z;
    centerEnemy = glm::vec3(-8, 1, 0);
    enemyPositions.push_back(enemyPosition);
    enemyCenters.push_back(centerEnemy);
    nrEnemies++;

    //2:
    enemyPosition.x = 18.02176f;
    enemyPosition.y = positionTank.y;
    enemyPosition.z = positionTank.z;
    centerEnemy = glm::vec3(18, 1, 0);
    enemyPositions.push_back(enemyPosition);
    enemyCenters.push_back(centerEnemy);
    nrEnemies++;
    
    //3:
    enemyPosition.x = positionTank.x;
    enemyPosition.z = 15.02176f;
    enemyPosition.y = positionTank.y;
    centerEnemy = glm::vec3(0, 1, -15);
    enemyPositions.push_back(enemyPosition);
    enemyCenters.push_back(centerEnemy);
    nrEnemies++;
    
    //4:
    enemyPosition.x = positionTank.x;
    enemyPosition.z = -18.5f;
    enemyPosition.y = positionTank.y;
    centerEnemy = glm::vec3(0, 1, -18);
    enemyPositions.push_back(enemyPosition);
    enemyCenters.push_back(centerEnemy);
    nrEnemies++;
    
    
}

void WOfTanks::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void WOfTanks::Update(float deltaTimeSeconds)
{   
    //timpul 
    timeGame = glfwGetTime();
    if (timeGame >= 60 || playerTank->life == 0 ) {
        playerTank->Freeze();
        for (int i = 0; i < nrProj; i++)
            playerTank->projectiles[i]->Freeze();
        for (int i = 0; i < nrEnemies; i++) {
            enemies[i]->Freeze();
        }
        printf("GAME OVER\n");
        printf("SCOR: %d\n", playerTank->score);
        window->Close();
    }
    
    //player:
     RenderSimpleMesh(meshes["rail"], shaders["TankShader"], playerTank->rails, gray, 3 - playerTank->life);
     RenderSimpleMesh(meshes["baza"], shaders["TankShader"], playerTank->body, darkPlayer,3 - playerTank->life);
     RenderSimpleMesh(meshes["turela"], shaders["TankShader"], playerTank->turret, lightPlayer, 3 - playerTank->life);
     RenderSimpleMesh(meshes["tun"], shaders["TankShader"], playerTank->gun, gray, 3 - playerTank->life);
     

     //inamici:
     for (int i = 0; i < nrEnemies; i++) {

         if ( enemies[i]->life > 0 )
            enemies[i]->GetRandomMovement(deltaTimeSeconds);

         //vf coliziune inamic cu fiecare proiectil activ
         //proiectile:
         for (int j = 0; j < nrProj; j++) {
             if (playerTank->projectiles[j]->active == true) {
                 enemies[i]->CollisionProjectile(playerTank->projectiles[j]);

                 if (enemies[i]->life == 0) {
                     if (enemyChecked[i] == false) {
                         playerTank->score++;
                         enemyChecked[i] = true;
                     }
                 }

                 glm::mat4 projMatrix = playerTank->projectiles[j]->matrix;
                 RenderSimpleMesh(meshes["projectile"], shaders["TankShader"], projMatrix, black, 0);
             }
         }

         //verificare coliziuni tank-tank 
         for (int contor = 0; contor < nrEnemies; contor++) {
             if (contor == i && enemies[i]->life > 0 ) {
                 enemies[i]->CollisionPlayer(playerTank, camera, deltaTimeSeconds);
                 
             }
             else {
                 enemies[i]->CollisionEnemy(enemies[contor], deltaTimeSeconds);
             }
         }

         //render inamici:
         RenderSimpleMesh(meshes["rail"], shaders["TankShader"], enemies[i]->matrixBody, gray, 3 - enemies[i]->life);
         RenderSimpleMesh(meshes["baza"], shaders["TankShader"], enemies[i]->matrixBody, darkEnemy,3 - enemies[i]->life);
         RenderSimpleMesh(meshes["turela"], shaders["TankShader"], enemies[i]->matrixTurret, lightEnemy, 3 - enemies[i]->life);
         RenderSimpleMesh(meshes["tun"], shaders["TankShader"], enemies[i]->matrixTurret, gray,3 - enemies[i]->life);
     }

     //cladiri:
     for (int i = 0; i < nrBuildings; i++) {
         glm::mat4 bmatrix = buildings[i]->matrix;
         RenderSimpleMesh(meshes["cladire1"], shaders["TankShader"], bmatrix, gray, buildings[i]->damage);
     }
     
}

void WOfTanks::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
                                        const glm::vec3& color, int life)
{
        if (!mesh || !shader || !shader->program)
            return;

        //matrice:
        glUseProgram(shader->program);
        GLint viewLocation = glGetUniformLocation(shader->GetProgramID(), "View");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        GLint projLocation = glGetUniformLocation(shader->GetProgramID(), "Projection");
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        GLint modelLocation = glGetUniformLocation(shader->GetProgramID(), "Model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        //culoare:
        GLint locObject = glGetUniformLocation(shader->program, "object_color");
        glUniform3fv(locObject, 1, glm::value_ptr(color));

        //hp:
        GLint locHp = glGetUniformLocation(shader->program, "HP");
        glUniform1i(locHp, life);

        //render:
        mesh->Render();

}



void WOfTanks::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}



/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void WOfTanks::OnInputUpdate(float deltaTime, int mods)
{
    //mutare tank <=> mutare camera in acc directie identic :
    if (window->KeyHold(GLFW_KEY_W)) {
        playerTank->MoveForward( - deltaTime * speed);
        camera->MoveForward(  deltaTime * cameraSpeed);
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        playerTank->MoveForward(  deltaTime * speed);
        camera->MoveForward(- deltaTime * cameraSpeed);
    }
    //rotatie la stanga/ dreapta A si D in acc schimb ar schimba poz tank in fata camerei
    if (window->KeyHold(GLFW_KEY_A) && pressedD == 0) {
        angular += deltaTime ;
        angularCamera += deltaTime  ;
        camera->RotateAroundTank(angularCamera);       
        playerTank->RotateTank(angular, angularTurret + angular);
        pressedA = 1; 
    }
    if (window->KeyHold(GLFW_KEY_D) && pressedA == 0) {
        angular -= deltaTime ;
        angularCamera -= deltaTime ;
        camera->RotateAroundTank(angularCamera);
        playerTank->RotateTank(angular, angularTurret + angular);
        pressedD = 1;
    }
    angularCamera = 0;
    pressedA = 0;
    pressedD = 0;

    //proiectile active se misca: 
    for (int i = 0; i < nrProj; i++) {
        if (playerTank->projectiles[i]->active && timerProjectile[i] != 0) {
            playerTank->projectiles[i]->MoveForward(deltaTime * speed);
            timerProjectile[i]--;
        }
        //oprire randare
        if (timerProjectile[i] == 0) {
            playerTank->projectiles[i]->DezactivateProjectile();
            lastProjIndex = i;
        }
    }
    
}



void WOfTanks::OnKeyPress(int key, int mods)
{

}


void WOfTanks::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void WOfTanks::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
   
    angularTurret += float (deltaX) / 55 ;
    playerTank->RotateTurret( angularTurret + angular);
}


void WOfTanks::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_2) {
        //pot trage daca a trecut 1 sec de la ultima tragere:
        double diff = currentTime - last_currentTime;
        if (diff >= 1 || currentTime == 0) {
            //trag cu proiectilul si il setez inactiv dupa un timp:
            playerTank->ShootProjectile(nrProj);
            playerTank->projectiles[nrProj]->ActivateProjectile();
            timerProjectile.push_back(400);
            nrProj++;
            last_currentTime = currentTime;
        }
       
        currentTime = glfwGetTime();
    }
}


void WOfTanks::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void WOfTanks::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void WOfTanks::OnWindowResize(int width, int height)
{
}
