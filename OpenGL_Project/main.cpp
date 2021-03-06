#include "libs.h"
#include "GLShader.h"
#include "Camera.h"
#include "tiny_obj_loader.h"

//Window variables
GLFWwindow* window;
// Delta time
float dt = 0.f;
float curTime = 0.f;
float lastTime = 0.f;
//Mouse Input
double lastMouseX = 0.0;
double lastMouseY = 0.0;
double mouseX = 0.0;
double mouseY = 0.0;
double mouseOffsetX = 0.0;
double mouseOffsetY = 0.0;
bool firstMouse = true;
// Get Window size ( to recalculate every resize )
int framebufferWidth = 0, framebufferHeight = 0;


// Camera
Camera camera;

// Shader
GLShader g_BasicShader;
GLShader g_PhongShader;
// Texture
Texture texture0;
Texture texture1;
Texture texture2;
Texture texture3;
Texture red;
// Material
Material material0;
Material material1;
Material material2;
Material matRed;

// Model
std::vector<Model*> models;
//Model model;

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


void framebuffer_resize_callback(GLFWwindow* windows, int fbW, int fbH)
{
    glViewport(0, 0, fbW, fbH);
}

void updateDT()
{
    curTime = static_cast<float>(glfwGetTime());
    dt = curTime - lastTime;
    lastTime = curTime;
}

void updateMouseInput()
{
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (firstMouse)
    {
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        firstMouse = false;
    }

    // Calculate offsets
    mouseOffsetX = mouseX - lastMouseX;
    mouseOffsetY = mouseY - lastMouseY;

    // set last x et y 
    lastMouseX = mouseX;
    lastMouseY = mouseY;
}

void updateKeyboardInput()
{
    // Program
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // Cameras
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.move(dt, Forward);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.move(dt, Backward);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.move(dt, Left);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.move(dt, Right);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        camera.move(dt, Down);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera.move(dt, Up);
    }
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

    //Input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // Load Shader 1
    // Vertex
    g_BasicShader.LoadVertexShader("basic.vs");
    // Fragment
    g_BasicShader.LoadFragmentShader("basic.fs");
    // Create shader
    g_BasicShader.Create();

    // Load Shader 2
    g_PhongShader.LoadVertexShader("phong.vs");

    g_PhongShader.LoadFragmentShader("phong.fs");

    g_PhongShader.Create();


#ifdef WIN32
    wglSwapIntervalEXT(1);
