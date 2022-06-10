#version 330 // OpenGL 3.3

// dans le fs, par contre, 'varying' devient 'in'
// de plus, gl_FragColor est deprecie du coup on peut nommer la valeur en sortie comme on veut
// a partir du moment ou elle est marque 'out'
//exemple, ici o_FragColor est une variable utilisateur qui remplace gl_FragColor

out vec4 o_FragColor;

in vec4 v_color;

uniform float u_Time;

void main(void) 
{
	float g = mod(sin(u_Time), 1.0);

	vec4 color = v_color;
	color.g = g;

	o_FragColor = color;

}