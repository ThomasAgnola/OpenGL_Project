#include "libs.h"
#include "GLShader.h"

GLShader g_BasicShader;
GLuint VBO;
GLuint IBO;
GLuint VAO;

Vertex vertices[] =
{
    //Position                      //Color                         //TexCoords
    glm::vec3(0.0f, 0.5f, 0.f),     255, 0, 0, 255,                 glm::vec2(0.f, 1.f),
    glm::vec3(-0.5f, -0.5f, 0.f),   0, 255, 0, 255,                 glm::vec2(0.f, 0.f),
    glm::vec3(0.5f, -0.5f, 0.f),    0, 0, 255, 255,                 glm::vec2(1.f, 0.f)
};
unsigned nbrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
    0, 1, 2
};
unsigned nbrOfIndices = sizeof(indices) / sizeof(GLuint);

void updateinput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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

    // GEN VBO and BIND/SEND DATA
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // GEN IBO and BIND/SEND DATA
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // GEN VAO and BIND
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    const uint32_t stride = sizeof(Vertex);

    int location = glGetAttribLocation(program, "a_position");
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, position));

    int color_location = glGetAttribLocation(program, "a_color");
    glEnableVertexAttribArray(color_location);
    glVertexAttribPointer(color_location, 4, GL_UNSIGNED_BYTE, /*normalisation entre [0;1]*/true, stride, reinterpret_cast<void*>(offsetof(Vertex, color)));

    //reinit tout a commencer en premier par le VAO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, nbrOfIndices, GL_UNSIGNED_INT, 0);

    // si je n'ai plus besoin de(s) VBOs, je peux revenir ) l'etat initial
    // CAD, que les adresses seront maintenant absolues (en RAM) et pas relative au VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int main(void)
{
    // Init GLFW
    glfwInit();

    // Cr�ation de la f�netre
    const int Win_width = 640;
    const int Win_height = 480;
    int framebufferWidth = 0, framebufferHeight = 0;

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(Win_width, Win_height, "OpenGL Project", NULL, NULL);

    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
    //glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
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