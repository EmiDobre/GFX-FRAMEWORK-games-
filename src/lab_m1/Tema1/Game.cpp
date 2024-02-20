#include "lab_m1/Tema1/Game.h"
#include <vector>
#include <iostream>

#define ORANGE 0 
#define BLUE 1
#define YELLOW 2
#define PURPLE 3

#define FREE 0
#define NOT_FREE 1

#define VISIBILE 1
#define NOT_VISIBLE 0

#define FIRST_FIVE 0
#define ADD 1
#define DELETE 2

using namespace std;
using namespace m1;


Game::Game()
{
}


Game::~Game()
{
}


//Pas1: desenare meshuri
void Game::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    glm::vec3 corner = glm::vec3(0, 0, 0);
   
    //Pas1.0: desenare scena statica
    DrawPlaneScene();

    //Pas1.1: inamicii -> 1) aparitie inamic + deplasare pe linie in update
    DrawEnemy();
    getRandomIndex(); 
    getRandomColumn(); 
    translateXEnemy.push_back(0);           
    int lower = 200, upper = 500;
    timePassed = std::rand() % (upper - lower + 1) + lower;

    //Pas1.1: arma-romb pentru fiecare culoare cate unul
    gunYellow = object2D::CreateRomb("gunYel", corner, lat_chenar / 5, 2 * lat_chenar / 5, yellow);
    gunOrange = object2D::CreateRomb("gunOrange", corner, lat_chenar / 5, 2 * lat_chenar / 5, orange);
    gunBlue = object2D::CreateRomb("gunBlue", corner, lat_chenar / 5, 2 * lat_chenar / 5, blue);
    gunPurple = object2D::CreateRomb("gunPurple", corner, lat_chenar / 5, 2 * lat_chenar / 5, purple);
    
    //Pas1.2: proiectile pt fiecare culoare
    projectileYellow = object2D::CreateStar("projYel", corner, 6, 0, yellow);
    projectileOrange = object2D::CreateStar("projOrange", corner, 6, 0, orange);
    projectileBlue = object2D::CreateStar("projBlue", corner, 6, 0, blue);
    projectilePurple = object2D::CreateStar("projPurple", corner, 6, 0, purple);

    //Pas1.3: desen stelute random de colectat
    star_rand = object2D::CreateStar("star", corner, dimRandom_stars, 0.1, magenta);
    //timpul dupa care apar random primele 3 stelele: 
    lower = 100; upper = 300;
    timeForStars = std::rand() % (upper - lower + 1) + lower;
}



