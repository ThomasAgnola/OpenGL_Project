#version 330 // OpenGL 3.3

//#version 120 // OpenGL 2.1

// dans le fs, par contre, 'varying' devient 'in'
// de plus, gl_FragColor est deprecie du coup on peut nommer la valeur en sortie comme on veut
// a partir du moment ou elle est marque 'out'
//exemple, ici o_FragColor est une variable utilisateur qui remplace gl_FragColor

out vec4 o_FragColor;

in vec3 v_normal;		// pour le Phong Shading (interpole les normales)
in vec4 v_color;
in vec2 v_texcoords;

uniform sampler2D u_sampler;

uniform float u_Time;

// '-' car on prend la direction du fragment vers la lumiere (donc depuis la gauche)
// la lumière diffuse vers la droite (mais pour le fragment c'est donc depuis la gauche)
const vec3 _lightDirection = -normalize(vec3(1.0, 0.0, 0.0));
const vec3 _lightDiffuseColor = vec3(1.0, 0.0, 0.0);
const vec3 _lightSpecularColor = vec3(0.0, 1.0, 0.0);
const vec3 _lightAmbientColor = vec3(0.0, 0.5, 0.5);

const float _shininess = 256.0;

// ici encore, le vecteur part du fragment VERS l'observateur
const vec3 _cameraDirection = vec3(0.0, 0.0, -1.0);

// on recupere ici la position du fragment interpolee par le rasterizeur
in vec3 v_fragmentPosition;
const vec3 _cameraPosition = vec3(0.0, 0.0, 0.0);

uniform samplerCube u_SamplerEnvMap;

// illumination selon la loi de Lambert
vec3 diffuse(vec3 N, vec3 L)
{
	float Lambert = max(dot(N, L), 0.0);
	return pow(_lightDiffuseColor, vec3(2.2)) * Lambert;
}

// V = direction VERS l'observateur
vec3 Phong(vec3 N, vec3 V, vec3 L, float shininess)
{
	vec3 R = reflect(-L, N);
	float phong = pow(max(dot(R, V), 0.0), shininess);
	return pow(_lightSpecularColor, vec3(2.2)) * phong;
}

void main(void) 
{
	// on calcule le vecteur entre la position du fragment et la camera
	// puis on normalise pour obtenir une direction
	vec3 V = normalize(_cameraPosition - v_fragmentPosition);


	// probleme du Phong Shading sur GPU : l'interpolation est lineaire
	// donc cela perturbe la longueur de la normale
	vec3 N = normalize(v_normal);

	vec4 texcolor = texture(u_sampler, v_texcoords);

	vec3 indirectR = reflect(-V, N);
	vec4 envmapColor = texture(u_SamplerEnvMap, indirectR);
	
	// pour la partie RGB seulement ( l'alpha est toujours lineaire)
	// gamma(sRGB) -> lineaire : pow(x, 2.2)
	// lineaire -> gamma : pow(x, 1.0/2.2)
	// mais plus necessaire car OpenGL peut faire la convertion sRG -> RGB pour nous
	//texcolor.rgb = pow(texcolor.rgb, vec3(2.2));
	//envmapColor.rgb = pow(envmapColor.rgb, vec3(2.2));

	vec3 ambientColor = texcolor.rgb* pow(_lightAmbientColor, vec3(2.2));

	vec3 indirectColor =ambientColor + envmapColor.rgb;

	// represente l'influence directe d'une source lumineuse
	vec3 diffuseColor = texcolor.rgb * diffuse(N, _lightDirection);
	vec3 specularColor = Phong(N, _cameraDirection, _lightDirection, _shininess);
	vec3 directColor = diffuseColor + specularColor;

	// en sortie, on convertie de lineaire -> gamma car l'image dois s'afficher sur un moniteur LDR
	vec3 colorFinal = indirectColor + directColor;
	//colorFinal = pow(colorFinal, vec3(1.0/2.2));
	// OpenGL peut automatiquement convertir de lineaire -> gamma (cf. .cpp)

	o_FragColor = vec4(colorFinal, texcolor.a);
}