#version 330 // OpenGL 3.3

// depuis OpenGL 3, les inputs sont caracterises par 'in' et les outputs par 'out'
// 'attribute' devient 'in' dans le vs
// 'varying' devient 'out' dans le vs

in vec3 a_position;
in vec4 a_color;

out vec4 v_color;

uniform float u_Time;

void main(void) 
{	
	v_color = a_color;

	vec2 offset = vec2(sin(u_Time), 0.0);

	gl_Position = vec4(a_position, 1.0);
}