//Pas1.0: scena
void Game::DrawPlaneScene()
{
    glm::vec3 corner = glm::vec3(0, 0, 0);
    corner.x = 0;
    corner.y = 0;

    //PATRATE
    for (int i = 0; i < 3; i++) {
        corner.x = lat_square * i + distance * i + dist_rectangle;
        for (int j = 0; j < 3; j++) {
            corner.y = lat_square * j + distance * j;
            Mesh* square = object2D::CreateSquare("square" + std::to_string(i * 3 + j + 1), corner, lat_square, glm::vec3(0.224, 0.671, 0.506), true);
            squares.push_back(square);

            cornerSquares.push_back(corner);
        }
    }

    //DREPTUNGHI:
    //reprezint dreptunghiul drept 8 patrate mai mici
    corner.x = 0;
    corner.y = 0;
    for (int i = 0; i < 8; i++) {
        corner.y = rectangle_l * i;
        Mesh* square = object2D::CreateSquare("mini_square" + std::to_string(i), corner, rectangle_l, glm::vec3(0.82, 0.055, 0.067), true);
        mini_squares.push_back(square);
    }

    //CHENARE: patrat negru si peste un patrat mai mic de culoarea ecranului
    float empty_space = rectangle_l + 35;
    corner.y = corner.y + rectangle_l + empty_space;
    //fiecare chenar va fi distantat pe ox fata de cel din fata lui
    for (int i = 0; i < 4; i++) {
        corner.x = lat_square * i + distance * i + dist_rectangle;
        Mesh* square = object2D::CreateSquare("chenar_square" + std::to_string(i), corner, lat_square, glm::vec3(0, 0, 0), true);
        chenar_squares.push_back(square);
        corner_Chenare.push_back(corner);
    }

    //ROMB + patratul middle acum patratul in care voi avea elementele
    //+ STELUTA sub fiecare chenar:
    margin = 1;
    corner.y = corner.y + margin;
    lat_chenar = lat_square - margin * 2;
    float dist_lifeSquares = distance + 2 * margin;
    float distcorner_chenar = dist_rectangle + margin;
    int nr_stars = 0;
    float star_len = 55;        
    for (int i = 0; i < 4; i++) {
        corner.x = lat_chenar * i + dist_lifeSquares * i + distcorner_chenar;
        Mesh* square = object2D::CreateSquare("chenar_middle_square", corner, lat_chenar, glm::vec4(0.231, 0.231, 0.224, 0.922), true);
        chenar_middle_squares.push_back(square);

        //pt fiecare chenar fac cate un romb + nr de stelute
        float cx = corner.x + lat_chenar / 2;
        float cy = corner.y + lat_chenar / 2;
        glm::vec3 corner_romb = glm::vec3(cx, cy, 0);
        glm::vec4 colour;
        if (i == 0) {
            colour = orange;
            nr_stars = 1;
        }
        if (i == 1) {
            colour = blue;
            nr_stars = 2;
        }
        if (i == 2) {
            colour = yellow;
            nr_stars = 2;
        }
        if (i == 3) {
            colour = purple;
            nr_stars = 3;
        }

        Mesh* romb = object2D::CreateRomb("romb", corner_romb, lat_chenar / 5, 2 * lat_chenar / 5, colour);
        romburi.push_back(romb);
        
        //desenez stelutele
        float cx_star = corner.x - star_len;
        float cy_star = corner.y - star_len;
        glm::vec3 corner_star = glm::vec3(cx_star, cy_star, 0);
        for (int j = 0; j < nr_stars; j++) {
            corner_star.x = cx_star + star_len * j;
            Mesh* star = object2D::CreateStar("star", corner_star, 5, 0, light_gray);
            price_stars.push_back(star);
        }
    }

    //PATRATELE ROSII + sub ele 5 stelute 
    float distUntilLife = corner.x + lat_square + 2 * dist_lifeSquares;
    corner.y = corner.y + lat_square / 4;
    for (int i = 0; i < 3; i++) {
        corner.x = distUntilLife + lat_square / 2 * i + dist_lifeSquares * i;
        Mesh* square = object2D::CreateSquare("lifesquare", corner, lat_square / 2, glm::vec3(0.82, 0.055, 0.067), true);
        lifeSquares.push_back(square);
    }

    //STELE TIP BANI: init vector de corner pt fiecare
    firstStarPadding.y = corner.y - lat_square / 2 + 15;
    firstStarPadding.x = distUntilLife - 3 * star_len / 4;
    firstStarPadding.z = 0;
    corner_MoneyStars.push_back(firstStarPadding);

    star = object2D::CreateStar("star", glm::vec3(0, 0, 0), 4, 0, light_gray);
    money_stars.push_back(star);
    UpdateNewMoneyStar(FIRST_FIVE);
}


//Pas1.1: creare cele 4 tipuri de inamici
void Game::DrawEnemy()
{
    float bigHex_len = 3 * lat_square / 5;
    float smallHex_len = 3 * lat_square / 8;
    glm::vec3 corner = glm::vec3(0, 0, 0);
    for (int i = 0; i < 4; i++) {
        Mesh* hexSmall = object2D::CreateHex("smallHex", corner, smallHex_len, middleColor);
        for (auto& vertex : hexSmall->vertices) {
            vertex.position.z = 0.1f; //setez ca small sa se coloreze peste big
        }
        Mesh* hexBig = object2D::CreateHex("bigHex", corner, bigHex_len, bigHexColours[i]);
        hexesSmall.push_back(hexSmall);
        hexesBig.push_back(hexBig);
    }
}



//Metoda pt a desena cele 3 stelute la pozitii random toate 3 deodata
void Game::getRandom3stars() {
    glm::ivec2 resolution = window->GetResolution();
    for (int i = 0; i < 3; i++) {
        int lower = star_len;
        int upper = resolution.x - star_len;
        int randomX = std::rand() % (upper - lower + 1) + lower;
        upper = resolution.y - star_len;
        int randomY = std::rand() % (upper - lower + 1) + lower;
        lower = 1;
        upper = 4;
        glm::vec3 corner = glm::vec3(randomX, randomY, 1);
        corner_RandomStars.push_back(corner);               
   }
}


