#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include <format>


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* object2D::CreateRomb(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float l1,
    float l2,
    glm::vec4 color)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(-l1, 0,0), color), //vf 0 romb pe Ox la stanga de centru (ox,oy,oz)
        VertexFormat(corner + glm::vec3(0, l2, 0), color), //vf 1 sus 
        VertexFormat(corner + glm::vec3(3 * l1 / 4,          l2 / 4,   0), color), // 2 primul vf dreptunghi
        VertexFormat(corner + glm::vec3(3 * (l1 + l2) / 4 , l2 / 4,  0), color), //vf 3: 23 lungime
        VertexFormat(corner + glm::vec3(3 * (l1 + l2) / 4 , -l2 / 4, 0), color),//vf 4: 34 latime // 26
        VertexFormat(corner + glm::vec3(l1, 0, 0), color), //vf 5 Ox dreapta -> in cont el este si varful unui triunghi mai mic nu se vede
        VertexFormat(corner + glm::vec3(3 * l1 / 4,          -l2 / 4, 0), color), //vf 6, 64 lungime, 26 latime
        VertexFormat(corner + glm::vec3(0, -l2, 0), color) // 7 vf jos romb
        //la vf de la romb se adauga cele 4 varfuri de la dreptunghi => 8 vf

    };

    Mesh* romb = new Mesh(name);
    std::vector<unsigned int> indices;
 
     romb->SetDrawMode(GL_TRIANGLE_STRIP);
     indices = { 0,7,5, 6, 4, 5, 5, 4, 3, 2, 5, 3,0, 5, 1 };
    

    romb->InitFromData(vertices, indices);
    return romb;
}


Mesh* object2D::CreateHex(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float l,
    glm::vec4 color)
{
    glm::vec3 corner = leftBottomCorner;

    //am in plus coordonata Oz care e 1 pt hexagonul mic si 0 si cel mare
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(0, l/2,0), color), //vf  0 cel de sus
        VertexFormat(corner + glm::vec3(l/2, l/4,0), color), //vf 1: din triunghiul de sus "acoperis"
        VertexFormat(corner + glm::vec3(-l/2, l/4,0), color), //vf2 : colt triunghi 2 1 0 trig order
        VertexFormat(corner + glm::vec3(l/2, -l/4, 0), color), //vf 3: colt jos face parte din triunghiul 2 3 1
        VertexFormat(corner + glm::vec3(0, -l/2, 0), color), //vf 4 : vf diametral opus 1 : triunghi 5 4 3
        VertexFormat(corner + glm::vec3(-l/2, -l/4, 0), color), //vf 5: diametral opus vf 2, face part e din 2 5 3
    };

    Mesh* romb = new Mesh(name);
    std::vector<unsigned int> indices;


    // folosesc triunghiuri daca fac fill cu culoare:
    romb->SetDrawMode(GL_TRIANGLE_STRIP);
    indices = { 0, 2, 1, 2, 3, 1, 2, 5, 3, 5, 4, 3 };


    romb->InitFromData(vertices, indices);
    return romb;
}


//z va fi dif de 0 pt ca steaua se afiseaza peste orice pt stelele magenta
//pt celelalte stele gri nu
Mesh* object2D::CreateStar(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float prop,
    float z,
    glm::vec4 color)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(11 * prop, 12 * prop, z), color), // varf sus B : 
        VertexFormat(corner + glm::vec3(12 * prop, 9 * prop , z), color), //A  : 
        VertexFormat(corner + glm::vec3(16 * prop, 9 * prop, z), color),  //J : 
        VertexFormat(corner + glm::vec3(13 * prop, 6.5 * prop, z), color), //I : 
        VertexFormat(corner + glm::vec3(14 * prop, 3 * prop, z), color), //H : 
        VertexFormat(corner + glm::vec3(11 * prop, 5 * prop, z), color), //G: : conectare jos stea
        VertexFormat(corner + glm::vec3(8 * prop, 3 * prop, z), color), //F a doau jumatate: 6 colt jos stanga
        VertexFormat(corner + glm::vec3(9 * prop, 6.5 * prop, z), color), // E 
        VertexFormat(corner + glm::vec3(6 * prop, 9 * prop, z), color), //D colt stanga sus
        VertexFormat(corner + glm::vec3(10 * prop, 9 * prop, z), color), //C colt interior aproape de varf
    };

    Mesh* star = new Mesh(name);
    std::vector<unsigned int> indices;

    star->SetDrawMode(GL_TRIANGLE_STRIP);
    indices = {9, 1, 0, 9, 3, 1, 1 , 3, 2, 9, 5, 3, 5, 4, 3, 7, 6, 5, 7, 5, 9, 8, 7, 9 };

    star->InitFromData(vertices, indices);

    return star;
}

glm::vec3 object2D::findStarCenter(float prop)
{
    std::vector<glm::vec3> vertices =
    {
        glm::vec3(11 * prop, 12 * prop, 0),
        glm::vec3(12 * prop, 9 * prop, 0), 
        glm::vec3(16 * prop, 9 * prop, 0),   
        glm::vec3(13 * prop, 6.5 * prop, 0),  
        glm::vec3(14 * prop, 3 * prop, 0),
        glm::vec3(11 * prop, 5 * prop, 0),
        glm::vec3(8 * prop, 3 * prop, 0),
        glm::vec3(9 * prop, 6.5 * prop, 0),
        glm::vec3(6 * prop, 9 * prop, 0), 
        glm::vec3(10 * prop, 9 * prop, 0)
    };
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < vertices.size(); i++) {
        center.x += center.x + vertices[i].x;
        center.y += center.y + vertices[i].y;
    }
    center.x = center.x / vertices.size();
    center.y = center.y / vertices.size();
    return center;
}
