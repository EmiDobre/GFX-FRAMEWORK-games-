#include "lab_m1/Tema3/Tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    //0) camera:
    GetSceneCamera()->SetPosition(glm::vec3(0, 4, 10));
  
    //1)Texturi:
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures");

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "brick.png").c_str(), GL_REPEAT);
        mapTextures["far"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "moon.png").c_str(), GL_REPEAT);
        mapTextures["moon"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "ground.png").c_str(), GL_REPEAT);
        mapTextures["isle"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "white.png").c_str(), GL_REPEAT);
        mapTextures["reflector"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "red.jpg").c_str(), GL_REPEAT);
        mapTextures["roof"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "crate.jpg").c_str(), GL_REPEAT);
        mapTextures["wood"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "ocean3.png").c_str(), GL_REPEAT);
        mapTextures["ocean"] = texture;
    }
    //2)Meshuri
    {
        Mesh* mesh = new Mesh("cylinder");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives", "tema3"), "cylinder.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("boatBase");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives", "tema3"), "bazaBarca.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("support");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives", "tema3"), "sustinere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("flag");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives", "tema3"), "steag.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("surface");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives", "tema3"), "surface.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    //3)Shader:
    {
        Shader* shader = new Shader("Shader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    //4) Obiecte: barca 
    //razee:
    implemented::Boat* boat1 = new implemented::Boat();
    boat1->SetRadiusSpeed(-9.1, 0.635);
    boat1->SetColor(red);
    boats.push_back(boat1);

    implemented::Boat* boat2 = new implemented::Boat();
    boat2->SetRadiusSpeed(6.4, 0.46);
    boat2->SetColor(green);
    boats.push_back(boat2);

    implemented::Boat* boat3 = new implemented::Boat();
    boat3->SetRadiusSpeed(5.1, 0.60);
    boat3->SetColor(purple);
    boats.push_back(boat3);

    implemented::Boat* boat4 = new implemented::Boat();
    boat4->SetRadiusSpeed(-8.15, 0.5);
    boat4->SetColor(orange);
    boats.push_back(boat4);

    //5)Lumina:
    {
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
        cutoffAngle = 0.1f;

        //init directii:
        for (int i = 0; i < nrBoats; i++) {
            boats_lightDirection.push_back(glm::vec3(1));
        }

        //lumina far init:
        lighthouseDirection1 = glm::vec3(1);
        lighthouseDirection2 = glm::vec3(1);
    }
}

void Tema3::InitPositions() {
    positions.push_back(glm::vec3(-9, 0.5, -3));
    positions.push_back(glm::vec3(7, 0.5, 1.5));
    positions.push_back(glm::vec3(-5, 0.5, 5));
    positions.push_back(glm::vec3(-3, 0.5, -6.5));
}

void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0.133, 0.4, 0.941);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
    
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-18, 9, -18));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4));
        lightPosition = glm::vec3(-8, 2, -8);
        lightDirection = glm::vec3(-20, 2, -21);
        typeOfLight = 1;
        RenderSimpleMesh(meshes["sphere"], shaders["Shader"], modelMatrix, mapTextures["moon"]);
    }
    typeOfLight = 0;
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -3));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4,1,4));
        RenderSimpleMesh(meshes["sphere"], shaders["Shader"], modelMatrix, mapTextures["isle"]);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -3));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.2));
        RenderSimpleMesh(meshes["cylinder"], shaders["Shader"], modelMatrix, mapTextures["far"]);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.2, -3));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.2));
        RenderSimpleMesh(meshes["cylinder"], shaders["Shader"], modelMatrix, mapTextures["far"]);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 2.4, -3));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.2));
        RenderSimpleMesh(meshes["cylinder"], shaders["Shader"], modelMatrix, mapTextures["far"]);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 3, -3));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.9));
        typeOfLight = 10; //reflectorul nu are textura 
        RenderSimpleMesh(meshes["cylinder"], shaders["Shader"], modelMatrix, mapTextures["reflector"]);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 3.9, -3));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.9, 0.4, 0.9));
        typeOfLight = 0;
        RenderSimpleMesh(meshes["cylinder"], shaders["Shader"], modelMatrix, mapTextures["roof"]);
    }
    //barcute:
    {   
        for (int i = 0; i < nrBoats; i++) {
            typeOfLight = 2;
            boats[i]->UpdateMatrices(deltaTimeSeconds);
            //lumina din barca:
            boat_lightPosition = boats[i]->positionSupport;
            boat_lightPosition.x = boat_lightPosition.x + 50;
            //lumina de la barca:
            boats_lightDirection[i] = boats[i]->positionSupport;
            boats_lightDirection[i].y = 0.90;
            
            RenderSimpleMesh(meshes["boatBase"], shaders["Shader"], boats[i]->baseMatrix, mapTextures["wood"]);
            RenderSimpleMesh(meshes["support"], shaders["Shader"], boats[i]->supportMatrix, mapTextures["wood"]);
            RenderSimpleMesh(meshes["flag"], shaders["Shader"], boats[i]->supportMatrix, mapTextures["reflector"]);
        }
    }
    typeOfLight = 3;
    {  
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(25));
        //poz reflector + cele 2 directii
        lightHousePosition = glm::vec3(0, 2, -3);
        angleHouse += deltaTimeSeconds;
        lighthouseDirection1 = glm::vec3(sin(angleHouse), -0.25, cos(angleHouse) );
        lighthouseDirection2 = glm::vec3(sin(angleHouse + RADIANS(180.0f)), -0.25, cos(angleHouse + RADIANS(180.0f)));
        RenderSimpleMesh(meshes["surface"], shaders["Shader"], modelMatrix, mapTextures["ocean"]);
    }

}