void Game::getRandomIndex() {
    int lower = 0, upper = 3;
    int index = std::rand() % (upper - lower + 1) + lower;
    indexColourEnemy.push_back(index);
}


void Game::getRandomColumn() {
    int lower = 0, upper = 2;
    int col = std::rand() % (upper - lower + 1) + lower;
    lineWhereToRender.push_back(col);

    //COLIZIUNE: init damage ul fiecarui enemy cu 0
    countDamageEnemy.push_back(0);
    //pt calcul proiectilul care a facut coliziunea anterior
    projDamageEnemy_ant.push_back(-1);
    projDamageEnemy_curent.push_back(0); //curent e cel putin 0

    //PROIECTIL ARUNCAT 
    projectilesThrownToEnemy.push_back(0);

    //PROIECTIL ARUNCAT IN ENEMY: dupa un timp
    timeBetweenProjectile.push_back(0);

    //scalarea inamicilor 
    scalingEnemies.push_back(glm::vec2(1, 1));
    //timp scalare inamici ca sa nu randez degeaba animatia
    timeRescaleEnemies.push_back(100);

}


//Pas2: randare scena + elemente daca am viata
void Game::Update(float deltaTimeSeconds)
{

    //Render patrate viata daca mai am viata:
    if (lifeLost >= 3) {
        // setez screen negru
        if (waitUntilOver == 0) {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        else {
            RenderEverythingLeft(deltaTimeSeconds);
            waitUntilOver--;
        }
    }
    else {
        if (lifeLost < 3 || lifeLost == 3) {
            RenderRandomEnemy(deltaTimeSeconds);
            RenderEverythingLeft(deltaTimeSeconds);
        }
    }
}


void Game::FrameStart()
{
    glClearColor(0.231, 0.231, 0.224, 0.922);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    //setez unde desenez
    glViewport(0, 0, resolution.x, resolution.y);
}


void Game::FrameEnd()
{
}


void Game::RenderRandomEnemy(float deltaTimeSeconds)
{
    float resolutionX = window->GetResolution().x;
    int elementsAdded = indexColourEnemy.size();
    for (int i = startRendEnemy; i < elementsAdded; i++) {
        //actualizare fiecare viteza a Enemy ului
        float translateX = translateXEnemy[i];
        translateX += deltaTimeSeconds * 70;
        translateXEnemy[i] = translateX;
        int index = indexColourEnemy[i];
        Mesh* hexSmall = hexesSmall[index];
        Mesh* hexBig = hexesBig[index];
        int col = lineWhereToRender[i];
        int ox = window->GetResolution().x + lat_square / 2;
        int oy = lat_square * col + distance * col + lat_square / 2;
        modelMatrix = glm::mat3(1);
        modelMatrix *= object2D::Translate(ox, oy);
        modelMatrix *= object2D::Translate(-translateX, 0);

        //DACA DAMAGE ul acelui enemy = 3: rescale pt un timp
        if (countDamageEnemy[i] >= 3) {
            if (timeRescaleEnemies[i] != 0) {
                timeRescaleEnemies[i]--;
                scalingEnemies[i].x -= deltaTimeSeconds * 1;
                scalingEnemies[i].y -= deltaTimeSeconds * 1;
                modelMatrix *= object2D::Scale(scalingEnemies[i].x, scalingEnemies[i].y);
                RenderMesh2D(hexSmall, shaders["VertexColor"], modelMatrix);
                RenderMesh2D(hexBig, shaders["VertexColor"], modelMatrix);
            }
        }

        //randez in continuare
        if (countDamageEnemy[i] < 3) {
            RenderMesh2D(hexSmall, shaders["VertexColor"], modelMatrix);
            RenderMesh2D(hexBig, shaders["VertexColor"], modelMatrix);

            //obiectele care au trecut de dreptunghi nu le mai randez
            if (translateX > -lat_square / 4 + resolutionX) {
                startRendEnemy = i;
                if (startRender_ant != startRendEnemy)
                    lifeLost++;
                startRender_ant = startRendEnemy;
                
            }
        }
    }
}


void Game::RenderEverythingLeft(float deltaTimeSeconds)
{
    //PATARTE VIATA:
    RenderLifeSquares();

    //RANDOM 3 STELE  deodata, z = 0 : nu mai randez a fost colectata
    for (int i = 0; i < corner_RandomStars.size(); i++) {
        if (corner_RandomStars[i].z == 1) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= object2D::Translate(corner_RandomStars[i].x, corner_RandomStars[i].y);
            RenderMesh2D(star_rand, shaders["VertexColor"], modelMatrix);
        }
    }

    //STELE-BANI +- cand am budget > star.size(), sau <
    for (int i = 0; i < corner_MoneyStars.size(); i++) {
        float visibile = corner_MoneyStars[i].z;
        if (visibile == 1) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= object2D::Translate(corner_MoneyStars[i].x, corner_MoneyStars[i].y);
            RenderMesh2D(star, shaders["VertexColor"], modelMatrix);
        }
    }

    //ARMA-ROMB cumparate:
    //bought devine false cand ii dau drop 
    if (bought == true) {
        RenderBoughtGun();
    }
    if (dropped == true) {
        RenderDroppedGun(deltaTimeSeconds);
    }
    //PROIECTILE din arma
    RenderProjectiles(deltaTimeSeconds);

    //Renderuri statice ( scena ):
    modelMatrix = glm::mat3(1);
    for (Mesh* square : squares) {
        RenderMesh2D(square, shaders["VertexColor"], modelMatrix);
    }
    for (Mesh* square : mini_squares) {
        RenderMesh2D(square, shaders["VertexColor"], modelMatrix);
    }
    for (Mesh* romb : romburi) {
        RenderMesh2D(romb, shaders["VertexColor"], modelMatrix);
    }
    for (Mesh* star : price_stars) {
        RenderMesh2D(star, shaders["VertexColor"], modelMatrix);
    }
    for (Mesh* square : chenar_middle_squares) {
        RenderMesh2D(square, shaders["VertexColor"], modelMatrix);
    }
    for (Mesh* square : chenar_squares) {
        RenderMesh2D(square, shaders["VertexColor"], modelMatrix);
    }
}


