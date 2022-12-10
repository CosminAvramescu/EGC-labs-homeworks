#version 330

// Input
// TODO(student): Get values from vertex shader
in vec2 frag_coord;
in vec3 frag_normal;
in vec3 frag_color;
in vec3 frag_position;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    //out_color = vec4(1);

    out_color = vec4(frag_color, 1);
}
