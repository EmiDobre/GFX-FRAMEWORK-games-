#version 330

// Input de la vertex shader
in vec2 texcoord;
in vec3 frag_world_position; //valuri
in float timeWave;

//2)Lumina:
in vec3 world_position;
in vec3 world_normal;

// 1) Textura sau culoare (reflector)
uniform sampler2D texture_1;
uniform vec3 object_color;
//2)Lumina luna directionala
uniform vec3 light_direction;
uniform vec3 light_position;
//3)Lumina punctiforma barcuta:
uniform vec3 boat_light_position;
//4)Lumina de la cele 4 barcute:
uniform vec3 boat1_light_direction;
uniform vec3 boat2_light_direction;
uniform vec3 boat3_light_direction;
uniform vec3 boat4_light_direction;
//5)Lumina spotlight 2 surse:
uniform vec3 lighthouse1_direction;
uniform vec3 lighthouse2_direction;
uniform vec3 lighthouse_position;


uniform vec3 eye_position;
//material:
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
//angle si tip:
uniform int type_of_light;
uniform float cut_off_angle;

//1)culori lumini: - luna- 
uniform vec3 moonColor;
//2)interior barci:
uniform vec3 insideBoatColor;
//3)barci:
uniform vec3 boatColor1;
uniform vec3 boatColor2;
uniform vec3 boatColor3;
uniform vec3 boatColor4;
//4)culori far: reflectorLight:
uniform vec3 reflectorColor;
// Output
layout(location = 0) out vec4 out_color;


vec3 calculateColouredLighting( vec3 light_position, vec3 light_colour, bool no_depth, bool emits_light) {
    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_position - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 R = normalize(reflect(L, N));

    vec3 ambient_light = vec3(0.015);
    vec3 diffuse_light = material_kd * max(dot(N, L), 0.0) * light_colour;
    vec3 specular_light = vec3(0.0);

    if (diffuse_light.r > 0.0) {
        specular_light = material_ks * pow(max(dot(N, H), 0.0), material_shininess) * light_colour;
    }

    //atenuare: nu depth 
    vec3 attenuation = vec3(1);
    if ( no_depth == true ) {
        float d = distance(light_position, world_position);
        float attenuation_factor = 1.f / max(d * d, 1.f);
        attenuation = vec3(attenuation_factor);
    }

    //lumina emisiva:
    vec3 emissive_light = vec3(0.0);
    if (emits_light) {
        emissive_light = light_colour * 0.55;
    }

    return ambient_light + attenuation * (diffuse_light + specular_light  + emissive_light);
}


vec3 calculateSpotLight(vec3 position, vec3 light_color, vec3 direction, float cut_off) {
    vec3 light;
    vec3 L = normalize(position - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 R = reflect(L, world_normal);

    float ambient_light = material_kd * 0.25;
    float diffuse_light = material_kd * max(dot(L, world_normal), 0);
    float specular_light = material_ks * pow(max(dot(R,V),0), material_shininess);
    float spot_light = dot(-L, direction);

    float spot_light_limit = cos(cut_off);
    float linear_att = (spot_light - spot_light_limit) / (1.0 - spot_light_limit);
    float light_att_factor = pow(linear_att, 2);
    float aten_fac = 1.0 / pow(length(direction - world_position), 2);

    if ( spot_light > cos(cut_off)) {
        light = light_color * (ambient_light + (light_att_factor + aten_fac) * (diffuse_light + specular_light));
    } else {
        light = light_color * ambient_light;
        light *= 0.1;
    }
    return light;
}




//type of light: 0 - lumina farului interioara + spotlight
//               1 - lumina luna interioara
//               2 - lumina interioara barcute
//               3 - lumina pe ocean: suma de lumini 

void main()
{
    //1) texturare:
   	vec4 color = texture2D(texture_1, texcoord);
	out_color = color;
    
	if (out_color.a < .5f) {
		discard;
	}
    // zone cu sau fara brightness: 
    bool brightZone = false;

    //1.1) fara textura: reflector
    if ( type_of_light == 10 ) {
        out_color = vec4(object_color, 1);
    }

	
    // 2) Lumini ambientala,difuza, speculara: - pt toate obiectele ce lumineaza
     vec3 light = vec3(0);
     vec3 boatLight = vec3(0);
     vec3 moonLight = vec3(0);
     vec3 lightHouseLight = vec3(0);
     
    //3.1)lumina pe luna
    if (type_of_light == 1) {                           //depth: nu; emite : da, spot: nu
        moonLight = calculateColouredLighting(light_position, moonColor, false, true);
        moonLight *= 1.75;
        light += moonLight;
    } 
    //3.2)barcute lumina interioara::
    if ( type_of_light == 2 ) {                                                    // nu modific depth lumina  //emit lumina
         boatLight = calculateColouredLighting(boat_light_position, insideBoatColor, false, true);
         //+ lumina de la luna:
         moonLight = calculateColouredLighting(light_direction, moonColor, false, false);
         //+lumina far 
         lightHouseLight = calculateSpotLight(lighthouse_position, reflectorColor, lighthouse1_direction, cut_off_angle);
         lightHouseLight += calculateSpotLight(lighthouse_position, reflectorColor, lighthouse2_direction, cut_off_angle);
         
         boatLight *= 1.5;
         moonLight *= 1.5;
         lightHouseLight *= 0.25;

         light += boatLight;
         light += moonLight;
         light += lightHouseLight;
    } 

    //3.3: lumina pe ocean 
    if ( type_of_light == 3 ) {
        //lumina lunii:
        moonLight = calculateColouredLighting(light_direction, moonColor, false, false);
        moonLight *= 1.5;
        light += moonLight;

        //lumina de la barcute:
       boatLight = calculateColouredLighting(boat1_light_direction, boatColor1, true, false);
       boatLight += calculateColouredLighting(boat2_light_direction, boatColor2, true, false);
       boatLight += calculateColouredLighting(boat3_light_direction, boatColor3, true, false);
       boatLight += calculateColouredLighting(boat4_light_direction, boatColor4, true, false);
       boatLight *= 7.5;
       light += boatLight;

        //lumina de la far: no depth change, nu emit, am spot light: cele 2 lumini
        lightHouseLight = calculateSpotLight(lighthouse_position, reflectorColor, lighthouse1_direction, cut_off_angle);
        lightHouseLight += calculateSpotLight(lighthouse_position, reflectorColor, lighthouse2_direction, cut_off_angle);
        lightHouseLight *= 0.35;
        light += lightHouseLight;
    }
    
    //3.4: lumina pe far: de la luna si atat:
    if ( type_of_light == 0 ) {
        moonLight = calculateColouredLighting(light_direction, moonColor, false, false);
        moonLight *= 1.5;
        light += moonLight;
    }

     // Culoare finala
     out_color += vec4(light,0);
    
    //este intuneric peste ocean lumina va arata textura adv a oceanului: fol fct mix si step
    if ( type_of_light != 10 ) { 
        if ( type_of_light == 3 ) {
         vec3 finalColor = mix(color.rgb * light,color.rgb, step(length(light), 0.01));
         finalColor *= 0.55;
         vec4 finalOutput = vec4(finalColor, color.a);
         out_color = finalOutput;
        } else 
            out_color *= 0.25;
    } 

}