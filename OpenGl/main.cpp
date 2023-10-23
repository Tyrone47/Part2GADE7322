#include <iostream>
#include <string>
// GLEW

#include <GL/glew.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Terrain.h"

#define STB_IMAGE_IMPLEMENTATION


// GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "TextRenderer.h"

// Window dimensions
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void processInput(GLFWwindow* window);


const GLuint WIDTH = 800, HEIGHT = 600;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool move_mouse = false;

bool keys_press[1024];
float yawvalue = -90.0f;	// yaw is initialized to -90.0 degrees since a yawvalue of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastXside = 800.0f / 2.0;
float lastYside = 600.0 / 2.0;
float fov = 45.0f;


GLuint shaderProgram;
GLuint shaderProgramtexture;


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool Mouse = false;


float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
int frame_counter = 0;
float elapsed_time = 0.0f;
int fps = 0;
bool display_fps = true;
GLuint VBO[86], VAO[86];






const GLchar* vertexShaderSourcechess = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"layout (location = 2) in vec2 atex;\n"
"out vec4 ourcolor;\n"
"out vec2 texcoord;\n"
"uniform mat4 MVP;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0);\n"
"ourcolor = vec4(color, 1.0);\n"
"texcoord = atex;\n"
"}\0";

// Shaders

const GLchar* fragmentShaderSourcetexturechess = "#version 330 core\n"
"out vec4 color;\n"
"in vec4 ourcolor;\n"
"in vec2 texcoord;\n"
"uniform sampler2D mytexture;\n"
"void main()\n"
"{\n"
"color = texture(mytexture, texcoord);\n"
"}\n\0";

