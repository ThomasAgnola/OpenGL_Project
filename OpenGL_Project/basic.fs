#version 330 // OpenGL 3.3

// dans le fs, par contre, 'varying' devient 'in'
// de plus, gl_FragColor est deprecie du coup on peut nommer la valeur en sortie comme on veut
// a partir du moment ou elle est marque 'out'
//exemple, ici o_FragColor est une variable utilisateur qui remplace gl_FragColor

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular; // ajout d'un reflet de lumiere
	sampler2D diffuseTexture; // premiere texture
	sampler2D specularTexture; // Correspond a la deuxieme texture
};

out vec4 o_FragColor;

in vec4 v_color;
in vec3 v_position;
in vec3 v_normal;
in vec2 v_texcoords;

uniform Material material;

uniform float u_Time;

uniform vec3 lightPos;
uniform vec3 cameraPos;

void main(void) 
{
	vec4 color = v_color;

	vec4 texcolor = texture2D(material.diffuseTexture, v_texcoords);
	vec4 texcolor1 = texture2D(material.specularTexture, v_texcoords);

	// ambient light
	vec3 ambientLight = material.ambient;

	// diffuse light
	vec3 posToLightDirVec = normalize(v_position - lightPos);
	vec3 diffuseColor = vec3(1.f, 1.f, 1.f);
	float diffuse = clamp(dot(posToLightDirVec, v_normal), 0, 1);
	vec3 diffuseFinal = material.diffuse * diffuse;

	//specular light
	vec3 lightToPosDirVec = normalize(lightPos - v_position);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(v_normal)));
	vec3 posToViewDirVec = normalize(v_position - cameraPos);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 35);
	vec3 specularFinal = material.specular * specularConstant;

	// calulcate final shader
	gl_FragColor = 
	texcolor
	* texcolor1
	* (vec4(ambientLight, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));

}