void Tema3::FrameEnd()
{
    //DrawCoordinateSystem();
}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    //Textura bind: 
    if (texture1)
    {
        // - activate texture location 0
        glActiveTexture(GL_TEXTURE0);
        // - bind the texture1 ID
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        // - send theuniform value
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

   
    //Valuri: 
    GLint locTime = glGetUniformLocation(shader->program, "time");
    if (mesh == meshes["surface"]) {
        glUniform1f(locTime, (GLfloat)Engine::GetElapsedTime());
    }
    else {
        glUniform1f(locTime, -1.f);
    }

    //Lumina luna:
    int light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    //Lumina barci:
    int boat_light_position = glGetUniformLocation(shader->program, "boat_light_position");
    glUniform3f(boat_light_position, boat_lightPosition.x, boat_lightPosition.y, boat_lightPosition.z);

    //Lumina DE LA barci:
    int boat1_light_direction = glGetUniformLocation(shader->program, "boat1_light_direction");
    glUniform3f(boat1_light_direction, boats_lightDirection[0].x, boats_lightDirection[0].y, boats_lightDirection[0].z);
    int boat2_light_direction = glGetUniformLocation(shader->program, "boat2_light_direction");
    glUniform3f(boat2_light_direction, boats_lightDirection[1].x, boats_lightDirection[1].y, boats_lightDirection[1].z);
    int boat3_light_direction = glGetUniformLocation(shader->program, "boat3_light_direction");
    glUniform3f(boat3_light_direction, boats_lightDirection[2].x, boats_lightDirection[2].y, boats_lightDirection[2].z);
    int boat4_light_direction = glGetUniformLocation(shader->program, "boat4_light_direction");
    glUniform3f(boat4_light_direction, boats_lightDirection[3].x, boats_lightDirection[3].y, boats_lightDirection[3].z);

    //Lumini DE LA far: pozitie far + directii cele 2 lumini
    int lighthouse1_direction = glGetUniformLocation(shader->program, "lighthouse1_direction");
    glUniform3f(lighthouse1_direction, lighthouseDirection1.x, lighthouseDirection1.y, lighthouseDirection1.z);
    int lighthouse2_direction = glGetUniformLocation(shader->program, "lighthouse2_direction");
    glUniform3f(lighthouse2_direction, lighthouseDirection2.x, lighthouseDirection2.y, lighthouseDirection2.z);
    int lighthouse_position = glGetUniformLocation(shader->program, "lighthouse_position");
    glUniform3f(lighthouse_position, lightHousePosition.x, lightHousePosition.y, lightHousePosition.z);


    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    GLint type = glGetUniformLocation(shader->program, "type_of_light");
    glUniform1i(type, typeOfLight);

    //CULORI:
    int moonColorLocation = glGetUniformLocation(shader->program, "moonColor");
    int insideBoatColorLocation = glGetUniformLocation(shader->program, "insideBoatColor");
    int boatColor1Location = glGetUniformLocation(shader->program, "boatColor1");
    int boatColor2Location = glGetUniformLocation(shader->program, "boatColor2");
    int boatColor3Location = glGetUniformLocation(shader->program, "boatColor3");
    int boatColor4Location = glGetUniformLocation(shader->program, "boatColor4");
    int reflectorColorLocation = glGetUniformLocation(shader->program, "reflectorColor");

    glUniform3fv(moonColorLocation, 1, glm::value_ptr(white));
    glUniform3fv(insideBoatColorLocation, 1, glm::value_ptr(insideBoat));
    glUniform3fv(boatColor2Location, 1, glm::value_ptr(purple));
    glUniform3fv(boatColor1Location, 1, glm::value_ptr(red));  
    glUniform3fv(boatColor3Location, 1, glm::value_ptr(green));  
    glUniform3fv(boatColor4Location, 1, glm::value_ptr(orange));
    glUniform3fv(reflectorColorLocation, 1, glm::value_ptr(relfectorLight));
    
    //CULOARE reflector
    GLint locObject = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(locObject, 1, glm::value_ptr(relfectorLight));

    //angle reflector:
    GLint cut_off_angle = glGetUniformLocation(shader->program, "cut_off_angle");
    glUniform1f(cut_off_angle, cutoffAngle);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}




/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
     
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    }
    
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
