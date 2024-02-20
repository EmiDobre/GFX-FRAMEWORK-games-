#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/object2D.h"


namespace m1
{
    class Game : public gfxc::SimpleScene
    {
    public:
        Game();
        ~Game();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
 

        //Desenare mesh uri:
        void DrawPlaneScene();
        void DrawEnemy();

        //Randomizare pt meshul enemy/star
        void getRandomIndex();
        void getRandomColumn();
        void getRandom3stars();

        //Randare elemente + animatii
        void RenderRandomEnemy(float deltaTimeSeconds);
        void RenderEverythingLeft(float deltaTimeSeconds);
        void RenderLifeSquares();
        void RenderBoughtGun();
        void RenderDroppedGun(float deltaTimeSeconds);
        void RenderProjectiles(float deltaTimeSeconds);

        //Animatie Steluta: 1) idenitifcare stea
        void CheckCollectStar(int mouseX, int mouseY);
        //Stele-bani: adaugara, disparitie, si primele 5
        void UpdateNewMoneyStar(int actionType);

        //Cumpar stele: daca am click pe chenare de sus
        void CheckBuyGun(int mouseX, int mouseY);
        //Animatie disparitie romb: daca rombul e visibil pe un patrat
        void CheckDeleteGun(int mouseX, int mouseY);

        //Coliziuni/Proiectile 
        void CheckProjectileNeeded(float deltaTimeSeconds);
        void CheckCollisionEnemy_Proj();
        void CheckCollisionEnemyGun();

        
    public:
        //variabile geometrie:
        float resolutionX;
        float lat_square = 125;
        float distance = 50; //dist intre patrate
        float rectangle_l = (lat_square * 3 + distance * 2) / 8; //latime dr = latura patrat mic
        float dist_rectangle = distance + rectangle_l; //distanta de la colt cu tot cu dreptunghi si distanta
        int star_len = 110;
        float margin = 1;
        float lat_chenar = lat_square - margin * 2;

        glm::vec3 color;
        glm::vec4 blue = glm::vec4(0.153, 0.314, 0.859, 0);
        glm::vec4 orange = glm::vec4(1, 0.471, 0, 0);
        glm::vec4 yellow = glm::vec4(1, 0.784, 0.204, 0.969);
        glm::vec4 purple = glm::vec4(0.761, 0.514, 0.961, 0.969);
        glm::vec4 middleColor = glm::vec4(0.153, 0.855, 0.859, 0);
        glm::vec4 light_gray = glm::vec4(0.404, 0.451, 0.451, 0.639);
        glm::vec4 magenta = glm::vec4(1, 0, 0.843, 0.8);
        glm::vec4 bigHexColours[4] = {orange, blue, yellow, purple};
      
        std::vector<Mesh*> squares;
        std::vector<Mesh*> mini_squares;
        std::vector<Mesh*> chenar_squares;
        std::vector<Mesh*> chenar_middle_squares;
        std::vector<Mesh*> romburi;
        std::vector<Mesh*> price_stars;
        std::vector<Mesh*> lifeSquares;
        std::vector<Mesh*> hexesBig, hexesSmall;
        std::vector<Mesh*> money_stars;
        Mesh* star_rand;
        Mesh* star;
        Mesh* gunYellow;
        Mesh* gunOrange;
        Mesh* gunBlue;
        Mesh* gunPurple;
        Mesh* projectileYellow;
        Mesh* projectileOrange;
        Mesh* projectileBlue;
        Mesh* projectilePurple;
       
        //vectori pentru creare treptata si random
        //de elemente
        std::vector<int> indexColourEnemy; //culorile enemy = index din vector culori
        std::vector<int> lineWhereToRender; //oy cu care se face translatia = ct
        float timePassed;
        
        //modelare ENEMY:
        glm::mat3 modelMatrix;
        std::vector<float> translateXEnemy; //translatia dinamica pe ox a fiecarui enemy
        glm::mat3 modelLifeSquares = glm::mat3(1);
        int startRendEnemy = 0;
        //modelare resclaare inamic mort:
        std::vector<glm::vec2> scalingEnemies; //fiecare enemy: incepe scalrare de la translatia lui
        std::vector <int> timeRescaleEnemies;

        //modelare stelute RANDOM
        std::vector<glm::vec3> corner_RandomStars;
        float timeForStars;
        float dimRandom_stars = 10;

        //modelare STELUTE BANI
        int budget = 0;
        glm::vec3 firstStarPadding;
        std::vector<glm::vec3> corner_MoneyStars;
        int counter = 0;

        //LOGICA ARMA = romb dar cu animatie 
        std::vector <glm::vec2> corner_Chenare;
        bool mouseHold = false;
        glm::vec3 mousePosition; //translatez unde e mouse ul
        int colourBoughtGun;
        //doar daca am cumparat intru in randare, dupa drop: devine false
        bool bought = false; 
        std::vector<glm::vec3> corner_RunningGuns;
        std::vector<int> coloursRunningGuns;
        bool dropped = false; //cu primul proiectlil dropped: devine true

        //modelare ARMA
        //randez doar proiectilele cu corner.z != 0, devin 0 cand dau click pe ele
        std::vector <glm::vec3> cornerSquares;
        std::vector <int> timeRescaleGUNs; 
        std::vector<glm::vec2> scalingGuns; 

        //modelare PROIECTILE 
        std::vector<glm::vec3> translateProjectiles;
        std::vector<int> indexColourProjectile; 
        std::vector<float> angularStepProjectiles;
        float angularStep = 10.0f;
       
        //PROIECITL SI INAMIC: counter care nr pt fiecare arma cate proj s-au tras: se initializeaza cu 0 odata ce se adauga noua arma
        std::vector<int> projectilesThrownToEnemy;// in fiecare enemy se trage cu 3 proiectile

        //DETECTIE COLIZIUNI:
        std::vector<float> countDamageEnemy;     //nr de stele cu care a facut coliziune
        std::vector<float> projDamageEnemy_ant; //ultimul proj care a dat damage unui enemy
        std::vector<float> projDamageEnemy_curent; //proj curent care a dat damage

        //proiectil si inamic: timp dintre proiectile pentru inamicul curent
        std::vector<int> timeBetweenProjectile;

        //VIATA RAMASA: vf care a fost ultimul dusman care a iesit din zona rosie
        int startRender_ant = -1;
        int lifeLost = 0;
        int waitUntilOver = 100;

     };
}   
