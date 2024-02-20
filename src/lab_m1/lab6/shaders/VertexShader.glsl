#version 330
////////////cum se primesc datele pe GPU de la CPU
// Input
// TODO(student): Get vertex attributes from each location
//aici am atributele vertexului din Create mesh la locatiile lor
//ATRIBUTLEE DE INTRARE pt fiecare vertex 

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec3 v_color;
//TIMPUL nu e atribut nu e trimis prin pipe!

// Uniform properties: nu-si schimba val la executie

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time; //e uniform!

// Output
// TODO(student): Output values to fragment shader
//declar cu out si in pentru a trimite la fragmentShade (acc nume!!)\
//se trimit pe pipe ul 0 ex: layout(location = 0)...

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;
out vec3 frag_color;


void main()
{
    // TODO(student): Send output to fragment shader, fiecare out = in de aici
    frag_position	= v_position;
    frag_normal		= v_normal;
    frag_texture	= v_texture;
    frag_color		= v_color;

    //Bonus: variere normala dupa o functie de timp ??

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position + vec3(sin(Time), cos(Time), 0 ), 1.0);
}
