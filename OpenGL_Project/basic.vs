#version 330 // OpenGL 3.3

// depuis OpenGL 3, les inputs sont caracterises par 'in' et les outputs par 'out'
// 'attribute' devient 'in' dans le vs
// 'varying' devient 'out' dans le vs

in vec3 a_position;
in vec4 a_color;
in vec2 a_texcoords;

out vec4 v_color;
out vec3 v_position;
out vec2 v_texcoords;

uniform float u_Time;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void) 
{	
	v_color = a_color;
	v_position = vec4(ModelMatrix * vec4(a_position, 1.f)).xyz;
	v_texcoords = a_texcoords;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(a_position, 1.f);
}