void Game::RenderLifeSquares()
{
   int size = lifeSquares.size() - 1;
   if (lifeLost > 0) {
        for (int i = 0; i < lifeLost; i++) {
            Mesh* lifeSquare = lifeSquares[ size - i];
            modelLifeSquares *= object2D::Scale(0, 0);
            RenderMesh2D(lifeSquare, shaders["VertexColor"], modelLifeSquares);
        }
        modelLifeSquares = glm::mat3(1);
        //incarc viata ramasa
        for (int i = lifeLost; i < 3; i++) {
            Mesh* lifeSquare = lifeSquares[size - i];
            RenderMesh2D(lifeSquare, shaders["VertexColor"], modelLifeSquares);
        }
    }
    else {
        modelLifeSquares = glm::mat3(1);
        for (int i = 0; i < 3; i++) {
            Mesh* lifeSquare = lifeSquares[i];
            RenderMesh2D(lifeSquare, shaders["VertexColor"], modelLifeSquares);
        }
   }
}


void Game::RenderBoughtGun()
{
    if (mouseHold == true) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= object2D::Translate(mousePosition.x, mousePosition.y);
        if ( colourBoughtGun == BLUE )
            RenderMesh2D(gunBlue, shaders["VertexColor"], modelMatrix);
        if ( colourBoughtGun == YELLOW ) 
            RenderMesh2D(gunYellow, shaders["VertexColor"], modelMatrix);
        if (colourBoughtGun == ORANGE)
            RenderMesh2D(gunOrange, shaders["VertexColor"], modelMatrix);
        if (colourBoughtGun == PURPLE)
            RenderMesh2D(gunPurple, shaders["VertexColor"], modelMatrix);
    }

}

