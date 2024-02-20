#version 330

// Input
layout(location = 0) in vec3 v_position;



// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;
uniform int HP;

// Output pt fragment shader
out vec3 color;

void main()
{   
//culoare:
    float darknessFactor = 1.0 - float(HP) / 3.0; 
    float minDarknessThreshold = 0.2;
    darknessFactor = max(darknessFactor, minDarknessThreshold);
    vec3 darkenedColor = object_color * darknessFactor;
    color = darkenedColor;

//pozitie:
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}