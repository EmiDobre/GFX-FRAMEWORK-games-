#include "lab_m1/Tema1/RenderGame.h"

void::RenderGame::RenderGameElements(float deltaTimeSeconds)
{
    m1::Game game;
    RenderRandomEnemy(deltaTimeSeconds);
    //Render patrate viata daca mai am viata:
    if (game.lifeLost >= 3) {
        // Seteazã culoarea ecranului ca fiind neagrã
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else {
        RenderEverythingLeft(deltaTimeSeconds);
    }
}


void RenderGame::RenderRandomEnemy(float deltaTimeSeconds)
{
	m1::Game game;
    float distance = game.distance;
    float lat_square = game.lat_square;
    resolutionX = game.resolutionX;
    int elementsAdded = game.indexEnemyToRender.size();
    
   

    for (int i = startRendEnemy; i < elementsAdded; i++) {
        //actualizare fiecare viteza a Enemy ului
        float translateX = game.translateXEnemy[i];
        translateX += deltaTimeSeconds * 70;
        game.translateXEnemy[i] = translateX;
        int index = game.indexEnemyToRender[i];
        Mesh* hexSmall = game.hexesSmall[index];
        Mesh* hexBig = game.hexesBig[index];
        int col = game.colWhereToRender[i];
        int ox = window->GetResolution().x + lat_square / 2;
        int oy = lat_square * col + distance * col + lat_square / 2;
        modelMatrix = glm::mat3(1);
        modelMatrix *= object2D::Translate(ox, oy);
        modelMatrix *= object2D::Translate(-translateX, 0);

        RenderMesh2D(hexSmall, shaders["VertexColor"], modelMatrix);
        RenderMesh2D(hexBig, shaders["VertexColor"], modelMatrix);

        //daca am ajuns la dreptunghi tr[i]:
        //obiectele pana in primul care a ajuns la drpeuthi nu le mai randez
        if (translateX > -lat_square / 4 + resolutionX) {
            startRendEnemy = i;
            game.lifeLost = i + 1; //viata pierduta = al i-lea elem care a trecut de linie 
        }
    }

    //actualizare noua la vitezele transaltiile enmey urilor in joc:
   

}

void RenderGame::RenderLifeSquares() 
{
	m1::Game game;
    if (game.lifeLost > 0) {
        for (int i = 0; i < game.lifeLost; i++) {
            Mesh* lifeSquare = game.lifeSquares[2 - i];
            modelLifeSquares *= object2D::Scale(0, 0);
            RenderMesh2D(lifeSquare, shaders["VertexColor"], modelLifeSquares);
        }
        modelLifeSquares = glm::mat3(1);
        for (int i = game.lifeLost; i < 3; i++) {
            Mesh* lifeSquare = game.lifeSquares[2 - i];
            RenderMesh2D(lifeSquare, shaders["VertexColor"], modelLifeSquares);
        }
    }
    else {
        modelLifeSquares = glm::mat3(1);
        for (int i = 0; i < 3; i++) {
            Mesh* lifeSquare = game.lifeSquares[i];
            RenderMesh2D(lifeSquare, shaders["VertexColor"], modelLifeSquares);
        }
    }
}

void RenderGame::RenderEverythingLeft(float deltaTimeSeconds)
{
	m1::Game game;
    //life squares scaleaza la 0 cand am o viata lipsa
    // primele lifeLost patrate scaleaza la 0
    RenderLifeSquares();

    //Renderuri statice ( scena ):
    modelMatrix = glm::mat3(1);
    for (Mesh* square : game.squares) {
        RenderMesh2D(square, shaders["VertexColor"], modelMatrix);
    }
    for (Mesh* square : game.mini_squares) {
        RenderMesh2D(square, shaders["VertexColor"], modelMatrix);
    }
    for (Mesh* star : game.stars) {
        RenderMesh2D(star, shaders["VertexColor"], modelMatrix);
    }
    for (Mesh* romb : game.romburi) {
        RenderMesh2D(romb, shaders["VertexColor"], modelMatrix);
    }
    for (Mesh* square : game.chenar_middle_squares) {
        RenderMesh2D(square, shaders["VertexColor"], modelMatrix);
    }
    for (Mesh* square : game.chenar_squares) {
        RenderMesh2D(square, shaders["VertexColor"], modelMatrix);
    }
}