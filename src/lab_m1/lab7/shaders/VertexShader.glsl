#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties: aici e spatiul global
// deci obitn v pos in spatiul lume 
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;


void main()
{
    // TODO(student): Compute world space vectors: vec pos,1 pt translatie
    vec3 world_position = (Model * vec4(v_position, 1)).xyz;
    vec3 world_normal	= normalize(mat3(Model) * normalize(v_normal)); //normala nu vrem translatie 

    //directiile pt lumina:
    vec3 N = normalize(world_normal);
    vec3 V = normalize(eye_position - world_position);
	vec3 L = normalize(light_position - world_position); //poz luminii - poz obiect in lume 
	vec3 H = normalize(L + V); //mmediana
	vec3 R = normalize(reflect(L, world_normal));

    // TODO(student): Define ambient light component: las asa?
    float ambient_light = 0.25;

    // TODO(student): Compute diffuse light component
    float diffuse_light = 0;
    diffuse_light =  material_kd *  max(dot(N, L), 0);

    // TODO(student): Compute specular light component
    float specular_light = 0;
    //lumina speculara doar daca cea difuza se vede: daca primeste lumina
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }

    // TODO(student): Compute light: atenuare lumina
    float d						= distance(eye_position, world_position);  //obiectul privit prin ochi si lume
	float attenuation_factor	= 10 / (d * d);
	float light					= ambient_light + attenuation_factor * (diffuse_light + specular_light);
    

    // TODO(student): Send color light output to fragment shader
    color = vec3(1);
    color = light * object_color;

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