#endif // WIN32

    // Program
    auto program = g_BasicShader.GetProgram();
    auto phongProgram = g_PhongShader.GetProgram();

    glGetProgramiv(program, GL_LINK_STATUS, &sucess);
    if (!sucess)
    {
        std::cout << "error link_status not sucess !" << "\n";
    }

    glGetProgramiv(phongProgram, GL_LINK_STATUS, &sucess);
    if (!sucess)
    {
        std::cout << "error link_status not sucess !" << "\n";
    }


    const uint32_t stride = sizeof(Vertex);

    // BasicProgram location
    int location = glGetAttribLocation(program, "a_position");
    int color_location = glGetAttribLocation(program, "a_color");
    int loc_texcoords = glGetAttribLocation(program, "a_texcoords");
    int normal_location = glGetAttribLocation(program, "a_normal");

    // PhongProgram location
    int phongPosition = glGetAttribLocation(phongProgram, "a_position");
    int phongNormal = glGetAttribLocation(phongProgram, "a_normal");
    int phongTexcoord = glGetAttribLocation(phongProgram, "a_texcoords");
    int phongColor = glGetAttribLocation(phongProgram, "a_color");

    // Texture init done globally -> to be usable in the render if necessary
    // Textures // Load image
    texture0.loadImage("Sunflower_from_Silesia2.jpg", GL_TEXTURE_2D, 0);

    texture1.loadImage("Minions.jpg", GL_TEXTURE_2D, 1);

    texture2.loadImage("glass-textures.png", GL_TEXTURE_2D, 2);

    texture3.loadImage("escher_animation.jpg", GL_TEXTURE_2D, 3);

    red.loadImage("Red.png", GL_TEXTURE_2D, 4);

    // Materials
    material0.loadMaterial(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.getTextureUnit(), texture1.getTextureUnit());

    material1.loadMaterial(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(2.f), texture1.getTextureUnit(), texture2.getTextureUnit());

    material2.loadMaterial(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(2.f), texture3.getTextureUnit(), texture2.getTextureUnit());

    matRed.loadMaterial(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(2.f), red.getTextureUnit(), red.getTextureUnit());
    // Load obj file via a function to a Vertex
    std::vector<Vertex> temp;
    temp = loadOBJfromlib("Deer.obj");
    //temp = loadOBJ("Deer.obj");


    // Make meshes locally 
    std::vector<Mesh*> meshes;

    //Can choose between obj or primitive
    /*
    Quad quad = Quad();
    meshes.push_back(new Mesh(&quad,
    location, color_location, loc_texcoords, normal_location,
    glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(2.f)));
    */

    Pyramid mesh_pyramid = Pyramid();
    Mesh pyramid(&mesh_pyramid,
        location, color_location, loc_texcoords, normal_location,
        glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.5f));

    // Init Meshes
    meshes.push_back(new Mesh(temp.data(), temp.size(), NULL, 0,
        location, color_location, loc_texcoords, normal_location,
        glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.001f)));

    // Init Models
    
    // init model with same mesh but position, material and texture can be different
    //models.push_back(new Model(glm::vec3(0.f), &material0, &texture0, &texture1, meshes));

    // Init model with different mesh

    models.push_back(new Model(glm::vec3(0.4f, -0.1f, 0.5f), glm::vec3(0.001f), &material0, &texture0, &texture1, "Deer.obj", location, color_location, loc_texcoords, normal_location));

    models.push_back(new Model(glm::vec3(-1.3f, -0.5f, -0.5f), glm::vec3(0.5f), &material2, &texture3, &texture2, "bunny.obj", location, color_location, loc_texcoords, normal_location));

    models.push_back(new Model(glm::vec3(0.f, -0.2f, 0.f), glm::vec3(0.005f), &material1, &texture0, &texture2, "teapot_convert.obj", location, color_location, loc_texcoords, normal_location));

    models.push_back(new Model(glm::vec3(-0.4f, -0.6f, -0.4f), &matRed, &red, &red, pyramid));

    // destruction because copied in Models
    for (auto*& i : meshes)
        delete i;

    // Init Position of Camera 
    ViewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraFront, worldUp);

    // Setting Perspective
    ProjectionMatrix = glm::perspective(
        glm::radians(fov),
        static_cast<float>(framebufferWidth) / framebufferHeight,
        nearPlane,
        farPlane
    );

    // Setting Scene light position
    glm::vec3 lightPos0(-0.5f, 0.1f, 1.f);

    glUseProgram(program);

    //envoi de la Vue au shader
    glUniformMatrix4fv(glGetUniformLocation(program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    //envoi de la Projection au shader
    glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

    //envoi de la lumiere
    glUniform3fv(glGetUniformLocation(program, "lightPos"), 1, glm::value_ptr(lightPos0));

    glUseProgram(0);

}

void Terminate()
{
    g_BasicShader.Destroy();
    g_PhongShader.Destroy();
}

void Render(GLFWwindow* window) 
{
    //recupere les dimensions de la fenetre
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto basicProgram = g_BasicShader.GetProgram();
    auto phongProgram = g_PhongShader.GetProgram();
    glUseProgram(basicProgram);

    const uint32_t stride = sizeof(Vertex);

    // --- uniforms

    //Update view matrix
    ViewMatrix = camera.getViewMatrix();
    //envoi de la nouvelle Vue au shader
    glUniformMatrix4fv(glGetUniformLocation(basicProgram, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    //envoi de la camera
    glUniform3fv(glGetUniformLocation(basicProgram, "cameraPos"), 1, glm::value_ptr(camera.getPosition()));


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

    // rotate the model
    models[0]->rotate(glm::vec3(0.f, 0.1f, 0.2f));
    models[1]->rotate(glm::vec3(0.f, 0.1f, 0.f));
    models[2]->rotate(glm::vec3(0.5f, 0.3f, 0.f));
    models[3]->rotate(glm::vec3(0.f, 0.f, 0.4f));

    // Render models
    for (auto& i : models)
    {
        glUseProgram(basicProgram);
        i->render(basicProgram);
    }


    // Test to use different shader
    /*
    glUseProgram(basicProgram);
    models[0]->render(basicProgram);
    glUseProgram(phongProgram);
    models[1]->render(phongProgram);
    glUseProgram(basicProgram);
    models[2]->render(basicProgram);
    */
    
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

    // Cr?ation de la f?netre
    const int Win_width = 1600;
    const int Win_height = 900;

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(Win_width, Win_height, "OpenGL Project", NULL, NULL);

    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
 
    camera.initCamera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f));

    glfwMakeContextCurrent(window);

    // Init GLEW
    glewExperimental = GL_TRUE;

    Initialize();

    // Boucle
    while (!glfwWindowShouldClose(window))
    {
        // maj input
        glfwPollEvents();
        //updateObject(window, mesh);

        // update
        updateDT();
        updateKeyboardInput();
        updateMouseInput();
        camera.updateInput(dt, -1, mouseOffsetX, mouseOffsetY);

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

    for (auto*& i : models)
        delete i;

    return 0;
}