void generate1()
{

    glGenVertexArrays(86, VAO);
    glGenBuffers(86, VBO);
    float firstxpos = -0.4f;
    float secondxpos = -0.3f;
    float firstypos = 0.5f;
    float secondypos = 0.4f;
    int index = 0;
    float white = 1.0f;
    float black = 0.0f;
    float changecolor = black;
    bool row1 = true;
    bool row2 = false;
    bool row3 = false;
    bool row4 = false;
    bool row5 = false;
    bool row6 = false;
    bool row7 = false;
    bool row8 = false;
    bool row9 = false;
    int first = 1;
    int second = 0;
    int third = 0;
    int fourth = 0;
    int fifth = 0;
    int sixth = 0;
    int seventh = 0;
    int eightth = 0;
    int nineth = 0;
    float original = -0.02f;
    float raise = 0.02f;

    srand(time(NULL));

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            float random = original + (float)rand() / (float)RAND_MAX * raise;
            if (row1)
            {

                if (first == 1 || first == 3 || first == 5 || first == 7 || first == 9)
                {
                    changecolor = black;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,  0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor, 0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor, 1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor, 1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,  0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor, 0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor, 1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor, 1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,  1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,  0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,  0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,    0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,  1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,  0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,  0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,    0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,  1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,  0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,   0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,  0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,  1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,  1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,   0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,  0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor, 0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor, 1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor, 1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,  1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,   0.0f, 1.0f


                    };

                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                if (first == 2 || first == 4 || first == 6 || first == 8)
                {
                    changecolor = white;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,   0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,     0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,   1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,   0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,    1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,     0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,     0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,     0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,      0.0f, 1.0f


                    };

                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                first++;
            }

            if (first == 10)
            {
                row2 = true;
                row1 = false;
            }
            if (row2)
            {
                if (second == 1 || second == 3 || second == 5 || second == 7 || second == 9)
                {
                    changecolor = white;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,       0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,     0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 1.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,     0.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,    0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,      0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,     1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,    0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,     0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,       0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,       1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,        1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,     1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,     0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,     0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,     0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,   0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,     0.0f, 1.0f


                    };
                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                if (second == 2 || second == 4 || second == 6 || second == 8)
                {
                    changecolor = black;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,     1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,     0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,    0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,      0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,    1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,    0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,     1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,     0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,   0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,      0.0f, 1.0f


                    };

                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                second++;
            }
            if (second == 10)
            {
                row3 = true;

            }
            if (row3)
            {
                if (third == 1 || third == 3 || third == 5 || third == 7 || third == 9)
                {
                    changecolor = black;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,   0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,   0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,     0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,   1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,   0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,   0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,   0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,   0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,  0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 1.0f


                    };

                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                if (third == 2 || third == 4 || third == 6 || third == 8)
                {
                    changecolor = white;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,     1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,     0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,    0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,      0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,   1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,   0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,    1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,     0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,     0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,     0.0f, 1.0f


                    };

                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                third++;
            }
            if (third == 10)
            {
                row4 = true;
                row3 = false;
            }
            if (row4)
            {
                if (fourth == 1 || fourth == 3 || fourth == 5 || fourth == 7 || fourth == 9)
                {
                    changecolor = white;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,     1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,     0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,    0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,      0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,    1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,    0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,     0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,     0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,   0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 1.0f


                    };

                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                if (fourth == 2 || fourth == 4 || fourth == 6 || fourth == 8)
                {
                    changecolor = black;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,  0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,     0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,    1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,    0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,    0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,  1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,   0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,   0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 1.0f


                    };
                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                fourth++;
            }
            if (fourth == 10)
            {
                row5 = true;
                row4 = false;

            }
            if (row5)
            {
                if (fifth == 1 || fifth == 3 || fifth == 5 || fifth == 7 || fifth == 9)
                {
                    changecolor = black;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,     1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,     0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,   0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,     0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,   1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,   0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,     0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,     0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,   0.0f, 1.0f


                    };
                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                if (fifth == 2 || fifth == 4 || fifth == 6 || fifth == 8)
                {
                    changecolor = white;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,     0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,     1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,    0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,      0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,    1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,    0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,     0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,       0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,       1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,       1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,     1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,     0.0f, 1.0f,

                            //Bottom 
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,   0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,   0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 1.0f


                    };
                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                fifth++;
            }
            if (fifth == 10)
            {
                row6 = true;
                row5 = false;
            }
            if (row6)
            {
                if (sixth == 1 || sixth == 3 || sixth == 5 || sixth == 7 || sixth == 9)
                {
                    changecolor = white;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,   0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,     0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,  1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,  0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,   0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,   0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,     0.0f, 1.0f


                    };

                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                if (sixth == 2 || sixth == 4 || sixth == 6 || sixth == 8)
                {
                    changecolor = black;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,   0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,   0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,    0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,  1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,  0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,   0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,   0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,   0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 1.0f


                    };

                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                sixth++;
            }
            if (sixth == 10)
            {
                row7 = true;
                row6 = false;
            }
            if (row7)
            {
                if (seventh == 1 || seventh == 3 || seventh == 5 || seventh == 7 || seventh == 9)
                {
                    changecolor = black;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,     0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,       0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,        1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,        1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,     1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,     0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,     0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,    1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,     0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,   0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,   1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,     0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,   0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,  0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,  1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,  1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,   1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,     0.0f, 1.0f


                    };
                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                if (seventh == 2 || seventh == 4 || seventh == 6 || seventh == 8)
                {
                    changecolor = white;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,   0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,     0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,   1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,   0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,    1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,     0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,     0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,    1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,       0.0f, 1.0f


                    };

                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                seventh++;
            }
            if (seventh == 10)
            {
                row8 = true;
                row7 = false;
            }
            if (row8)
            {
                if (eightth == 1 || eightth == 3 || eightth == 5 || eightth == 7 || eightth == 9)
                {
                    changecolor = white;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,   0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,   1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,     1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,    0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,      0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,   1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,   0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,      0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,        0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,        1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,       1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,     1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,      0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,    0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,   0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,    1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,     1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,      0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,     0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,    0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,     0.0f, 1.0f


                    };
                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                if (eightth == 2 || eightth == 4 || eightth == 6 || eightth == 8)
                {
                    changecolor = black;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,        0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,       0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,        1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,        0.0f, 1.0f,

                          //back 
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,     0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,    0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,      1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,      1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,       0.0f, 1.0f,

                           //left
                           //forward/back  sides  height  
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,      0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,        0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,      1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,       0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,      0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,        0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,         1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,       1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,      1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,       0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,        0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,       0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,       1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,        1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,       0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,       0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,      0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,     1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,        1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,        0.0f, 1.0f


                    };

                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                eightth++;
            }
            if (eightth == 10)
            {
                row9 = true;
                row8 = false;
            }
            if (row9)
            {
                if (nineth == 1 || nineth == 3 || nineth == 5 || nineth == 7 || nineth == 9)
                {
                    changecolor = black;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,        0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,       0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,         1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,        0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,       0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,      0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,       1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,       1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,        1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,         0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,     0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,       0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,        1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,        0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,       0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,         0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,         1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,        1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,     1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,       0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,       0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,       0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,        1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,        1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,       1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,       0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,       0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,      0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,     0.0f, 1.0f


                    };

                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                if (nineth == 2 || nineth == 4 || nineth == 6 || nineth == 8)
                {
                    changecolor = white;
                    GLfloat vertices[] =
                    {

                           firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,       0.0f, 0.0f,
                           secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      0.0f, 1.0f,
                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,

                           secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 0.0f,
                          firstxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,         1.0f, 1.0f,
                          firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,       0.0f, 1.0f,

                          //back
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,      0.0f, 0.0f,
                           secondxpos, secondypos, random,      changecolor, changecolor, changecolor,     0.0f, 1.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,     1.0f, 0.0f,
                           secondxpos, firstypos, random,       changecolor, changecolor, changecolor,    1.0f, 0.0f,
                           firstxpos, firstypos, random,       changecolor, changecolor, changecolor,      1.0f, 1.0f,
                           firstxpos, secondypos, random,      changecolor, changecolor, changecolor,     0.0f, 1.0f,

                           //left
                           //forward/back  sides  height
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,      0.0f, 0.0f,
                            firstxpos, firstypos, -0.1f,       changecolor, changecolor, changecolor,        0.0f, 1.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 0.0f,
                            firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,       1.0f, 0.0f,
                            firstxpos, secondypos, random,       changecolor, changecolor, changecolor,     1.0f, 1.0f,
                            firstxpos, firstypos, random,        changecolor, changecolor, changecolor,      0.0f, 1.0f,

                            //right
                            //x    z      y
                            //sides  forward/back    height
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,        0.0f, 0.0f,
                            secondxpos, firstypos, -0.1f,      changecolor, changecolor, changecolor,         0.0f, 1.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                            secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,       1.0f, 0.0f,
                            secondxpos, secondypos, random,      changecolor, changecolor, changecolor,       1.0f, 1.0f,
                            secondxpos, firstypos, random,       changecolor, changecolor, changecolor,      0.0f, 1.0f,

                            //Bottom
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,         0.0f, 0.0f,
                             secondxpos, secondypos, random,    changecolor, changecolor, changecolor,      0.0f, 1.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                             secondxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,      1.0f, 0.0f,
                             firstxpos, secondypos, -0.1f,     changecolor, changecolor, changecolor,        1.0f, 1.0f,
                             firstxpos, secondypos, random,    changecolor, changecolor, changecolor,       0.0f, 1.0f,

                             //Top
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,         0.0f, 0.0f,
                              secondxpos, secondypos, random,     changecolor, changecolor, changecolor,       0.0f, 1.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,
                              secondxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,      1.0f, 0.0f,
                              firstxpos, secondypos, -0.1f,      changecolor, changecolor, changecolor,        1.0f, 1.0f,
                              firstxpos, secondypos, random,     changecolor, changecolor, changecolor,       0.0f, 1.0f


                    };

                    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
                    glBindVertexArray(VAO[index]);

                    glBindBuffer(GL_ARRAY_BUFFER, VBO[index]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
                    glEnableVertexAttribArray(2);
                }
                nineth++;
            }
            //continue adding for full board


            index++;
            firstxpos += 0.1f;
            secondxpos += 0.1f;
        }


        firstxpos = -0.4f;
        secondxpos = -0.3f;
        firstypos -= 0.1f;
        secondypos -= 0.1f;

    }

}

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();

    // Set all the required options for GLFW and telling the Glfw that we using 3.3 version as seen by the second arguments
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

    int screenWidth, screenHeight;
    //callback everytime the window has been resized
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window,NULL);
    glfwSetCursorPosCallback(window,NULL);

    // GLFW Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }





    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //Calling the vertex shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourcechess, NULL);
    glCompileShader(vertexShader);
    

    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment shader
    
    GLuint fragmentShadertexture = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShadertexture, 1, &fragmentShaderSourcetexturechess, NULL);
    glCompileShader(fragmentShadertexture);
    //glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //glCompileShader(fragmentShader);
   

    // Check for compile time errors
    glGetShaderiv(fragmentShadertexture, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShadertexture, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


   

    shaderProgramtexture = glCreateProgram();
    glAttachShader(shaderProgramtexture, vertexShader);
    glAttachShader(shaderProgramtexture, fragmentShadertexture);
    glLinkProgram(shaderProgramtexture);

    

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShadertexture);
    


    unsigned int textureblock1, textureblock2, textureblock3, textureblock4, textureblock5;

    glGenTextures(1, &textureblock1);
    glBindTexture(GL_TEXTURE_2D, textureblock1);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("res/images/Wood.jpg", &width, &height,
        &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &textureblock2);
    glBindTexture(GL_TEXTURE_2D, textureblock2);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width1, height1, nrChannels1;
    unsigned char* data1 = stbi_load("res/images/tiles.jpg", &width1, &height1,
        &nrChannels1, 0);
    if (data1)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data1);

    glGenTextures(1, &textureblock3);
    glBindTexture(GL_TEXTURE_2D, textureblock3);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width2, height2, nrChannels2;
    unsigned char* data2 = stbi_load("res/images/BlackTile.jpg", &width2, &height2,
        &nrChannels2, 0);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data2);

    

    
    //chessboard 
    generate1();


    float xcoordinates = -0.35f;
    float xcoordinates2 = -0.3f;
    float ycoordinates = 0.5f;
    float ycoordinates2 = 0.4f;
    int index_border = 81;
    int chessborder = 0;
    int chessborder2 = 0;
    int chessborder3 = 0;
    for (int x = 0; x < 4; x++)
    {
        if (chessborder == 0)
        {
            GLfloat vertices[] =
            {
                -0.4f, 0.5f, -0.05f,   1.0f, 1.0f, 0.0f,     0.0f, 0.0f,
                -0.45f, 0.55f, -0.05f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
                -0.45f, -0.45f, -0.05f,  1.0f, 1.0f, 0.0f,   1.0f, 0.0f,

                 -0.4f, -0.4f, -0.05f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f,
                 -0.4f, 0.5f, -0.05f,    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
                 -0.45f, -0.45f, -0.05f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f
            };
            glBindVertexArray(VAO[index_border]);

            glBindBuffer(GL_ARRAY_BUFFER, VBO[index_border]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);

        }
        if (chessborder == 1)
        {
            GLfloat vertices[] =
            {
                -0.4f, 0.5f, -0.05f,   1.0f, 1.0f, 0.0f,    0.0f, 0.0f,
                -0.45f, 0.55f, -0.05f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
                 0.55f, 0.55f, -0.05f,  1.0f, 1.0f, 0.0f,   1.0f, 0.0f,

                 0.5f, 0.5f, -0.05f,   1.0f, 1.0f, 0.0f,    1.0f, 0.0f,
                 0.55f, 0.55f, -0.05f,    1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
                 -0.4f, 0.5f, -0.05f,   1.0f, 1.0f, 0.0f,     0.0f, 1.0f
            };
            glBindVertexArray(VAO[index_border]);

            glBindBuffer(GL_ARRAY_BUFFER, VBO[index_border]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
        }
        if (chessborder == 2)
        {
            GLfloat vertices[] =
            {
                0.5f, 0.5f, -0.05f,   1.0f, 1.0f, 0.0f,       0.0f, 0.0f,
                0.55f, 0.55f, -0.05f,   1.0f, 1.0f, 0.0f,     0.0f, 1.0f,
                 0.55f, -0.45f, -0.05f,  1.0f, 1.0f, 0.0f,    1.0f, 0.0f,

                 0.5f, -0.45f, -0.05f,   1.0f, 1.0f, 0.0f,    1.0f, 0.0f,
                 0.55f, -0.45f, -0.05f,    1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
                 0.5f, 0.5f, -0.05f,   1.0f, 1.0f, 0.0f,        0.0f, 1.0f
            };
            glBindVertexArray(VAO[index_border]);

            glBindBuffer(GL_ARRAY_BUFFER, VBO[index_border]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
        }
        if (chessborder == 3)
        {
            GLfloat vertices[] =
            {
                0.5f, -0.4f, -0.05f,   1.0f, 1.0f, 0.0f,    0.0f, 0.0f,
                0.55f, -0.45f, -0.05f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
                -0.4f, -0.4f, -0.05f,  1.0f, 1.0f, 0.0f,    1.0f, 0.0f,

                 -0.45f, -0.45f, -0.05f,   1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
                 -0.4f, -0.4f, -0.05f,    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
                 0.55f, -0.45f, -0.05f,   1.0f, 1.0f, 0.0f,    0.0f, 1.0f
            };
            glBindVertexArray(VAO[index_border]);

            glBindBuffer(GL_ARRAY_BUFFER, VBO[index_border]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
        }

        chessborder++;
        index_border++;
    }
    //uhmmmmmm

    GLfloat vertices3D[] =
    {

        //Back 
         -0.45f, -0.45f, -0.1f,     1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
          0.55f, -0.45f, -0.1f,     1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
          0.55f, 0.55f, -0.1f,      1.0f, 1.0f, 0.0f,  1.0f, 0.0f,

          0.55f, 0.55f, -0.1f,      1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
         -0.45f, 0.55f, -0.1f,      1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
         -0.45f, -0.45f, -0.1f,     1.0f, 1.0f, 0.0f,  0.0f, 1.0f,

         //Front
         -0.45f, -0.45f, -0.05f,      1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
          0.55f, -0.45f, -0.05f,      1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
          0.55f, 0.55f, -0.05f,       1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
          0.55f, 0.55f, -0.05f,       1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
         -0.45f, 0.55f, -0.05f,       1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
         -0.45f, -0.45f, -0.05f,      1.0f, 1.0f, 0.0f,  0.0f, 1.0f,

         //left
         //forward/back  sides  height
         -0.45f, 0.55f, -0.02f,        1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
         -0.45f, 0.55f, -0.1f,       1.0f, 1.0f, 0.0f,    0.0f, 1.0f,
         -0.45f, -0.45f, -0.1f,      1.0f, 1.0f, 0.0f,    1.0f, 0.0f,
         -0.45f, -0.45f, -0.1f,      1.0f, 1.0f, 0.0f,    1.0f, 0.0f,
         -0.45f, -0.45f, -0.02f,       1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
         -0.45f, 0.55f, -0.02f,        1.0f, 1.0f, 0.0f,  0.0f, 1.0f,

         //right
         //x    z      y
         //sides  forward/back    height
         0.55f, 0.55f, -0.02f,       1.0f, 1.0f, 0.0f,   0.0f, 0.0f,
         0.55f, 0.55f, -0.1f,      1.0f, 1.0f, 0.0f,     0.0f, 1.0f,
         0.55f, -0.45f, -0.1f,     1.0f, 1.0f, 0.0f,     1.0f, 0.0f,
         0.55f, -0.45f, -0.1f,     1.0f, 1.0f, 0.0f,     1.0f, 0.0f,
         0.55f, -0.45f, -0.02f,      1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         0.55f, 0.55f, -0.02f,       1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

         //Bottom
         -0.45f, -0.45f, -0.02f,    1.0f, 1.0f, 0.0f,   0.0f, 0.0f,
          0.55f, -0.45f, -0.02f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
          0.55f, -0.45f, -0.1f,     1.0f, 1.0f, 0.0f,   1.0f, 0.0f,
          0.55f, -0.45f, -0.1f,     1.0f, 1.0f, 0.0f,   1.0f, 0.0f,
         -0.45f, -0.45f, -0.1f,     1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         -0.45f, -0.45f, -0.02f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

         //Top
         -0.45f, 0.55f, -0.02f,     1.0f, 1.0f, 0.0f,   0.0f, 0.0f,
          0.55f, 0.55f, -0.02f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
          0.55f, 0.55f, -0.1f,      1.0f, 1.0f, 0.0f,   1.0f, 0.0f,
          0.55f, 0.55f, -0.1f,      1.0f, 1.0f, 0.0f,   1.0f, 0.0f,
         -0.45f, 0.55f, -0.1f,      1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         -0.45f, 0.55f, -0.02f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

    };
    glBindVertexArray(VAO[85]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[85]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3D), vertices3D, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
 
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //process input
        //moving();
       

        // Render
        // Clear the colorbuffer
        //glClearColor specifies the color on the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       
       
        glUseProgram(shaderProgramtexture);
        

        for (int block = 0; block < 81; block++)
        {
            if (block == 0 || block == 2 || block == 4 || block == 6 || block == 8 || block == 10 || block == 12 || block == 14 || block == 16 || block == 18 || block == 20)
            {
                
                glm::mat4 model4 = glm::mat4(1.0f); 
                glm::mat4 view4 = glm::mat4(1.0f);
                glm::mat4 projection4 = glm::mat4(1.0f);
                float radius = 10.0f;
                float camX = static_cast<float>(sin(glfwGetTime()) * radius);
                float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
                view4 = camera.GetViewMatrix();
                model4 = glm::rotate(model4, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
                
                projection4 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

                //Camera
                unsigned int modelLoc4 = glGetUniformLocation(shaderProgramtexture, "model");
                unsigned int viewLoc4 = glGetUniformLocation(shaderProgramtexture, "view");
                unsigned int theprojection4 = glGetUniformLocation(shaderProgramtexture, "projection");

                glUniformMatrix4fv(modelLoc4, 1, GL_FALSE, glm::value_ptr(model4));
                glUniformMatrix4fv(viewLoc4, 1, GL_FALSE, &view4[0][0]);
                glUniformMatrix4fv(theprojection4, 1, GL_FALSE, &projection4[0][0]);
                
                glBindTexture(GL_TEXTURE_2D, textureblock2);
                glBindVertexArray(VAO[block]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (block == 22 || block == 24 || block == 26 || block == 28 || block == 30 || block == 32 || block == 34 || block == 36 || block == 38 || block == 40 || block == 42)
            {
                //glUseProgram(shaderProgramlight);
                glm::mat4 model4 = glm::mat4(1.0f); 
                glm::mat4 view4 = glm::mat4(1.0f);
                glm::mat4 projection4 = glm::mat4(1.0f);
                float radius = 10.0f;
                float camX = static_cast<float>(sin(glfwGetTime()) * radius);
                float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
                view4 = camera.GetViewMatrix();
                model4 = glm::rotate(model4, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
                
                projection4 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

                //Camera
                unsigned int modelLoc4 = glGetUniformLocation(shaderProgramtexture, "model");
                unsigned int viewLoc4 = glGetUniformLocation(shaderProgramtexture, "view");
                unsigned int theprojection4 = glGetUniformLocation(shaderProgramtexture, "projection");

                glUniformMatrix4fv(modelLoc4, 1, GL_FALSE, glm::value_ptr(model4));
                glUniformMatrix4fv(viewLoc4, 1, GL_FALSE, &view4[0][0]);
                glUniformMatrix4fv(theprojection4, 1, GL_FALSE, &projection4[0][0]);
                
                glBindTexture(GL_TEXTURE_2D, textureblock2);
                glBindVertexArray(VAO[block]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (block == 44 || block == 46 || block == 48 || block == 50 || block == 52 || block == 54 || block == 56 || block == 58 || block == 60 || block == 62 || block == 64)
            {
                
                glm::mat4 model4 = glm::mat4(1.0f); 
                glm::mat4 view4 = glm::mat4(1.0f);
                glm::mat4 projection4 = glm::mat4(1.0f);
                float radius = 10.0f;
                float camX = static_cast<float>(sin(glfwGetTime()) * radius);
                float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
                view4 = camera.GetViewMatrix();
                model4 = glm::rotate(model4, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
              
                projection4 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

                //Camera
                unsigned int modelLoc4 = glGetUniformLocation(shaderProgramtexture, "model");
                unsigned int viewLoc4 = glGetUniformLocation(shaderProgramtexture, "view");
                unsigned int theprojection4 = glGetUniformLocation(shaderProgramtexture, "projection");

                glUniformMatrix4fv(modelLoc4, 1, GL_FALSE, glm::value_ptr(model4));
                glUniformMatrix4fv(viewLoc4, 1, GL_FALSE, &view4[0][0]);
                glUniformMatrix4fv(theprojection4, 1, GL_FALSE, &projection4[0][0]);
              
                glBindTexture(GL_TEXTURE_2D, textureblock2);
                glBindVertexArray(VAO[block]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (block == 66 || block == 68 || block == 70 || block == 72 || block == 74 || block == 76 || block == 78 || block == 80)
            {
                
                glm::mat4 model4 = glm::mat4(1.0f); 
                glm::mat4 view4 = glm::mat4(1.0f);
                glm::mat4 projection4 = glm::mat4(1.0f);
                float radius = 10.0f;
                float camX = static_cast<float>(sin(glfwGetTime()) * radius);
                float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
                view4 = camera.GetViewMatrix();
                model4 = glm::rotate(model4, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
                
                projection4 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

                //Camera
                unsigned int modelLoc4 = glGetUniformLocation(shaderProgramtexture, "model");
                unsigned int viewLoc4 = glGetUniformLocation(shaderProgramtexture, "view");
                unsigned int theprojection4 = glGetUniformLocation(shaderProgramtexture, "projection");

                glUniformMatrix4fv(modelLoc4, 1, GL_FALSE, glm::value_ptr(model4));
                glUniformMatrix4fv(viewLoc4, 1, GL_FALSE, &view4[0][0]);
                glUniformMatrix4fv(theprojection4, 1, GL_FALSE, &projection4[0][0]);
            
                glBindTexture(GL_TEXTURE_2D, textureblock2);
                glBindVertexArray(VAO[block]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else
            {
                
                glm::mat4 model4 = glm::mat4(1.0f);
                glm::mat4 view4 = glm::mat4(1.0f);
                glm::mat4 projection4 = glm::mat4(1.0f);
                float radius = 10.0f;
                float camX = static_cast<float>(sin(glfwGetTime()) * radius);
                float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
                view4 = camera.GetViewMatrix();
                model4 = glm::rotate(model4, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            
                projection4 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

                //Camera
                unsigned int modelLoc4 = glGetUniformLocation(shaderProgramtexture, "model");
                unsigned int viewLoc4 = glGetUniformLocation(shaderProgramtexture, "view");
                unsigned int theprojection4 = glGetUniformLocation(shaderProgramtexture, "projection");

                glUniformMatrix4fv(modelLoc4, 1, GL_FALSE, glm::value_ptr(model4));
                glUniformMatrix4fv(viewLoc4, 1, GL_FALSE, &view4[0][0]);
                glUniformMatrix4fv(theprojection4, 1, GL_FALSE, &projection4[0][0]);
                
                glBindTexture(GL_TEXTURE_2D, textureblock3);
                glBindVertexArray(VAO[block]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

        }
        
        glm::mat4 model5 = glm::mat4(1.0f); 
        glm::mat4 view5 = glm::mat4(1.0f);
        glm::mat4 projection5 = glm::mat4(1.0f);
        view5 = camera.GetViewMatrix();
        model5 = glm::rotate(model5, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        
        projection5 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        //Camera
        unsigned int modelLoc5 = glGetUniformLocation(shaderProgramtexture, "model");
        unsigned int viewLoc5 = glGetUniformLocation(shaderProgramtexture, "view");
        unsigned int theprojection5 = glGetUniformLocation(shaderProgramtexture, "projection");

        glUniformMatrix4fv(modelLoc5, 1, GL_FALSE, glm::value_ptr(model5));
        glUniformMatrix4fv(viewLoc5, 1, GL_FALSE, &view5[0][0]);
        glUniformMatrix4fv(theprojection5, 1, GL_FALSE, &projection5[0][0]);
        
        for (int x = 81; x < 85; x++)
        {
            glBindTexture(GL_TEXTURE_2D, textureblock1);
            glBindVertexArray(VAO[x]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        glm::mat4 model6 = glm::mat4(1.0f); 
        glm::mat4 view6 = glm::mat4(1.0f);
        glm::mat4 projection6 = glm::mat4(1.0f);
        view6 = camera.GetViewMatrix();
        model6 = glm::rotate(model6, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        
        projection6 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        //Camera
        unsigned int modelLoc6 = glGetUniformLocation(shaderProgramtexture, "model");
        unsigned int viewLoc6 = glGetUniformLocation(shaderProgramtexture, "view");
        unsigned int theprojection6 = glGetUniformLocation(shaderProgramtexture, "projection");

        glUniformMatrix4fv(modelLoc6, 1, GL_FALSE, glm::value_ptr(model6));
        glUniformMatrix4fv(viewLoc6, 1, GL_FALSE, &view6[0][0]);
        glUniformMatrix4fv(theprojection6, 1, GL_FALSE, &projection6[0][0]);
       
        glBindTexture(GL_TEXTURE_2D, textureblock1);
        glBindVertexArray(VAO[85]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Activiting the shader and calling the methods for chess pieces
        

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    
    
    glfwTerminate();

    return EXIT_SUCCESS;
}



void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //Pressing the Escape key to exit
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }


    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys_press[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys_press[key] = false;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    //Movement of the mouse
    float xposition = static_cast<float>(xposIn);
    float yposition = static_cast<float>(yposIn);

    if (Mouse)
    {
        lastXside = xposition;
        lastYside = yposition;
        Mouse = false;
    }

    GLfloat xOffset = xposition - lastXside;
    GLfloat yOffset = lastYside - yposition;  // Reversed since y-coordinates go from bottom to left

    lastXside = xposition;
    lastYside = yposition;

    if (move_mouse) {
        camera.ProcessMouseMovement(xOffset, yOffset);
    }



}