void Game::RenderDroppedGun(float deltaTimeSeconds)
{
    for (int i = 0; i < corner_RunningGuns.size(); i++) {
        int colour = coloursRunningGuns[i];
        float translateX = corner_RunningGuns[i].x + lat_square/2;
        float translateY = corner_RunningGuns[i].y + lat_square/2;
        float animation = corner_RunningGuns[i].z;
        if (animation == VISIBILE) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= object2D::Translate(translateX, translateY);
            if (colour == BLUE)
                RenderMesh2D(gunBlue, shaders["VertexColor"], modelMatrix);
            if (colour == YELLOW)
                RenderMesh2D(gunYellow, shaders["VertexColor"], modelMatrix);
            if (colour == ORANGE)
                RenderMesh2D(gunOrange, shaders["VertexColor"], modelMatrix);
            if (colour == PURPLE)
                RenderMesh2D(gunPurple, shaders["VertexColor"], modelMatrix);
        }
        else {
            //altfel rescale pana trece un anumit timp apoi nu mai randez deloc:
            if (timeRescaleGUNs[i] != 0) {
                timeRescaleGUNs[i]--;
                modelMatrix = glm::mat3(1);
                modelMatrix *= object2D::Translate(translateX, translateY);
                scalingGuns[i].x -= deltaTimeSeconds * 1;
                scalingGuns[i].y -= deltaTimeSeconds * 1;
                modelMatrix *= object2D::Scale(scalingGuns[i].x, scalingGuns[i].y);

                if (colour == BLUE)
                    RenderMesh2D(gunBlue, shaders["VertexColor"], modelMatrix);
                if (colour == YELLOW)
                    RenderMesh2D(gunYellow, shaders["VertexColor"], modelMatrix);
                if (colour == ORANGE)
                    RenderMesh2D(gunOrange, shaders["VertexColor"], modelMatrix);
                if (colour == PURPLE)
                    RenderMesh2D(gunPurple, shaders["VertexColor"], modelMatrix);
            }
        }
    }
}

void Game::RenderProjectiles(float deltaTimeSeconds)
{
    for (int i = 0; i < translateProjectiles.size(); i++) {
        float visible = translateProjectiles[i].z;
        if (visible != NOT_VISIBLE) {
            float translateX = translateProjectiles[i].x;
            translateX += deltaTimeSeconds * 300;
            translateProjectiles[i].x = translateX;
            float translateY = translateProjectiles[i].y;
            modelMatrix = glm::mat3(1);

            //translatie + "centrul stelei"
            modelMatrix *= object2D::Translate(translateX + 65, translateY + 35);

            //rotatie:
            angularStepProjectiles[i] += 0.1;
            modelMatrix *= object2D::Rotate( angularStepProjectiles[i]);

            //translate inapoi:
            modelMatrix *= object2D::Translate(-55, -55);

            int colour = indexColourProjectile[i];
            //render pe culoare 
            if (colour == BLUE)
                RenderMesh2D(projectileBlue, shaders["VertexColor"], modelMatrix);
            if (colour == YELLOW)
                RenderMesh2D(projectileYellow, shaders["VertexColor"], modelMatrix);
            if (colour == ORANGE)
                RenderMesh2D(projectileOrange, shaders["VertexColor"], modelMatrix);
            if (colour == PURPLE)
                RenderMesh2D(projectilePurple, shaders["VertexColor"], modelMatrix);

        }
    }
}


//Pas3: inainte de update: 
void Game::OnInputUpdate(float deltaTime, int mods)
{
    //ENEMY: aparitie dupa un timp
    if (timePassed == 0) {
        getRandomIndex();                 //nou element random
        getRandomColumn();               //pe o coloana random
        translateXEnemy.push_back(0);   //viteza lui pe Ox noua 
        int lower = 500, upper = 1000; //actulizare interval cu o val random
        timePassed = std::rand() % (upper - lower + 1) + lower;
    }
    else {
        timePassed--;
    }  

    //STELUTA: aparitie dupa un timp
    if (timeForStars == 0) {
        getRandom3stars();
        int lower = 800, upper = 1800; 
        timeForStars = std::rand() % (upper - lower + 1) + lower;
    }
    else {
        timeForStars--;
    }

    //ACTIVARE PROIECTIL cand am enemy pe linie
    CheckProjectileNeeded(deltaTime);

    //VERIFICARE COLIZIUNI
    CheckCollisionEnemy_Proj();
    CheckCollisionEnemyGun();
}

//Pas4: actualizare mouse
void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    //pozitii proiectil randat INAINTE de DROP
    mouseY = 720 - mouseY;
    mousePosition.x = mouseX;
    mousePosition.y = mouseY;
    mousePosition.z = 0;
    //arma romb dupa drop se randeaza in functie de mousePos
}

