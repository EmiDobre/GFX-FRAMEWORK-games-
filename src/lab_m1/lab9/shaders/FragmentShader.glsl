#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms
uniform bool mix_textures;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
   // out_color = vec4(1);
   	vec4 colour1 = texture2D(texture_1, texcoord);
	vec4 colour2 = texture2D(texture_2, texcoord);

	//combinare de texturi daca e cazul:
	if (mix_textures == true) {           //factor interpolare: 0.5: text 1 conteaza 50%, t2 restul
		out_color = mix(colour1, colour2, 0.5f);
	} else {
		out_color = colour1;
	}

	//alpha discard pt < 0.5:
	if (out_color.a < .5f) {
		discard;
	}

}
