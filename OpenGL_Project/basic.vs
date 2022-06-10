#version 330 // OpenGL 3.3

// depuis OpenGL 3, les inputs sont caracterises par 'in' et les outputs par 'out'
// 'attribute' devient 'in' dans le vs
// 'varying' devient 'out' dans le vs

in vec3 a_position;
in vec4 a_color;
in vec2 a_texcoord;

out vec4 v_color;
out vec3 v_position;
out vec2 v_texcoord;

uniform float u_Time;

void main(void) 
{	
	v_color = a_color;
	v_position = a_position;
	v_texcoord = a_texcoord;

	gl_Position = vec4(a_position, 1.0);
}