//Pas5: actiuni click stanga
void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    mouseY = 720 - mouseY;
    if (button == GLFW_MOUSE_BUTTON_2) {
        //click pe stea:
        CheckCollectStar(mouseX, mouseY);

        //click patrat cu proiectil in el
        CheckDeleteGun(mouseX, mouseY);

        //click pe cele 4 chenar:
        CheckBuyGun(mouseX, mouseY);
    }
}

//Pas6: proiectilul a fost cumparat si s-a dat dropp => modelare GUN
void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    //se da DROP cand se face mouse release
    mouseHold = false;
    mouseY = 720 - mouseY;

    if (bought == true) {
        //GUN: corner.z = 1 => il randez, cand e 0 nu il mai randez
        glm::vec3 corner = glm::vec3(0, 0, 1);
        for (int i = 0; i < cornerSquares.size(); i++) {
            corner.x = cornerSquares[i].x;
            corner.y = cornerSquares[i].y;
            if (mouseX >= corner.x && mouseX <= corner.x + lat_square) {
                if (mouseY >= corner.y && mouseY <= corner.y + lat_square) {
                    //pot sa ii dau drop doar daca am patratul liber
                    float availableSquare = cornerSquares[i].z;

                    if (availableSquare == FREE) {
                        dropped = true;
                        UpdateNewMoneyStar(DELETE);
                        //dandu- drop trb sa il marchez ca ocupat 
                        cornerSquares[i].z = NOT_FREE;

                        corner_RunningGuns.push_back(corner);
                        coloursRunningGuns.push_back(colourBoughtGun);
                        timeRescaleGUNs.push_back(100);

                        scalingGuns.push_back(glm::vec2(1, 1));
                    }
                }
            }
        }
    }

    //s-a dat drop => am cumparat => reset la fals 
    bought = false;
}


void Game::UpdateNewMoneyStar(int actionType)
{
    int nrStars = corner_MoneyStars.size();
    if (actionType == FIRST_FIVE) {
        for (int i = 0; i < 5; i++) {
            budget++;
            int nrStars = corner_MoneyStars.size();
            glm::vec3 newCorner;
            newCorner.z = 1;
            newCorner.y = corner_MoneyStars[nrStars - 1].y;
            newCorner.x = firstStarPadding.x + star_len / 3 * i;
            corner_MoneyStars.push_back(newCorner);
            counter++;
        }
    }
    //adaug de la ultimele stele vizibile care se afla de index = buget-1
    if (actionType == ADD) {
        budget++;
        glm::vec3 newCorner;
        newCorner.z = 1;
        newCorner.y = corner_MoneyStars[budget - 1].y;
        newCorner.x = firstStarPadding.x + star_len / 3 * counter;
        corner_MoneyStars.push_back(newCorner);
        counter++;
    }
    //am dat drop la gun => buget scade, stelele dispar 
    if (actionType == DELETE) {
        //pretul stelelor dropate = cumparate
        int price = 0;
        if (colourBoughtGun == ORANGE)
            price = 1;
        if (colourBoughtGun == BLUE)
            price = 2;
        if (colourBoughtGun == YELLOW)
            price = 2;
        if (colourBoughtGun == PURPLE)
            price = 3;
        budget = budget - price;
        //sterg ultimele price stele tip bani-> setez corner.z = 0
        //sterg ultimele price stele tip bani care nu au fost deja sterse
        int deletedStars = 0;
        int lastStar = nrStars - 1;
        while (deletedStars < price) {
            int alreadyDeleted = corner_MoneyStars[lastStar].z;
            if (alreadyDeleted == 1) {
                //nu a fost stearsa:
                corner_MoneyStars[lastStar].z = 0;
                deletedStars++;
                counter--;
            }
            lastStar--;
        }
    }
}


void Game::CheckCollectStar(int mouseX, int mouseY) {
    for (int i = 0; i < corner_RandomStars.size(); i++ ) {
        glm::vec3 corner = corner_RandomStars[i];
        float ox_starScreen = corner.x + 90;
        float oy_starScreen = corner.y + 55; //steaua nu e centrata pe corner fix din cauza meshului
        if (mouseX >= ox_starScreen && mouseX <= ox_starScreen + 35)
            if (mouseY >= oy_starScreen && mouseY <= oy_starScreen + 50) {
                //verific daca am stea vizibila 
                if (corner_RandomStars[i].z == VISIBILE) {
                    //Steaua o sterg: - nu o mai randez
                    corner_RandomStars[i].z = NOT_VISIBLE;
                    //banii cresc, adaug un corner nou pt mesh 
                    UpdateNewMoneyStar(ADD);
                }
            }
    }
}


