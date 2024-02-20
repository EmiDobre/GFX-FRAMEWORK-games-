#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

//Valuri: translatie dupa timp 
uniform float time;

// Output catre fragment shader:
out vec2 texcoord;
out vec3 frag_world_position; //pt efectul de valuri
out float timeWave;
//2)Lumina:
out vec3 world_position;
out vec3 world_normal;


void main()
{
    //1) textura
    texcoord = v_texture_coord;

    //2)valuri daca e cazul:
        if (time >= 0) {
		texcoord = vec2(texcoord.x - time / 35.f, texcoord.y);
        float translatedTexCoordX = texcoord.x ;
		float pixel_movement = sin(translatedTexCoordX * 1 ) * 5  ;
        //uncomment pt fara valuri
        translatedTexCoordX += pixel_movement;
        texcoord.x = translatedTexCoordX;    
	} 

    //2)pozitia
    world_position = (Model * vec4(v_position, 1)).xyz;
	world_normal = normalize( mat3(Model) * normalize(v_normal) );

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);

    //3)timp pt fragment shadeR:
    timeWave = time;
}
