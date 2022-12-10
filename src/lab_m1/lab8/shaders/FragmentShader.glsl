#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 lp;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;

uniform int isSpot;
uniform float angle;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
	vec3 L = normalize( light_position - world_position );
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );
	vec3 R = reflect( L, world_normal );

    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = material_kd * 0.25;
    float diffuse_light = material_kd * max(dot(L, world_normal), 0);
    float specular_light = material_ks * pow(max(dot(R, V), 0), material_shininess);

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
        
    }
    float cut_off = angle;
	float spot_light = dot(-L, light_direction);
	float spot_light_limit = cos(cut_off);

    float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
	float light_att_factor = pow(linear_att, 2);
	float aten_fac = 1 / pow(length(light_position - world_position), 2);
	vec3 light;

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    	if (isSpot == 0) {
        float d = distance(light_position , world_position);
        float factor_atenuare = 1 / (d*d + 1);
		light = object_color * (ambient_light + factor_atenuare * (diffuse_light + specular_light));
	} else {
		if (spot_light > cos(cut_off))
		{
			// fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
			// se calculeaza atenuarea luminii
			
			light = object_color * (ambient_light + (light_att_factor + aten_fac) * (diffuse_light + specular_light));
		} else {
			light = ambient_light * object_color;
		}
	}
    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.





    L = normalize( lp - world_position );
	V = normalize( eye_position - world_position );
	H = normalize( L + V );
	R = reflect( L, world_normal );

    // TODO(student): Define ambient, diffuse and specular light components
    ambient_light = material_kd * 0.25;
    diffuse_light = material_kd * max(dot(L, world_normal), 0);
    specular_light = material_ks * pow(max(dot(R, V), 0), material_shininess);

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
    }
    cut_off = angle;
	spot_light = dot(-L, light_direction);
	spot_light_limit = cos(cut_off); 

    linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
	light_att_factor = pow(linear_att, 2);
	aten_fac = 1 / pow(length(lp - world_position), 2);

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    if (isSpot == 0) {
		float d = distance(lp , world_position);
        float factor_atenuare = 1 / (d*d + 1);
        light += object_color * (ambient_light + factor_atenuare * (diffuse_light + specular_light));
    } else {
		if (spot_light > cos(cut_off))
		{
			// fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
			// se calculeaza atenuarea luminii
			
			light += object_color * (ambient_light + (light_att_factor + aten_fac) * (diffuse_light + specular_light));
		} else {
			light += ambient_light * object_color;
		}
	}
    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.



    // TODO(student): Write pixel out color
    out_color = vec4(light, 1);

}