void Game::CheckBuyGun(int mouseX, int mouseY)
{
    for (int i = 0; i < 4; i++) {
        float ox = corner_Chenare[i].x;
        float oy = corner_Chenare[i].y;
        if (mouseX >= ox && mouseX <= ox + lat_chenar)
            if (mouseY >= oy && mouseY <= oy + lat_chenar) {
                if (i == 0 && budget >= 1) {
                    bought = true;
                    mouseHold = true;
                    colourBoughtGun = ORANGE;
                }
                if (i == 1 && budget >= 2) {
                    bought = true;
                    mouseHold = true;
                    colourBoughtGun = BLUE;
                }
                if (i == 2 && budget >= 2) {
                    bought = true;
                    mouseHold = true;
                    colourBoughtGun = YELLOW;
                }
                if (i == 3 && budget >= 3) {
                    bought = true;
                    mouseHold = true;
                    colourBoughtGun = PURPLE;
                }
               
            }
    }
}

//Sterge gun + eliberare statut de ocupat pt patrat
void Game::CheckDeleteGun(int mouseX, int mouseY)
{    
    for (int nrSquare = 0; nrSquare < cornerSquares.size(); nrSquare++) {
        float occupiedSquare = cornerSquares[nrSquare].z;
        if (occupiedSquare == NOT_FREE) {
            float oxSquare = cornerSquares[nrSquare].x;
            float oySquare = cornerSquares[nrSquare].y;
            //daca s-a dat click in interiorul unui square ocupat pe poz unde e romb
            if (mouseX >= oxSquare && mouseX <= oxSquare + 3 * lat_square / 4) {
                if (mouseY >= oySquare && mouseY <= oySquare + 3 * lat_square / 4) {
                    //eliberez patratul 
                    cornerSquares[nrSquare].z = FREE;
                    //aflu ce romb se afla in patrat: pt a nu i mai da render
                    for (int i = 0; i < corner_RunningGuns.size(); i++) {
                        if (corner_RunningGuns[i].x == oxSquare) {
                            if (corner_RunningGuns[i].y == oySquare) {
                                //am gasit rombul: ii setez .z
                                corner_RunningGuns[i].z = NOT_VISIBLE;
                            }
                        }
                    }
                }
            }
        }
    }
}

//Modelare proiectile pentru fiecare enemy DE ACC culoare cu 
// arma VIZIBILA pe ACC linie care NU A TRECUT DE ARMA: arunc 3 stele
void Game::CheckProjectileNeeded(float deltaTimeSeconds)
{
    float padding = 62.5f;
    for (int i = startRendEnemy; i < indexColourEnemy.size(); i++) {
        int lineEnemy = lineWhereToRender[i];
        float oyEnemy = lat_square * lineEnemy + distance * lineEnemy + lat_square / 2;
        //daca enemy ul este inca randat: adica nu este mort
        if (countDamageEnemy[i] >= 3)
            continue;

        //fiecare arma arunc fix 3 proiectile in ENEMY ul cerut
        for (int nrGun = 0; nrGun < corner_RunningGuns.size(); nrGun++) {
            int visibileGun = corner_RunningGuns[nrGun].z;
            //din armele vizibile arunc proiectil
            if (visibileGun == VISIBILE) {
                float oxGun = corner_RunningGuns[nrGun].x;
                float oxEnemy = translateXEnemy[i];
                if (coloursRunningGuns[nrGun] == indexColourEnemy[i]) {
                    //arma de acc culoare cu enemy
                    float oyGun = corner_RunningGuns[nrGun].y + padding;
                    //pe acc linie:
                    if (oyGun == oyEnemy) {
                        int colourProjectile = coloursRunningGuns[nrGun];
                        glm::vec3 coordinatesProj;
                        coordinatesProj.y = oyGun - padding / 2;  //oy-ul se pastreaza, ox este cel al armei + padding 
                        coordinatesProj.x = oxGun + padding;     // plus padding ca sa iasa din mijloc
                        coordinatesProj.z = 2;                  //randez proiectilul, il pun peste celelalte

                        //timp
                        if (timeBetweenProjectile[i] == 0 && projectilesThrownToEnemy[i] < 3 ) {
                                timeBetweenProjectile[i] = 110;
                                translateProjectiles.push_back(coordinatesProj);
                                indexColourProjectile.push_back(colourProjectile);
                                angularStepProjectiles.push_back(10.0f); //init angular step fiecare proiectil

                                projectilesThrownToEnemy[i]++;                             
                        }
                        else {
                            timeBetweenProjectile[i]--;
                        }
                     
                    }
                }
            }
        }
    }
}

