#version 330 // OpenGL 3.3

// depuis OpenGL 3, les inputs sont caracterises par 'in' et les outputs par 'out'
// 'attribute' devient 'in' dans le vs
// 'varying' devient 'out' dans le vs

in vec3 a_position;
in vec3 a_normal;	// pour l'illumination
in vec2 a_texcoords;
in vec4 a_color;

out vec3 v_normal;		// pour le Phong Shading ( interpole les normales )
out vec2 v_texcoords;
out vec4 v_color;

uniform float u_Time;

// definition d'un uniform Block
// le generateur va generer un indice de 'binding' pour ce Block
// note: en OpenGL 4, on peut forcer la valeur de l'indice avec layout(binding = 0)
uniform Matrices
{
	mat4 u_Projection;
	mat4 u_Translation;
	mat4 u_Rotation;
	mat4 u_Scale;
};

// position du fragment ( ou sommet ) dans la scene 3D
out vec3 v_fragmentPosition;

void main(void) 
{	
	v_color = a_color;
	v_texcoords = vec2(a_texcoords.s, 1.0-a_texcoords.t);

	vec2 offset = vec2(sin(u_Time), 0.0);
	
	
	// idealement a passer en uniform depuis le C++
	mat4 _WorldMatrix = (u_Translation * u_Rotation * u_Scale);

	// sous-matrice 3x3 a utiliser avec les normales
	// en theorie il faudrait plutot utilise la transposee de l'inverse (M^-1)^T 
	// 	mat3 NormalMatrix = transpose(inverse(mat3(_WorldMatrix))); // pas disponible dans chaque version de GLSL
	mat3 NormalMatrix = mat3(_WorldMatrix);

	v_normal = NormalMatrix * a_normal;

	vec4 worldPosition = _WorldMatrix * vec4(a_position, 1.0);

	v_fragmentPosition = vec3(worldPosition);

	// v' = Proj * Camera * ( T * R * S ) * v
	
	gl_Position = u_Projection * worldPosition;

	//gl_Position = vec4(a_position + offset, 0.0, 1.0);
}