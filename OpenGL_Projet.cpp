#define GLEW_STATIC // necessite glew32s.lib
#define STB_IMAGE_IMPLEMENTATION
#define _USE_MATH_DEFINES
#define TINYOBJLOADER_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <GL/wglew.h>
#include "stb_image.h"
#include "../common/GLShader.h"
#include "DragonData.h"
#include "tiny_obj_loader.h"

GLShader g_BasicShader;

GLuint VBO;
GLuint IBO;
GLuint VAO;
GLuint FBO;
GLuint texID;
GLuint cubemap_texture;

GLuint UBO; // Uniform Buffer Object, un object stockant des uniforms et partageable
GLuint Deer;

std::string inputfile = "Deer.obj";
tinyobj::ObjReaderConfig reader_config = tinyobj::ObjReaderConfig(); // Path to material files

tinyobj::ObjReader reader;

struct Color
{
    uint8_t r, g, b, a; // equivalent de unsigned char
};

// Vertex du Dragon
struct Vertex
{
    float position[3];
    float normal[3];
    float UV[2];
};

void Initialize()
{
    GLenum error = glewInit();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Erreur d'initialisation de GLEW\n";
    }
    g_BasicShader.LoadVertexShader("basic.vs");
    g_BasicShader.LoadFragmentShader("basic.fs");
    g_BasicShader.Create();
#ifdef WIN32
    wglSwapIntervalEXT(1);
#endif // WIN32

    

    //const int VertexCount = _countof(DragonVertices)/sizeof(Vertex);
    const int VertexBufferSize = sizeof(DragonVertices);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, 
        DragonVertices, GL_STATIC_DRAW);

    const int IndexCount = _countof(DragonIndices);
    const int IndexBufferSize = sizeof(DragonIndices);


    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * IndexCount, 
        DragonIndices, GL_STATIC_DRAW);


    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO); // en lecture et écriture


    glBindFramebuffer(GL_FRAMEBUFFER, 0); // rétabli le FBO par défaut, créé par l’API de fenêtrage

    // --- attributs

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    auto program = g_BasicShader.GetProgram();

    const uint32_t stride = sizeof(Vertex);

    // declare dans le VS en "attribute vec3 a_Position"
    int location = 0; //glGetAttribLocation(program, "a_position");
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location,
        3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, position));

    int loc_normal = 1; //glGetAttribLocation(program, "a_normal");
    glEnableVertexAttribArray(loc_normal);
    glVertexAttribPointer(loc_normal,
        3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, normal));

    int loc_texcoords = 2; //glGetAttribLocation(program, "a_texcoords");
    glEnableVertexAttribArray(loc_texcoords);
    glVertexAttribPointer(loc_texcoords,
        2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, UV));




    //reinit tout a commencer en premier par le VAO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // textures ---
    bool forceSRGB = true;

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    // Filtrage trilinéaire en minification et bilineaire en magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int w, h;
    uint8_t* data = stbi_load("dragon.png", &w, &h, nullptr, STBI_rgb_alpha);

    if (data) {
        GLenum internalFormat = forceSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }

    // GL_TEXTURE_CUBE_MAP_POSITIVE_X + i = croissant : +x, -x, +y, -y, +z, -z
    uint8_t* cubemap_image[6] = {
        stbi_load("testpx.png", &w, &h, nullptr, STBI_rgb_alpha),
        stbi_load("testnx.png", &w, &h, nullptr, STBI_rgb_alpha),
        stbi_load("testpy.png", &w, &h, nullptr, STBI_rgb_alpha),
        stbi_load("testny.png", &w, &h, nullptr, STBI_rgb_alpha),
        stbi_load("testpz.png", &w, &h, nullptr, STBI_rgb_alpha),
        stbi_load("testnz.png", &w, &h, nullptr, STBI_rgb_alpha)
    };

    const char* envmaps[] = {
        //"envmaps/test_px.png", 
        //"envmaps/test_nx.png", 
        //"envmaps/test_py.png", 
        //"envmaps/test_ny.png", 
        //"envmaps/test_pz.png", 
        //"envmaps/test_nz.png"
        "envmaps/pisa_posx.jpg",
        "envmaps/pisa_negx.jpg",
        "envmaps/pisa_posy.jpg",
        "envmaps/pisa_negy.jpg",
        "envmaps/pisa_posz.jpg",
        "envmaps/pisa_negz.jpg"
    };

    glGenTextures(1, &cubemap_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    for (int i = 0; i < 6; i++)
    {
        int w, h;
        uint8_t* data = stbi_load(envmaps[i], &w, &h, nullptr, STBI_rgb_alpha);
        if (data) {
            GLenum internalFormat = forceSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                0, internalFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
    }

    // on va stocker 4 mat4 dans l'UBO
    const int UBOsize = 4 * (sizeof(float) * 4 * 4);
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, UBOsize, nullptr, GL_STREAM_DRAW);

    int blockIndex = glGetUniformBlockIndex(program, "Matrices");
    glUniformBlockBinding(program, blockIndex, 0 /*meme valeur que glBindBufferBase*/);
    // le second parametre est un indice de 'binding' qui permet de faire le lien
    // entre l'UBO en memoire et son equivalent dans le shader (un uniform block)
    glBindBufferBase(GL_UNIFORM_BUFFER, 0/*meme valeur que glUniformBlockBinding*/, UBO);

}