void Game::CheckCollisionEnemy_Proj()
{
    float padding = 62.5f;
    for (int i = startRendEnemy; i < indexColourEnemy.size(); i++) {
        int lineEnemy = lineWhereToRender[i];
        float oyEnemy = lat_square * lineEnemy + distance * lineEnemy + lat_square / 2;
        float oxEnemy = translateXEnemy[i];

        for (int nrProj = 0; nrProj < translateProjectiles.size(); nrProj++) {
            //daca am steluta in screen continui cu vf 
            float oxProj = translateProjectiles[nrProj].x;
            float oyProj = translateProjectiles[nrProj].y;
            //stea in cadru:
            if (oxProj < 1280) {
                //acc linie
                if (oyProj == oyEnemy - padding / 2) {
                    //acc culoare
                    if (indexColourProjectile[nrProj] == indexColourEnemy[i]) {
                        //Enemy ul este inca randat - nu a fost omorat
                        if (countDamageEnemy[i] < 3) {
                            //proiectilul vine din arma care inca exista::
                            if (translateProjectiles[nrProj].z != NOT_VISIBLE) {
                                
                                //distanta maxima dintre Enemy Proiectil pana la coliziune
                                float radiusEnemy = 3 * lat_square / 4;
                                float radiusProj = lat_square / 4;
                                float distanceBetween = 1280 - oxEnemy - oxProj;
                                if (distanceBetween < 0 && distanceBetween >= -(radiusProj + radiusEnemy)) {
                                    translateProjectiles[nrProj].z = NOT_VISIBLE;
                                    
                                    //actualizare damage in fct de ultimul proiectil aruncat
                                    projDamageEnemy_curent[i] = nrProj;
                                    if (projDamageEnemy_curent[i] != projDamageEnemy_ant[i]) {
                                        countDamageEnemy[i]++;
                                    }
                                    projDamageEnemy_ant[i] = projDamageEnemy_curent[i];

                                    std::cout << " damage enemy frame curent: " << countDamageEnemy[i] << endl;

                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


void Game::CheckCollisionEnemyGun()
{   
    float padding = 62.5f;
    for (int i = startRendEnemy; i < indexColourEnemy.size(); i++) {
        int lineEnemy = lineWhereToRender[i];
        float oyEnemy = lat_square * lineEnemy + distance * lineEnemy + lat_square / 2;
        float oxEnemy = translateXEnemy[i];
        //daca enemy ul este inca randat: adica nu este mort
        if (countDamageEnemy[i] >= 3)
            continue;

        for (int nrGun = 0; nrGun < corner_RunningGuns.size(); nrGun++) {
            int visibileGun = corner_RunningGuns[nrGun].z;
            if (visibileGun == VISIBILE) {
                float oxGun = corner_RunningGuns[nrGun].x;
                float oyGun = corner_RunningGuns[nrGun].y + padding;
                //pe acc linie:
                if (oyGun == oyEnemy) {
                    float radiusEnemy = 3 * lat_square / 8;
                    float radiusGun = lat_square / 4;
                    float distanceBetween = 1280 - oxEnemy - oxGun;
                    if (distanceBetween < 0 && distanceBetween >= -(radiusGun + radiusEnemy)) {
                        corner_RunningGuns[nrGun].z = NOT_VISIBLE;
                        
                        //caut patratul in care se afla si il setez ca liber:
                        for (int nrSquare = 0; nrSquare < cornerSquares.size(); nrSquare++) {
                            float occupiedSquare = cornerSquares[nrSquare].z;
                            if (occupiedSquare == NOT_FREE) {
                                float oxSquare = cornerSquares[nrSquare].x;
                                float oySquare = cornerSquares[nrSquare].y;
                                if (oxSquare == oxGun && oyGun == oySquare + padding) {
                                    cornerSquares[nrSquare].z = FREE;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
