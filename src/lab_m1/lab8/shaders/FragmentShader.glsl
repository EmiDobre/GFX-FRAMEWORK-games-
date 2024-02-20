#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// lumina nr 2:
uniform vec3 light_direction2;
uniform vec3 light_position2;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;
uniform int type_of_light;
uniform float cut_off_angle;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Define ambient, diffuse and specular light components
   /// float ambient_light = 0.25;
  //  float diffuse_light = 0;
  //  float specular_light = 0;
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    //if (diffuse_light > 0)
   // {
  //  }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.

    // TODO(student): Write pixel out color
   // out_color = vec4(1);


   	vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 R = normalize(reflect(L, world_normal));

	//amientala
	float ambient_light = 0.25f;

	//difuza
	float diffuse_light = material_kd * max(dot(normalize(N), L), 0.f);

	//speculara
	float specular_light = 0.f;

	if (diffuse_light > 0.f)
	{
		specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);
	}

	// lumina
	float light = 0.f;

	if (type_of_light %2 == 1)
	{
		float cut_off_rad		= radians(cut_off_angle);
		float spot_light		= dot(L, light_direction);
		float spot_light_limit	= cos(cut_off_rad);
		
		if (spot_light > spot_light_limit)
		{	 
			// atenuare:
			float linear_att		= (spot_light - spot_light_limit) / (1.f - spot_light_limit);
			float light_att_factor	= linear_att * linear_att;
			light					= ambient_light + light_att_factor * (diffuse_light + specular_light);
		} else
		{
			light = ambient_light;  // nu am spot light ci doar lumina de la restul
		}
	} else
	{
		float d						= distance(light_position, world_position);
		float attenuation_factor	= 1.f / max(d * d, 1.f);
		light						= ambient_light + attenuation_factor * (diffuse_light + specular_light);
	}

	// Write pixel out color
	vec3 colour = object_color * light;


	///////////////////////////lumina nr 2:
	vec3 N2 = normalize(world_normal);
	vec3 L2 = normalize(light_position2 - world_position);
	vec3 V2 = normalize(eye_position - world_position);
	vec3 H2 = normalize(L2 + V2);
	vec3 R2 = normalize(reflect(L2, world_normal));

	//amientala
	ambient_light = 0.25f;

	//difuza
	float diffuse_light2 = material_kd * max(dot(normalize(N2), L2), 0.f);

	//speculara
	float specular_light2 = 0.f;

	if (diffuse_light2 > 0.f)
	{
		specular_light2 = material_ks * pow(max(dot(N2, H2), 0), material_shininess);
	}

	// lumina
	light = 0.f;
	float cut_off_rad2		= radians(cut_off_angle);
	float spot_light2		= dot(L2, light_direction2);
	float spot_light_limit2	= cos(cut_off_rad2);

	float d2						= distance(light_position2, world_position);
	float attenuation_factor2	= 1.f / max(d2 * d2, 1.f);
	light						= ambient_light + attenuation_factor2 * (diffuse_light2 + specular_light2);



	//MEREU CU PLUS SE RETURNEAZA O DA Aout_colour
	colour += object_color * light;

	out_color = vec4(colour, 1.f);
}