void Terminate()
{
    glDeleteBuffers(1, &UBO);

    glDeleteTextures(1, &texID);
    glDeleteTextures(1, &cubemap_texture);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteFramebuffers(1, &FBO);

    g_BasicShader.Destroy();
}

void Render(GLFWwindow* window) 
{
    //recupere les dimensions de la fenetre
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // ici, pas de conversion sur le clearColor
    glDisable(GL_FRAMEBUFFER_SRGB);

    // temps ecoule depuis le lancement de l'application
    float time = glfwGetTime();

    /*int offscreenWidth = 512, offscreenHeight = 512;
    glBindFramebuffer(GL_FRAMEBUFFER, FBO); // rendu hors écran
    glViewport(0, 0, offscreenWidth, offscreenHeight);
    glClearColor(1.f, 0.f, 0.f, 1.f); // efface l’écran en rouge pour test
    glClear(GL_COLOR_BUFFER_BIT); // juste un color-buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // ne pas oublier de rétablir la bonne taille de viewport
    glViewport(0, 0, width, height);
    glClearColor(1.f, 1.f, 1.f, 1.f); // Efface le back-buffer en blanc
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // efface le color-buffer et depth - buffer
    */
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Conversion automatique en ecriture dans le color buffer de RGB -> sRGB
    glEnable(GL_FRAMEBUFFER_SRGB);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // inferieur strict

    auto basicProgram = g_BasicShader.GetProgram();
    glUseProgram(basicProgram);

    const uint32_t stride = sizeof(Vertex);

    if (!reader.ParseFromFile(inputfile, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                }
            }
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
    
    glBufferData(GL_ARRAY_BUFFER, attrib.vertices.size() * sizeof(GL_FLOAT_MAT3), &attrib.vertices[0], GL_STATIC_DRAW);

    // --- uniforms

    int loc_time = glGetUniformLocation(basicProgram, "u_Time");
    glUniform1f(loc_time, time);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    int loc_sampler = glGetUniformLocation(basicProgram, "u_sampler");
    glUniform1i(loc_sampler, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
    int loc_samplerEnv = glGetUniformLocation(basicProgram, "u_SamplerEnvMap");
    glUniform1i(loc_samplerEnv, 1);

    // --- matrices

    const float aspect = float(width) / float(height);
    constexpr float zNear = 0.1f;
    constexpr float zFar = 100.0f;
    constexpr float fov = 45.f;
    constexpr float fovrad = fov * M_PI / 180.0;
    const float f = 1.f / tanf(fovrad / 2.f);

    const float projectionmatrix[] = { 
        f / aspect, 0.0f, 0.0f, 0.0f, 
        0.0f, f, 0.0f, 0.0f,
        0.0f, 0.0f, (zNear + zFar) / (zNear - zFar), -1.0f,
        0.0f, 0.0f,(2.f*zNear*zFar)/(zNear-zFar), 0.0f
    };

    const float translationnmatrix[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, -5.0f, -20.0f, 1.0f // 4eme colonne
    };

    const float rotation[] = {
        cosf(time), 0.0f, -sinf(time), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sinf(time), 0.0f, cosf(time), 0.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };

    const float scale[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };
    
    // utilisation de l'UBO pour passer les matrices a l'uniform Block des shaders
    // Technique 1
    // les matrices doivent etre contigue en memoire, si on souhaite tout transferer d'un coup
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    float matrixdata[4 * 16];
    memcpy(&matrixdata[0 * 16], projectionmatrix, sizeof(float) * 16);
    memcpy(&matrixdata[1 * 16], translationnmatrix, sizeof(float) * 16);
    memcpy(&matrixdata[2 * 16], rotation, sizeof(float) * 16);
    memcpy(&matrixdata[3 * 16], scale, sizeof(float) * 16);
    glBufferData(GL_UNIFORM_BUFFER, 4 * (sizeof(float) * 16), matrixdata, GL_STREAM_DRAW);

    // Technique 2
    // a prefere si vous ne modifiez qu'une partie
    int offset = 0;
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float) * 16, projectionmatrix);
    offset += sizeof(float) * 16;
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float) * 16, translationnmatrix);
    offset += sizeof(float) * 16;
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float) * 16, rotation);
    offset += sizeof(float) * 16;
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float) * 16, scale);

    // Technique 3 - Memory-mapped I/O
    // parfois plus rapide selon l'architecture
    float* matrixPointer = (float*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(&matrixPointer[0 * 16], projectionmatrix, sizeof(float) * 16);
    memcpy(&matrixPointer[1 * 16], translationnmatrix, sizeof(float) * 16);
    memcpy(&matrixPointer[2 * 16], rotation, sizeof(float) * 16);
    memcpy(&matrixPointer[3 * 16], scale, sizeof(float) * 16);
    glUnmapBuffer(GL_UNIFORM_BUFFER);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, _countof(DragonIndices), 
        GL_UNSIGNED_SHORT, 0);

    // si je n'ai plus besoin de(s) VBOs, je peux revenir ) l'etat initial
    // CAD, que les adresses seront maintenant absolues (en RAM) et pas relative au VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    Initialize();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        Render(window);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    Terminate();

    glfwTerminate();
    return 0;
}