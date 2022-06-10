#version 330 // OpenGL 3.3

// dans le fs, par contre, 'varying' devient 'in'
// de plus, gl_FragColor est deprecie du coup on peut nommer la valeur en sortie comme on veut
// a partir du moment ou elle est marque 'out'
//exemple, ici o_FragColor est une variable utilisateur qui remplace gl_FragColor

out vec4 o_FragColor;

in vec4 v_color;
in vec2 v_texcoords;

uniform float u_Time;
uniform sampler2D u_sampler;
uniform sampler2D u_sampler1; //afficher la deuxième imahe avec texture

void main(void) 
{
	float g = mod(sin(u_Time), 1.0);

	vec4 texcolor = texture2D(u_sampler, v_texcoords);
	vec4 texcolor1 = texture2D(u_sampler1, v_texcoords);

	vec4 color = v_color;

	color.g = g;

	//gl_FragColor = texcolor;
	//gl_FragColor = color;

	// melange = blend
	// additif => +
	// modulation => *
	
	gl_FragColor = texcolor * texcolor1 * color;

}