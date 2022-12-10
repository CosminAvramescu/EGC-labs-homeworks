#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 Car;
uniform int setColor;

// Output
// TODO(student): Output values to fragment shader
out vec2 frag_coord;
out vec3 frag_normal;
out vec3 frag_color;
out vec3 frag_position;

void main()
{
	vec3 world_pos = (Model * vec4(v_position, 1)).xyz;
    // TODO(student): Send output to fragment shader
   	frag_position = v_position;
	frag_normal = v_normal;
	frag_coord = v_coord;

    // TODO(student): Compute gl_Position
	if(setColor==1){
		frag_color = vec3(0, 1, 0);
	}
	else{
		frag_color = v_color;
	}

	vec3 dif = Car - world_pos;
	world_pos.y = world_pos.y - dot(dif,dif) * 0.02;
	gl_Position = Projection * View * vec4(world_pos, 1);
}
