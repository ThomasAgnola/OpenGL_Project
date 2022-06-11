#include "libs.h"
#include "GLShader.h"

GLShader g_BasicShader;
// Texture
Texture texture0;
Texture texture1;
// Material
Material material0;
Material material1;
// Mesh
Mesh mesh;

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

void updateinput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void updateObject(GLFWwindow* window, Mesh& mesh)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(0.f, 0.f, -0.01f));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(-0.01f, 0.f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(0.f, 0.f, 0.01f));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        mesh.move(glm::vec3(0.01f, 0.f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        mesh.rotate(glm::vec3(0.f, -1.f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        mesh.rotate(glm::vec3(0.f, 1.f, 0.f));
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        mesh.scale(glm::vec3(0.01f));
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        mesh.scale(glm::vec3(-0.01f));
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

    const uint32_t stride = sizeof(Vertex);

    int location = glGetAttribLocation(program, "a_position");

    int color_location = glGetAttribLocation(program, "a_color");

    int loc_texcoords = glGetAttribLocation(program, "a_texcoords");

    int normal_location = glGetAttribLocation(program, "a_normal");

    // Model

    //Can change figure by primitive
    Triangle triangle = Triangle();

    //Quad quad = Quad();
    mesh.loadMesh(&triangle,
        location, color_location, loc_texcoords, normal_location, 
        glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f));

    // Texture init done globally -> to be usable in the render
    // Texture 0 // Load image
    texture0.loadImage("Sunflower_from_Silesia2.jpg", GL_TEXTURE_2D, 0);

    // Texture 1 // Load image
    texture1.loadImage("Minions.jpg", GL_TEXTURE_2D, 1);

    // Material 0
    material0.loadMaterial(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.getTextureUnit(), texture1.getTextureUnit());
    
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

    // update frameBufferSize
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

    mesh.render(basicProgram);

    // fin du render, retour a l'etat initial
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
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
        updateObject(window, mesh);

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