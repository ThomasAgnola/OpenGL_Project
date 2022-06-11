#include "libs.h"
#include "GLShader.h"
#include "Texture.h"
#include "Material.h"

GLShader g_BasicShader;
// Texture
Texture texture0;
Texture texture1;
//Material
Material material0;
Material material1;

GLuint VBO;
GLuint IBO;
GLuint VAO;

glm::mat4 ModelMatrix(1.f);
glm::mat4 ViewMatrix(1.f);

// Camera start Position
glm::vec3 worldUp(0.f, 1.f, 0.f);
glm::vec3 cameraFront(0.f, 0.f, -1.f);
glm::vec3 cameraPosition(0.f, 0.f, 1.f);

// Set Camera parameters
float fov = 90.f;
float nearPlane = 0.1f;
float farPlane = 100.f;
glm::mat4 ProjectionMatrix(1.f);

// Get Window size ( to recalculate every resize )
int framebufferWidth = 0, framebufferHeight = 0;

// defintion de variable pour la position/rotation/scale de l'object
glm::vec3 objectPosition(0.f);
glm::vec3 objectRotation(0.f);
glm::vec3 objectScale(1.f);

Vertex vertices[] =
{
    //Position                      //Color                         //TexCoords             //Normal
    glm::vec3(-0.5f, 0.5f, 0.f),    255, 0, 0, 255,                 glm::vec2(0.f, 0.f),    glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(-0.5f, -0.5f, 0.f),   0, 255, 0, 255,                 glm::vec2(0.f, 1.f),    glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(0.5f, -0.5f, 0.f),    0, 0, 255, 255,                 glm::vec2(1.f, 1.f),    glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(0.5f, 0.5f, 0.f),     0, 255, 0, 255,               glm::vec2(1.f, 0.f),    glm::vec3(0.f, 0.f, -1.f)
};
unsigned nbrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
    0, 1, 2, // Triangle 1
    0, 2, 3 // Triangle 2
};
unsigned nbrOfIndices = sizeof(indices) / sizeof(GLuint);

void updateinput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void updateObject(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position.z -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        position.x -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position.z += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        position.x += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        rotation.y -= 1.f;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        rotation.y += 1.f;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        scale += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        scale -= 0.01f;
    }
}

void framebuffer_resize_callback(GLFWwindow* windows, int fbW, int fbH)
{
    glViewport(0, 0, fbW, fbH);
}

void Initialize()
{
    GLenum error = glewInit();
    GLint sucess;
    if (error != GL_NO_ERROR)
    {
        std::cout << "Erreur d'initialisation de GLEW\n";
    }

    // OPENGL Options
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Vertex
    g_BasicShader.LoadVertexShader("basic.vs");

    // Fragment
    g_BasicShader.LoadFragmentShader("basic.fs");

    g_BasicShader.Create();
#ifdef WIN32
    wglSwapIntervalEXT(1);
#endif // WIN32

    // Program
    auto program = g_BasicShader.GetProgram();

    glGetProgramiv(program, GL_LINK_STATUS, &sucess);
    if (!sucess)
    {
        std::cout << "error link_status not sucess !" << "\n";
    }

    // VAO, VBO, IBO
    // GEN VAO and BIND
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // GEN VBO and BIND/SEND DATA
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // GEN IBO and BIND/SEND DATA
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    const uint32_t stride = sizeof(Vertex);

    int location = glGetAttribLocation(program, "a_position");
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, position));

    int color_location = glGetAttribLocation(program, "a_color");
    glEnableVertexAttribArray(color_location);
    glVertexAttribPointer(color_location, 4, GL_UNSIGNED_BYTE, /*normalisation entre [0;1]*/true, stride, reinterpret_cast<void*>(offsetof(Vertex, color)));

    int loc_texcoords = glGetAttribLocation(program, "a_texcoords");
    glEnableVertexAttribArray(loc_texcoords);
    glVertexAttribPointer(loc_texcoords, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, texcoords));

    int normal_location = glGetAttribLocation(program, "a_normal");
    glEnableVertexAttribArray(normal_location);
    glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, normal));

    //reinit tout a commencer en premier par le VAO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Texture init done globally -> to be usable in the render
    // Texture 0 // Load image
    texture0.loadImage("Sunflower_from_Silesia2.jpg", GL_TEXTURE_2D, 0);

    // Texture 1 // Load image
    texture1.loadImage("Minions.jpg", GL_TEXTURE_2D, 1);

    // Material 0
    material0.loadMaterial(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.getTextureUnit(), texture1.getTextureUnit());

    //Matrice modèle / mouvement / rotation etc..
    //multiplication des matrices de droite à gauche avec OpenGL /!\ //
    //glm::mat4 ModelMatrix(1.f); //Matrice identitée de 4x4
    ModelMatrix = glm::mat4(1.f);
    ModelMatrix = glm::translate(ModelMatrix, objectPosition); //matrice identité mais avec des valeurs dans la dernière colonne // on multiplie la matrice de coordonnée avec la matrice de translation et les valeurs s'additionnent
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(objectRotation.x), glm::vec3(1.f, 0.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(objectRotation.y), glm::vec3(0.f, 1.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(objectRotation.z), glm::vec3(0.f, 0.f, 1.f));
    ModelMatrix = glm::scale(ModelMatrix, objectScale);
    
    // Init Position of Camera 
    ViewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, worldUp);

    // Setting Perspective
    ProjectionMatrix = glm::perspective(
        glm::radians(fov),
        static_cast<float>(framebufferWidth) / framebufferHeight,
        nearPlane,
        farPlane
    );

    glm::vec3 lightPos0(0.f, 0.f, 1.f);

    glUseProgram(program);

    //envoi de la matrice au shader 
    glUniformMatrix4fv(glGetUniformLocation(program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    //envoi de la Vue au shader
    glUniformMatrix4fv(glGetUniformLocation(program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    //envoi de la Projection au shader
    glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

    //envoi de lumiere
    glUniform3fv(glGetUniformLocation(program, "lightPos"), 1, glm::value_ptr(lightPos0));
    //envoi de la camera
    glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, glm::value_ptr(cameraPosition));

    glUseProgram(0);

}

void Terminate()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glDeleteVertexArrays(1, &VAO);

    g_BasicShader.Destroy();
}

void Render(GLFWwindow* window)
{
    //recupere les dimensions de la fenetre
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // temps ecoule depuis le lancement de l'application
    float time = glfwGetTime();

    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto basicProgram = g_BasicShader.GetProgram();
    glUseProgram(basicProgram);

    const uint32_t stride = sizeof(Vertex);

    // --- uniforms

    int loc_time = glGetUniformLocation(basicProgram, "u_Time");
    glUniform1f(loc_time, time);


    //Mouvement Rotation Scale
    ModelMatrix = glm::mat4(1.f);
    ModelMatrix = glm::translate(ModelMatrix, objectPosition); //matrice identité mais avec des valeurs dans la dernière colonne // on multiplie la matrice de coordonnée avec la matrice de translation et les valeurs s'additionnent
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(objectRotation.x), glm::vec3(1.f, 0.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(objectRotation.y), glm::vec3(0.f, 1.f, 0.f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(objectRotation.z), glm::vec3(0.f, 0.f, 1.f));
    ModelMatrix = glm::scale(ModelMatrix, objectScale);

    glUniformMatrix4fv(glGetUniformLocation(basicProgram, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    // Updating Perspective
    ProjectionMatrix = glm::mat4(1.f);
    ProjectionMatrix = glm::perspective(
        glm::radians(fov),
        static_cast<float>(framebufferWidth) / framebufferHeight,
        nearPlane,
        farPlane
    );
    glUniformMatrix4fv(glGetUniformLocation(basicProgram, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));


    // Activating Texture
    texture0.bind();

    texture1.bind();

    material0.sendToShader(basicProgram);

    // Bind VAO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, nbrOfIndices, GL_UNSIGNED_INT, 0);

    // fin du render, retour a l'etat initial
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int main(void)
{
    // Init GLFW
    glfwInit();

    // Création de la fênetre
    const int Win_width = 640;
    const int Win_height = 480;

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(Win_width, Win_height, "OpenGL Project", NULL, NULL);

    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
    //glViewport(0, 0, framebufferWidth, framebufferHeight);

    glfwMakeContextCurrent(window);

    // Init GLEW
    glewExperimental = GL_TRUE;

    Initialize();

    // Boucle
    while (!glfwWindowShouldClose(window))
    {
        // maj input
        glfwPollEvents();
        updateObject(window, objectPosition, objectRotation, objectScale);

        // update
        updateinput(window);

        // maj fenetre
        // clear
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // draw
        Render(window);

        // end
        glfwSwapBuffers(window);
        glFlush();
    }

    Terminate();

    // fin du programme
    glfwTerminate();


    return 0;
}