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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void processInput(GLFWwindow* window);
void moving();

const GLuint WIDTH = 800, HEIGHT = 600;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraforward = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
bool Using_mouse = true;

GLuint shaderProgram;
GLuint shaderProgramtexture;
GLuint shaderProgramlighting;
GLuint shaderProgramlightingcube;
GLuint shaderProgrampawnpiece;
GLuint shaderProgramrookpiece;
GLuint shaderProgramkingpiece;
GLuint shaderProgramqueenpiece;
GLuint shaderProgrambishoppiece;
GLuint shaderProgramknightpiece;



bool ActiveMouse = true;

bool keys[1024];
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastXpos = 800.0f / 2.0;
float lastYpos = 600.0 / 2.0;
float fov = 45.0f;

glm::vec3 cameraknightpiece = glm::vec3(-0.3f, 0.0f, -3.0f);
glm::vec3 cameraknightheightpiece = glm::vec3(-0.3f, 0.12f, -3.0f);
glm::vec3 cameraknighttoppiece = glm::vec3(-0.3f, 0.15f, -3.0f);

glm::vec3 camerabishoppiece = glm::vec3(-0.15f, 0.0f, -3.0f);
glm::vec3 camerabishopheightpiece = glm::vec3(-0.15f, 0.18f, -3.0f);
glm::vec3 camerabishoptoppiece = glm::vec3(-0.15f, 0.25f, -3.0f);

glm::vec3 cameraqueenpiece = glm::vec3(0.0f, 0.0f, -3.0f);
glm::vec3 cameraqueenheightpiece = glm::vec3(0.0f, 0.2f, -3.0f);
glm::vec3 cameraqueentoppiece = glm::vec3(0.0f, 0.3f, -3.0f);

glm::vec3 camerakingpiece = glm::vec3(0.18f, 0.0f, -3.0f);
glm::vec3 camerakingheightpiece = glm::vec3(0.18f, 0.3f, -3.0f);
glm::vec3 camerakingtoppiece = glm::vec3(0.18f, 0.35f, -3.0f);

glm::vec3 camerarookpiece = glm::vec3(0.35f, 0.0f, -3.0f);
glm::vec3 camerarookheightpiece = glm::vec3(0.35f, 0.2f, -3.0f);
glm::vec3 camerarooktoppiece = glm::vec3(0.35f, 0.2f, -3.0f);

glm::vec3 camerapawnpiece = glm::vec3(0.5f, 0.0f, -3.0f);
glm::vec3 camerapawnheightpiece = glm::vec3(0.5f, 0.14f, -3.0f);
glm::vec3 camerapawntoppiece = glm::vec3(0.5f, 0.14f, -3.0f);

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
const GLchar* vertexShaderSourcelighting = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 color;\n"
"layout (location = 2) in vec2 atex;\n"
"uniform mat4 VP;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform vec3 aNormal;\n"
"uniform mat4 projection;\n"
"out vec3 Normal;\n"
"out vec2 TexCoords;\n"
"out vec3 FragPos;\n"
"void main()\n"
"{\n"
"FragPos = vec3(model * vec4(aPos, 1.0));\n"
"Normal = mat3(transpose(inverse(model))) * aNormal;\n"
"TexCoords = atex;\n"
"gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"}\0";
const GLchar* vertexShaderSourcecubelight = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 VP;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\0";
const GLchar* fragmentShaderSourcecombinelight = "#version 330 core\n"
"#define NUMBER_OF_POINT_LIGHTS 1\n"
"out vec4 color;\n"
"struct Material {\n"
"sampler2D diffuse;\n"
"sampler2D specular;\n"
"float shininess;\n"
"};\n"
"struct DirLight {\n"
"vec3 direction;\n"
"vec3 ambient;\n"
"vec3 diffuse;\n"
"vec3 specular;\n"
"};\n"
"struct PointLight {\n"
"vec3 position;\n"
"float constant;\n"
"float linear;\n"
"float quadratic;\n"
"vec3 ambient;\n"
"vec3 diffuse;\n"
"vec3 specular;\n"
"};\n"
"struct SpotLight {\n"
"vec3 position;\n"
"vec3 direction;\n"
"float cutOff;\n"
"float outerCutOff;\n"
"float constant;\n"
"float linear;\n"
"float quadratic;\n"
"vec3 ambient;\n"
"vec3 diffuse;\n"
"vec3 specular;\n"
"};\n"
"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"in vec2 TexCoords;\n"
"uniform vec3 viewPos;\n"
"uniform Material material;\n"
"uniform DirLight dirLight;\n"
"uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];\n"
"uniform SpotLight spotLight;\n"
"vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);\n"
"vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
"vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
"void main()\n"
"{\n"
"vec3 norm = normalize(Normal);\n"
"vec3 viewDir = normalize(viewPos - FragPos);\n"
"vec3 result = CalcDirLight( dirLight, norm, viewDir );\n"
"for ( int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++ ){\n"
"result += CalcPointLight( pointLights[i], norm, FragPos, viewDir );\n"
"}\n"
"result += CalcSpotLight( spotLight, norm, FragPos, viewDir );\n"
"color = vec4(result, 1.0);\n"
"}\n"
"vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir ){\n"
"vec3 lightDir = normalize( -light.direction );\n"
"float diff = max( dot( normal, lightDir ), 0.0 );\n"
"vec3 reflectDir = reflect( -lightDir, normal );\n"
"float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );\n"
"vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ) );\n"
"vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ) );\n"
"vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );\n"
"return ( ambient + diffuse + specular );\n"
"}\n"
"vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir ){\n"
"vec3 lightDir = normalize( light.position - fragPos );\n"
"float diff = max( dot( normal, lightDir ), 0.0 );\n"
"vec3 reflectDir = reflect( -lightDir, normal );\n"
"float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );\n"
"float distance = length( light.position - fragPos );\n"
"float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );\n"
"vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ) );\n"
"vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ) );\n"
"vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );\n"
"ambient *= attenuation;\n"
"diffuse *= attenuation;\n"
"specular *= attenuation;\n"
"return ( ambient + diffuse + specular );\n"
"}\n"
"vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir ){\n"
"vec3 lightDir = normalize( light.position - fragPos );\n"
"float diff = max( dot( normal, lightDir ), 0.0 );\n"
"vec3 reflectDir = reflect( -lightDir, normal );\n"
"float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );\n"
"float distance = length( light.position - fragPos );\n"
"float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );\n"
"float theta = dot( lightDir, normalize( -light.direction ) );\n"
"float epsilon = light.cutOff - light.outerCutOff;\n"
"float intensity = clamp( ( theta - light.outerCutOff ) / epsilon, 0.0, 1.0 );\n"
"vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ) );\n"
"vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ) );\n"
"vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );\n"
"ambient *= attenuation * intensity;\n"
"diffuse *= attenuation * intensity;\n"
"specular *= attenuation * intensity;\n"
"return ( ambient + diffuse + specular );\n"
"}\n\0";
const GLchar* fragmentShaderSourcecubelight = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0);\n"
"}\n\0";
const GLchar* fragmentShaderSourcetexturechess = "#version 330 core\n"
"out vec4 color;\n"
"in vec4 ourcolor;\n"
"in vec2 texcoord;\n"
"uniform sampler2D mytexture;\n"
"void main()\n"
"{\n"
"color = texture(mytexture, texcoord);\n"
"}\n\0";

const GLchar* fragmentShaderSourcepawnchesspiece = "#version 330 core\n"
"out vec4 color;\n"
"in vec4 ourcolor;\n"
"in vec2 texcoord;\n"
"uniform sampler2D mytexture;\n"
"void main()\n"
"{\n"
"color = texture(mytexture, texcoord);\n"
"}\n\0";
const GLchar* fragmentShaderSourcerookchesspiece = "#version 330 core\n"
"out vec4 color;\n"
"in vec4 ourcolor;\n"
"in vec2 texcoord;\n"
"uniform sampler2D mytexture;\n"
"void main()\n"
"{\n"
"color = texture(mytexture, texcoord);\n"
"}\n\0";
const GLchar* fragmentShaderSourcekingchesspiece = "#version 330 core\n"
"out vec4 color;\n"
"in vec4 ourcolor;\n"
"in vec2 texcoord;\n"
"uniform sampler2D mytexture;\n"
"void main()\n"
"{\n"
"color = texture(mytexture, texcoord);\n"
"}\n\0";
const GLchar* fragmentShaderSourcequeenchesspiece = "#version 330 core\n"
"out vec4 color;\n"
"in vec4 ourcolor;\n"
"in vec2 texcoord;\n"
"uniform sampler2D mytexture;\n"
"void main()\n"
"{\n"
"color = texture(mytexture, texcoord);\n"
"}\n\0";
const GLchar* fragmentShaderSourcebishopchesspiece = "#version 330 core\n"
"out vec4 color;\n"
"in vec4 ourcolor;\n"
"in vec2 texcoord;\n"
"uniform sampler2D mytexture;\n"
"void main()\n"
"{\n"
"color = texture(mytexture, texcoord);\n"
"}\n\0";
const GLchar* fragmentShaderSourceknightchesspiece = "#version 330 core\n"
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
void blackpiecepawntop()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 view2 = glm::mat4(1.0f);
    glm::mat4 projection2 = glm::mat4(1.0f);

    model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model2 = glm::scale(model2, glm::vec3(0.07, 0.07, 0.07));
    model2 = glm::translate(model2, glm::vec3(-3.8f, 8.0f, 5.0f));
    view2 = glm::translate(view2, camerapawntoppiece);
    projection2 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc2 = glGetUniformLocation(shaderProgrampawnpiece, "model");
    unsigned int viewLoc2 = glGetUniformLocation(shaderProgrampawnpiece, "view");
    unsigned int theprojection2 = glGetUniformLocation(shaderProgrampawnpiece, "projection");

    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, &view2[0][0]);
    glUniformMatrix4fv(theprojection2, 1, GL_FALSE, &projection2[0][0]);


    glPushMatrix();
    GLUquadricObj* blacksphere = gluNewQuadric();
    glTranslatef(0.0f, 0.0f, 5.0f);
    gluQuadricDrawStyle(blacksphere, GLU_FILL);
    gluSphere(blacksphere, 0.5, 20, 20);
    gluDeleteQuadric(blacksphere);
}
void blackpiecepawnheight()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 view1 = glm::mat4(1.0f);
    glm::mat4 projection1 = glm::mat4(1.0f);

    model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model1 = glm::scale(model1, glm::vec3(0.07, 0.07, 0.07));
    model1 = glm::translate(model1, glm::vec3(-3.6f, 5.5f, 4.8f));
    view1 = glm::translate(view1, camerapawnheightpiece);
    projection1 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc1 = glGetUniformLocation(shaderProgrampawnpiece, "model");
    unsigned int viewLoc1 = glGetUniformLocation(shaderProgrampawnpiece, "view");
    unsigned int theprojection1 = glGetUniformLocation(shaderProgrampawnpiece, "projection");

    glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));
    glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, &view1[0][0]);
    glUniformMatrix4fv(theprojection1, 1, GL_FALSE, &projection1[0][0]);



    glPushMatrix();
    glRotatef(90, 1.0, 0, 0);
    glScalef(0.1f, 0.1f, 0.1f);
    GLUquadricObj* blackcylinder1 = gluNewQuadric();
    glColor3f(1, 0, 0);
    gluQuadricDrawStyle(blackcylinder1, GLU_FILL);
    gluCylinder(blackcylinder1, 0.0, 0.52, 2.0, 30, 20);
    gluDeleteQuadric(blackcylinder1);
    blackpiecepawntop();
}
void blackpiecepawn()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, glm::vec3(0.07, 0.07, 0.07));
    model = glm::translate(model, glm::vec3(0.0f, -8.0f, 0.0f));
    view = glm::translate(view, camerapawnpiece);
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc = glGetUniformLocation(shaderProgrampawnpiece, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgrampawnpiece, "view");
    unsigned int theprojection = glGetUniformLocation(shaderProgrampawnpiece, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(theprojection, 1, GL_FALSE, &projection[0][0]);



    glPushMatrix();
    GLUquadricObj* blackcylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -15.0f);
    gluQuadricDrawStyle(blackcylinderrookheight, GLU_FILL);
    gluCylinder(blackcylinderrookheight, 0.52, 0.52, 0.5, 30, 20);
    gluDeleteQuadric(blackcylinderrookheight);

    blackpiecepawnheight();
}
void blackpiecerooktop()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 view2 = glm::mat4(1.0f);
    glm::mat4 projection2 = glm::mat4(1.0f);

    model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model2 = glm::scale(model2, glm::vec3(0.09, 0.09, 0.09));
    model2 = glm::translate(model2, glm::vec3(-2.3f, 3.3f, 3.5f));
    view2 = glm::translate(view2, camerarooktoppiece);
    projection2 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc2 = glGetUniformLocation(shaderProgramrookpiece, "model");
    unsigned int viewLoc2 = glGetUniformLocation(shaderProgramrookpiece, "view");
    unsigned int theprojection2 = glGetUniformLocation(shaderProgramrookpiece, "projection");

    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, &view2[0][0]);
    glUniformMatrix4fv(theprojection2, 1, GL_FALSE, &projection2[0][0]);



    glPushMatrix();
    GLUquadricObj* blackcylinderpawn = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(blackcylinderpawn, GLU_FILL);
    gluCylinder(blackcylinderpawn, 0.7, 0.7, 0.5, 30, 20);
    gluDeleteQuadric(blackcylinderpawn);
}
void blackpiecerookheight()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 view1 = glm::mat4(1.0f);
    glm::mat4 projection1 = glm::mat4(1.0f);

    model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model1 = glm::scale(model1, glm::vec3(0.09, 0.09, 0.09));
    model1 = glm::translate(model1, glm::vec3(-2.3f, 3.3f, 4.0f));
    view1 = glm::translate(view1, camerarookheightpiece);
    projection1 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc1 = glGetUniformLocation(shaderProgramrookpiece, "model");
    unsigned int viewLoc1 = glGetUniformLocation(shaderProgramrookpiece, "view");
    unsigned int theprojection1 = glGetUniformLocation(shaderProgramrookpiece, "projection");

    glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));
    glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, &view1[0][0]);
    glUniformMatrix4fv(theprojection1, 1, GL_FALSE, &projection1[0][0]);


    glPushMatrix();
    GLUquadricObj* blackcylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(blackcylinderrookheight, GLU_FILL);
    gluCylinder(blackcylinderrookheight, 0.52, 0.52, 2.5, 30, 20);
    gluDeleteQuadric(blackcylinderrookheight);

    blackpiecerooktop();
}
void blackpiecerook()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, glm::vec3(0.09, 0.09, 0.09));
    model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));

    view = glm::translate(view, camerarookpiece);
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc = glGetUniformLocation(shaderProgramrookpiece, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgramrookpiece, "view");
    unsigned int theprojection = glGetUniformLocation(shaderProgramrookpiece, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(theprojection, 1, GL_FALSE, &projection[0][0]);


    glPushMatrix();
    GLUquadricObj* blackcylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -15.0f);
    gluQuadricDrawStyle(blackcylinderrookheight, GLU_FILL);
    gluCylinder(blackcylinderrookheight, 0.6, 0.6, 0.5, 30, 20);
    gluDeleteQuadric(blackcylinderrookheight);

    blackpiecerookheight();
}
void blackpiecekingtop()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 view2 = glm::mat4(1.0f);
    glm::mat4 projection2 = glm::mat4(1.0f);

    model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model2 = glm::scale(model2, glm::vec3(0.1, 0.1, 0.1));
    model2 = glm::translate(model2, glm::vec3(-1.8f, 2.3f, 3.0f));
    view2 = glm::translate(view2, camerakingtoppiece);
    projection2 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc2 = glGetUniformLocation(shaderProgramkingpiece, "model");
    unsigned int viewLoc2 = glGetUniformLocation(shaderProgramkingpiece, "view");
    unsigned int theprojection2 = glGetUniformLocation(shaderProgramkingpiece, "projection");

    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, &view2[0][0]);
    glUniformMatrix4fv(theprojection2, 1, GL_FALSE, &projection2[0][0]);


    glPushMatrix();
    GLUquadricObj* blackcylinderpawn = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(blackcylinderpawn, GLU_FILL);
    gluCylinder(blackcylinderpawn, 0.5, 0.7, 0.6, 30, 20);
    gluDeleteQuadric(blackcylinderpawn);
}
void blackpiecekingheight()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 view1 = glm::mat4(1.0f);
    glm::mat4 projection1 = glm::mat4(1.0f);

    model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model1 = glm::scale(model1, glm::vec3(0.1, 0.1, 0.1));
    model1 = glm::translate(model1, glm::vec3(-1.8f, 2.3f, 3.0f));
    view1 = glm::translate(view1, camerakingheightpiece);
    projection1 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc1 = glGetUniformLocation(shaderProgramkingpiece, "model");
    unsigned int viewLoc1 = glGetUniformLocation(shaderProgramkingpiece, "view");
    unsigned int theprojection1 = glGetUniformLocation(shaderProgramkingpiece, "projection");

    glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));
    glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, &view1[0][0]);
    glUniformMatrix4fv(theprojection1, 1, GL_FALSE, &projection1[0][0]);

    glPushMatrix();
    GLUquadricObj* blackcylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(blackcylinderrookheight, GLU_FILL);
    gluCylinder(blackcylinderrookheight, 0.52, 0.52, 3.0, 30, 20);
    gluDeleteQuadric(blackcylinderrookheight);

    blackpiecekingtop();
}
void blackpieceking()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
    model = glm::translate(model, glm::vec3(0.0f, -4.0f, 0.0f));
    view = glm::translate(view, camerakingpiece);
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc = glGetUniformLocation(shaderProgramkingpiece, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgramkingpiece, "view");
    unsigned int theprojection = glGetUniformLocation(shaderProgramkingpiece, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(theprojection, 1, GL_FALSE, &projection[0][0]);


    glPushMatrix();
    GLUquadricObj* blackcylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -15.0f);
    gluQuadricDrawStyle(blackcylinderrookheight, GLU_FILL);
    gluCylinder(blackcylinderrookheight, 0.6, 0.6, 0.5, 30, 20);
    gluDeleteQuadric(blackcylinderrookheight);

    blackpiecekingheight();
}
void blackpiecequeentop()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 view2 = glm::mat4(1.0f);
    glm::mat4 projection2 = glm::mat4(1.0f);

    model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model2 = glm::scale(model2, glm::vec3(0.1, 0.1, 0.1));
    //                                        z      x      y
    model2 = glm::translate(model2, glm::vec3(-1.5f, 2.7f, 2.7f));
    view2 = glm::translate(view2, cameraqueentoppiece);
    projection2 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc2 = glGetUniformLocation(shaderProgramqueenpiece, "model");
    unsigned int viewLoc2 = glGetUniformLocation(shaderProgramqueenpiece, "view");
    unsigned int theprojection2 = glGetUniformLocation(shaderProgramqueenpiece, "projection");

    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, &view2[0][0]);
    glUniformMatrix4fv(theprojection2, 1, GL_FALSE, &projection2[0][0]);


    glPushMatrix();
    GLUquadricObj* blackcylinderpawn = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(blackcylinderpawn, GLU_FILL);
    gluCylinder(blackcylinderpawn, 0.7, 0.7, 1.0, 30, 20);
    gluDeleteQuadric(blackcylinderpawn);
}
void blackpiecequeenheight()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 view1 = glm::mat4(1.0f);
    glm::mat4 projection1 = glm::mat4(1.0f);

    model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model1 = glm::scale(model1, glm::vec3(0.1, 0.1, 0.1));
    model1 = glm::translate(model1, glm::vec3(-1.5f, 2.7f, 2.2f));
    view1 = glm::translate(view1, cameraqueenheightpiece);
    projection1 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc1 = glGetUniformLocation(shaderProgramqueenpiece, "model");
    unsigned int viewLoc1 = glGetUniformLocation(shaderProgramqueenpiece, "view");
    unsigned int theprojection1 = glGetUniformLocation(shaderProgramqueenpiece, "projection");

    glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));
    glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, &view1[0][0]);
    glUniformMatrix4fv(theprojection1, 1, GL_FALSE, &projection1[0][0]);

    glPushMatrix();
    GLUquadricObj* blackcylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(blackcylinderrookheight, GLU_FILL);
    gluCylinder(blackcylinderrookheight, 0.2, 0.52, 2.0, 30, 20);
    gluDeleteQuadric(blackcylinderrookheight);

    blackpiecequeentop();
}
void blackpiecequeen()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
    model = glm::translate(model, glm::vec3(0.0f, -3.5f, 0.0f));
    view = glm::translate(view, cameraqueenpiece);
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc = glGetUniformLocation(shaderProgramqueenpiece, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgramqueenpiece, "view");
    unsigned int theprojection = glGetUniformLocation(shaderProgramqueenpiece, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(theprojection, 1, GL_FALSE, &projection[0][0]);


    glPushMatrix();
    GLUquadricObj* blackcylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -15.0f);
    gluQuadricDrawStyle(blackcylinderrookheight, GLU_FILL);
    gluCylinder(blackcylinderrookheight, 0.6, 0.6, 0.5, 30, 20);
    gluDeleteQuadric(blackcylinderrookheight);

    blackpiecequeenheight();
}
void blackpiecebishoptop()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 view2 = glm::mat4(1.0f);
    glm::mat4 projection2 = glm::mat4(1.0f);

    model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model2 = glm::scale(model2, glm::vec3(0.1, 0.1, 0.1));
    model2 = glm::translate(model2, glm::vec3(-1.4f, 2.0f, 2.0f));
    view2 = glm::translate(view2, camerabishoptoppiece);
    projection2 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc2 = glGetUniformLocation(shaderProgrambishoppiece, "model");
    unsigned int viewLoc2 = glGetUniformLocation(shaderProgrambishoppiece, "view");
    unsigned int theprojection2 = glGetUniformLocation(shaderProgrambishoppiece, "projection");

    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, &view2[0][0]);
    glUniformMatrix4fv(theprojection2, 1, GL_FALSE, &projection2[0][0]);

    glPushMatrix();
    GLUquadricObj* blackcylinderpawn = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(blackcylinderpawn, GLU_FILL);
    gluCylinder(blackcylinderpawn, 0.0, 0.6, 1.0, 30, 20);
    gluDeleteQuadric(blackcylinderpawn);
}
void blackpiecebishopheight()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 view1 = glm::mat4(1.0f);
    glm::mat4 projection1 = glm::mat4(1.0f);

    model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model1 = glm::scale(model1, glm::vec3(0.1, 0.1, 0.1));
    model1 = glm::translate(model1, glm::vec3(-1.4f, 2.0f, 2.0f));
    view1 = glm::translate(view1, camerabishopheightpiece);
    projection1 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc1 = glGetUniformLocation(shaderProgrambishoppiece, "model");
    unsigned int viewLoc1 = glGetUniformLocation(shaderProgrambishoppiece, "view");
    unsigned int theprojection1 = glGetUniformLocation(shaderProgrambishoppiece, "projection");

    glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));
    glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, &view1[0][0]);
    glUniformMatrix4fv(theprojection1, 1, GL_FALSE, &projection1[0][0]);


    glPushMatrix();
    GLUquadricObj* blackcylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(blackcylinderrookheight, GLU_FILL);
    gluCylinder(blackcylinderrookheight, 0.0, 0.52, 1.8, 30, 20);
    gluDeleteQuadric(blackcylinderrookheight);

    blackpiecebishoptop();


}
void blackpiecebishop()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    view = glm::translate(view, camerabishoppiece);
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc = glGetUniformLocation(shaderProgrambishoppiece, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgrambishoppiece, "view");
    unsigned int theprojection = glGetUniformLocation(shaderProgrambishoppiece, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(theprojection, 1, GL_FALSE, &projection[0][0]);

    glPushMatrix();
    GLUquadricObj* blackcylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -15.0f);
    gluQuadricDrawStyle(blackcylinderrookheight, GLU_FILL);
    gluCylinder(blackcylinderrookheight, 0.6, 0.6, 0.5, 30, 20);
    gluDeleteQuadric(blackcylinderrookheight);

    blackpiecebishopheight();
}
void blackpieceknighttop()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 view2 = glm::mat4(1.0f);
    glm::mat4 projection2 = glm::mat4(1.0f);

    model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model2 = glm::scale(model2, glm::vec3(0.15, 0.15, 0.15));
    model2 = glm::translate(model2, glm::vec3(-0.9f, 0.8f, 1.5f));
    view2 = glm::translate(view2, cameraknighttoppiece);
    projection2 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc2 = glGetUniformLocation(shaderProgramknightpiece, "model");
    unsigned int viewLoc2 = glGetUniformLocation(shaderProgramknightpiece, "view");
    unsigned int theprojection2 = glGetUniformLocation(shaderProgramknightpiece, "projection");

    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, &view2[0][0]);
    glUniformMatrix4fv(theprojection2, 1, GL_FALSE, &projection2[0][0]);


    glPushMatrix();
    GLUquadricObj* blacksphere = gluNewQuadric();
    glTranslatef(0.0f, 0.0f, 5.0f);
    gluQuadricDrawStyle(blacksphere, GLU_FILL);
    gluSphere(blacksphere, 0.5, 20, 20);
    gluDeleteQuadric(blacksphere);
}
void blackpieceknightheight()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 view1 = glm::mat4(1.0f);
    glm::mat4 projection1 = glm::mat4(1.0f);

    model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model1 = glm::scale(model1, glm::vec3(0.1, 0.1, 0.1));
    model1 = glm::translate(model1, glm::vec3(-1.3f, 1.0f, 1.8f));
    view1 = glm::translate(view1, cameraknightheightpiece);
    projection1 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc1 = glGetUniformLocation(shaderProgramknightpiece, "model");
    unsigned int viewLoc1 = glGetUniformLocation(shaderProgramknightpiece, "view");
    unsigned int theprojection1 = glGetUniformLocation(shaderProgramknightpiece, "projection");

    glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));
    glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, &view1[0][0]);
    glUniformMatrix4fv(theprojection1, 1, GL_FALSE, &projection1[0][0]);


    glPushMatrix();
    GLUquadricObj* blackcylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(blackcylinderrookheight, GLU_FILL);
    gluCylinder(blackcylinderrookheight, 0.52, 0.52, 1.3, 30, 20);
    gluDeleteQuadric(blackcylinderrookheight);

    blackpieceknighttop();
}
void blackpieceknight()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
    model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
    view = glm::translate(view, cameraknightpiece);
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc = glGetUniformLocation(shaderProgramknightpiece, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgramknightpiece, "view");
    unsigned int theprojection = glGetUniformLocation(shaderProgramknightpiece, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(theprojection, 1, GL_FALSE, &projection[0][0]);


    glPushMatrix();
    GLUquadricObj* blackcylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -15.0f);
    gluQuadricDrawStyle(blackcylinderrookheight, GLU_FILL);
    gluCylinder(blackcylinderrookheight, 0.6, 0.6, 0.5, 30, 20);
    gluDeleteQuadric(blackcylinderrookheight);

    blackpieceknightheight();
}
void whitespherepawn()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 view2 = glm::mat4(1.0f);
    glm::mat4 projection2 = glm::mat4(1.0f);

    model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model2 = glm::scale(model2, glm::vec3(0.07, 0.07, 0.07));
    model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, -2.0f));
    view2 = glm::translate(view2, camerapawntoppiece);
    projection2 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc2 = glGetUniformLocation(shaderProgrampawnpiece, "model");
    unsigned int viewLoc2 = glGetUniformLocation(shaderProgrampawnpiece, "view");
    unsigned int theprojection2 = glGetUniformLocation(shaderProgrampawnpiece, "projection");

    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, &view2[0][0]);
    glUniformMatrix4fv(theprojection2, 1, GL_FALSE, &projection2[0][0]);


    glPushMatrix();
    GLUquadricObj* whitesphere = gluNewQuadric();
    glTranslatef(0.0f, 0.0f, 5.0f);
    gluQuadricDrawStyle(whitesphere, GLU_FILL);
    gluSphere(whitesphere, 0.5, 20, 20);
    gluDeleteQuadric(whitesphere);






}
void whitecylinderflatpawn()
{
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 view1 = glm::mat4(1.0f);
    glm::mat4 projection1 = glm::mat4(1.0f);

    model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model1 = glm::scale(model1, glm::vec3(0.07, 0.07, 0.07));
    model1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, -2.0f));
    view1 = glm::translate(view1, camerapawnheightpiece);
    projection1 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc1 = glGetUniformLocation(shaderProgrampawnpiece, "model");
    unsigned int viewLoc1 = glGetUniformLocation(shaderProgrampawnpiece, "view");
    unsigned int theprojection1 = glGetUniformLocation(shaderProgrampawnpiece, "projection");

    glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));
    glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, &view1[0][0]);
    glUniformMatrix4fv(theprojection1, 1, GL_FALSE, &projection1[0][0]);

    glPushMatrix();
    glRotatef(90, 1.0, 0, 0);
    glScalef(0.1f, 0.1f, 0.1f);
    GLUquadricObj* whitecylinder1 = gluNewQuadric();
    glColor3f(1, 0, 0);
    gluQuadricDrawStyle(whitecylinder1, GLU_FILL);
    gluCylinder(whitecylinder1, 0.0, 0.52, 2.0, 30, 20);
    gluDeleteQuadric(whitecylinder1);


    whitespherepawn();





}
void whitecylinderpawn()
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, glm::vec3(0.07, 0.07, 0.07));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
    view = glm::translate(view, camerapawnpiece);
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc = glGetUniformLocation(shaderProgrampawnpiece, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgrampawnpiece, "view");
    unsigned int theprojection = glGetUniformLocation(shaderProgrampawnpiece, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(theprojection, 1, GL_FALSE, &projection[0][0]);


    glPushMatrix();
    GLUquadricObj* whitecylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);

    glTranslatef(0.0f, 0.0f, -15.0f);
    gluQuadricDrawStyle(whitecylinderrookheight, GLU_FILL);
    gluCylinder(whitecylinderrookheight, 0.52, 0.52, 0.5, 30, 20);
    gluDeleteQuadric(whitecylinderrookheight);

    whitecylinderflatpawn();





}
void whitecylinderrooktop()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 view2 = glm::mat4(1.0f);
    glm::mat4 projection2 = glm::mat4(1.0f);

    model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model2 = glm::scale(model2, glm::vec3(0.09, 0.09, 0.09));
    model2 = glm::translate(model2, glm::vec3(0.5f, 2.0f, 0.0f));
    view2 = glm::translate(view2, camerarooktoppiece);
    projection2 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc2 = glGetUniformLocation(shaderProgramrookpiece, "model");
    unsigned int viewLoc2 = glGetUniformLocation(shaderProgramrookpiece, "view");
    unsigned int theprojection2 = glGetUniformLocation(shaderProgramrookpiece, "projection");

    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, &view2[0][0]);
    glUniformMatrix4fv(theprojection2, 1, GL_FALSE, &projection2[0][0]);


    glPushMatrix();
    GLUquadricObj* whitecylinderpawn = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(whitecylinderpawn, GLU_FILL);
    gluCylinder(whitecylinderpawn, 0.7, 0.7, 0.5, 30, 20);
    gluDeleteQuadric(whitecylinderpawn);







}
void whitecylinderheightrook()
{
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 view1 = glm::mat4(1.0f);
    glm::mat4 projection1 = glm::mat4(1.0f);

    model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model1 = glm::scale(model1, glm::vec3(0.09, 0.09, 0.09));
    model1 = glm::translate(model1, glm::vec3(1.0f, -1.0f, 0.0f));
    view1 = glm::translate(view1, camerarookheightpiece);
    projection1 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc1 = glGetUniformLocation(shaderProgramrookpiece, "model");
    unsigned int viewLoc1 = glGetUniformLocation(shaderProgramrookpiece, "view");
    unsigned int theprojection1 = glGetUniformLocation(shaderProgramrookpiece, "projection");

    glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));
    glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, &view1[0][0]);
    glUniformMatrix4fv(theprojection1, 1, GL_FALSE, &projection1[0][0]);


    glPushMatrix();
    GLUquadricObj* whitecylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(whitecylinderrookheight, GLU_FILL);
    gluCylinder(whitecylinderrookheight, 0.52, 0.52, 2.5, 30, 20);
    gluDeleteQuadric(whitecylinderrookheight);

    whitecylinderrooktop();






}
void whitecylinderrook()
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, glm::vec3(0.09, 0.09, 0.09));
    model = glm::translate(model, glm::vec3(0.4f, 1.3f, 0.0f));

    view = glm::translate(view, camerarookpiece);
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc = glGetUniformLocation(shaderProgramrookpiece, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgramrookpiece, "view");
    unsigned int theprojection = glGetUniformLocation(shaderProgramrookpiece, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(theprojection, 1, GL_FALSE, &projection[0][0]);


    glPushMatrix();
    GLUquadricObj* whitecylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -15.0f);
    gluQuadricDrawStyle(whitecylinderrookheight, GLU_FILL);
    gluCylinder(whitecylinderrookheight, 0.6, 0.6, 0.5, 30, 20);
    gluDeleteQuadric(whitecylinderrookheight);

    whitecylinderheightrook();





}
void whitecylinderkingtop()
{
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 view2 = glm::mat4(1.0f);
    glm::mat4 projection2 = glm::mat4(1.0f);

    model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model2 = glm::scale(model2, glm::vec3(0.1, 0.1, 0.1));
    model2 = glm::translate(model2, glm::vec3(1.2f, -2.0f, -1.0f));
    view2 = glm::translate(view2, camerakingtoppiece);
    projection2 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);


    unsigned int modelLoc2 = glGetUniformLocation(shaderProgramkingpiece, "model");
    unsigned int viewLoc2 = glGetUniformLocation(shaderProgramkingpiece, "view");
    unsigned int theprojection2 = glGetUniformLocation(shaderProgramkingpiece, "projection");

    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, &view2[0][0]);
    glUniformMatrix4fv(theprojection2, 1, GL_FALSE, &projection2[0][0]);


    glPushMatrix();
    GLUquadricObj* whitecylinderpawn = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(whitecylinderpawn, GLU_FILL);
    gluCylinder(whitecylinderpawn, 0.5, 0.7, 0.6, 30, 20);
    gluDeleteQuadric(whitecylinderpawn);

}
void whitecylinderheightking()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 view1 = glm::mat4(1.0f);
    glm::mat4 projection1 = glm::mat4(1.0f);

    model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model1 = glm::scale(model1, glm::vec3(0.1, 0.1, 0.1));
    model1 = glm::translate(model1, glm::vec3(1.2f, -2.0f, -1.0f));
    view1 = glm::translate(view1, camerakingheightpiece);
    projection1 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc1 = glGetUniformLocation(shaderProgramkingpiece, "model");
    unsigned int viewLoc1 = glGetUniformLocation(shaderProgramkingpiece, "view");
    unsigned int theprojection1 = glGetUniformLocation(shaderProgramkingpiece, "projection");

    glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));
    glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, &view1[0][0]);
    glUniformMatrix4fv(theprojection1, 1, GL_FALSE, &projection1[0][0]);


    glPushMatrix();
    GLUquadricObj* whitecylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(whitecylinderrookheight, GLU_FILL);
    gluCylinder(whitecylinderrookheight, 0.52, 0.52, 3.0, 30, 20);
    gluDeleteQuadric(whitecylinderrookheight);

    whitecylinderkingtop();






}

void whitecylinderking()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
    model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
    view = glm::translate(view, camerakingpiece);
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc = glGetUniformLocation(shaderProgramkingpiece, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgramkingpiece, "view");
    unsigned int theprojection = glGetUniformLocation(shaderProgramkingpiece, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(theprojection, 1, GL_FALSE, &projection[0][0]);


    glPushMatrix();
    GLUquadricObj* whitecylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -15.0f);
    gluQuadricDrawStyle(whitecylinderrookheight, GLU_FILL);
    gluCylinder(whitecylinderrookheight, 0.6, 0.6, 0.5, 30, 20);
    gluDeleteQuadric(whitecylinderrookheight);

    whitecylinderheightking();
}
void whitecylinderqueentop()
{
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 view2 = glm::mat4(1.0f);
    glm::mat4 projection2 = glm::mat4(1.0f);

    model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model2 = glm::scale(model2, glm::vec3(0.1, 0.1, 0.1));
    //                                        z      x      y
    model2 = glm::translate(model2, glm::vec3(1.3f, -2.0f, -1.2f));
    view2 = glm::translate(view2, cameraqueentoppiece);
    projection2 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc2 = glGetUniformLocation(shaderProgramqueenpiece, "model");
    unsigned int viewLoc2 = glGetUniformLocation(shaderProgramqueenpiece, "view");
    unsigned int theprojection2 = glGetUniformLocation(shaderProgramqueenpiece, "projection");

    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, &view2[0][0]);
    glUniformMatrix4fv(theprojection2, 1, GL_FALSE, &projection2[0][0]);


    glPushMatrix();
    GLUquadricObj* whitecylinderpawn = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(whitecylinderpawn, GLU_FILL);
    gluCylinder(whitecylinderpawn, 0.7, 0.7, 1.0, 30, 20);
    gluDeleteQuadric(whitecylinderpawn);







}
void whitecylinderheightqueen()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 view1 = glm::mat4(1.0f);
    glm::mat4 projection1 = glm::mat4(1.0f);

    model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model1 = glm::scale(model1, glm::vec3(0.1, 0.1, 0.1));
    model1 = glm::translate(model1, glm::vec3(1.3f, -2.0f, -1.5f));
    view1 = glm::translate(view1, cameraqueenheightpiece);
    projection1 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc1 = glGetUniformLocation(shaderProgramqueenpiece, "model");
    unsigned int viewLoc1 = glGetUniformLocation(shaderProgramqueenpiece, "view");
    unsigned int theprojection1 = glGetUniformLocation(shaderProgramqueenpiece, "projection");

    glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));
    glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, &view1[0][0]);
    glUniformMatrix4fv(theprojection1, 1, GL_FALSE, &projection1[0][0]);


    glPushMatrix();
    GLUquadricObj* whitecylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(whitecylinderrookheight, GLU_FILL);
    gluCylinder(whitecylinderrookheight, 0.2, 0.52, 2.0, 30, 20);
    gluDeleteQuadric(whitecylinderrookheight);

    whitecylinderqueentop();
}

void whitecylinderqueen()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
    model = glm::translate(model, glm::vec3(0.0f, 2.8f, 0.0f));
    view = glm::translate(view, cameraqueenpiece);
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc = glGetUniformLocation(shaderProgramqueenpiece, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgramqueenpiece, "view");
    unsigned int theprojection = glGetUniformLocation(shaderProgramqueenpiece, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(theprojection, 1, GL_FALSE, &projection[0][0]);


    glPushMatrix();
    GLUquadricObj* whitecylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -15.0f);
    gluQuadricDrawStyle(whitecylinderrookheight, GLU_FILL);
    gluCylinder(whitecylinderrookheight, 0.6, 0.6, 0.5, 30, 20);
    gluDeleteQuadric(whitecylinderrookheight);

    whitecylinderheightqueen();

}
void whitecylinderbishoptop()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 view2 = glm::mat4(1.0f);
    glm::mat4 projection2 = glm::mat4(1.0f);

    model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model2 = glm::scale(model2, glm::vec3(0.1, 0.1, 0.1));
    model2 = glm::translate(model2, glm::vec3(1.3f, -3.5f, -2.5f));
    view2 = glm::translate(view2, camerabishoptoppiece);
    projection2 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc2 = glGetUniformLocation(shaderProgrambishoppiece, "model");
    unsigned int viewLoc2 = glGetUniformLocation(shaderProgrambishoppiece, "view");
    unsigned int theprojection2 = glGetUniformLocation(shaderProgrambishoppiece, "projection");

    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, &view2[0][0]);
    glUniformMatrix4fv(theprojection2, 1, GL_FALSE, &projection2[0][0]);

    glPushMatrix();
    GLUquadricObj* whitecylinderpawn = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(whitecylinderpawn, GLU_FILL);
    gluCylinder(whitecylinderpawn, 0.0, 0.6, 1.0, 30, 20);
    gluDeleteQuadric(whitecylinderpawn);
}
void whitecylinderheightbishop()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 view1 = glm::mat4(1.0f);
    glm::mat4 projection1 = glm::mat4(1.0f);

    model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model1 = glm::scale(model1, glm::vec3(0.1, 0.1, 0.1));
    model1 = glm::translate(model1, glm::vec3(1.3f, -3.5f, -2.5f));
    view1 = glm::translate(view1, camerabishopheightpiece);
    projection1 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc1 = glGetUniformLocation(shaderProgrambishoppiece, "model");
    unsigned int viewLoc1 = glGetUniformLocation(shaderProgrambishoppiece, "view");
    unsigned int theprojection1 = glGetUniformLocation(shaderProgrambishoppiece, "projection");

    glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));
    glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, &view1[0][0]);
    glUniformMatrix4fv(theprojection1, 1, GL_FALSE, &projection1[0][0]);


    glPushMatrix();
    GLUquadricObj* whitecylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(whitecylinderrookheight, GLU_FILL);
    gluCylinder(whitecylinderrookheight, 0.0, 0.52, 1.8, 30, 20);
    gluDeleteQuadric(whitecylinderrookheight);

    whitecylinderbishoptop();
}
void whitecylinderbishop()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
    model = glm::translate(model, glm::vec3(-0.5f, 4.0f, 0.0f));
    view = glm::translate(view, camerabishoppiece);
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc = glGetUniformLocation(shaderProgrambishoppiece, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgrambishoppiece, "view");
    unsigned int theprojection = glGetUniformLocation(shaderProgrambishoppiece, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(theprojection, 1, GL_FALSE, &projection[0][0]);


    glPushMatrix();
    GLUquadricObj* whitecylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -15.0f);
    gluQuadricDrawStyle(whitecylinderrookheight, GLU_FILL);
    gluCylinder(whitecylinderrookheight, 0.6, 0.6, 0.5, 30, 20);
    gluDeleteQuadric(whitecylinderrookheight);

    whitecylinderheightbishop();

}
void whitecylinderknighttop()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 view2 = glm::mat4(1.0f);
    glm::mat4 projection2 = glm::mat4(1.0f);

    model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model2 = glm::scale(model2, glm::vec3(0.15, 0.15, 0.15));
    model2 = glm::translate(model2, glm::vec3(0.8f, -0.5f, -1.2f));
    view2 = glm::translate(view2, cameraknighttoppiece);

    projection2 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc2 = glGetUniformLocation(shaderProgramknightpiece, "model");
    unsigned int viewLoc2 = glGetUniformLocation(shaderProgramknightpiece, "view");
    unsigned int theprojection2 = glGetUniformLocation(shaderProgramknightpiece, "projection");

    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model2));
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, &view2[0][0]);
    glUniformMatrix4fv(theprojection2, 1, GL_FALSE, &projection2[0][0]);


    glPushMatrix();
    GLUquadricObj* whitesphere = gluNewQuadric();
    glTranslatef(0.0f, 0.0f, 5.0f);
    gluQuadricDrawStyle(whitesphere, GLU_FILL);
    gluSphere(whitesphere, 0.5, 20, 20);
    gluDeleteQuadric(whitesphere);
}
void whitecylinderheightknight()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 view1 = glm::mat4(1.0f);
    glm::mat4 projection1 = glm::mat4(1.0f);

    model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model1 = glm::scale(model1, glm::vec3(0.1, 0.1, 0.1));
    model1 = glm::translate(model1, glm::vec3(1.1f, -3.0f, -2.0f));
    view1 = glm::translate(view1, cameraknightheightpiece);
    projection1 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc1 = glGetUniformLocation(shaderProgramknightpiece, "model");
    unsigned int viewLoc1 = glGetUniformLocation(shaderProgramknightpiece, "view");
    unsigned int theprojection1 = glGetUniformLocation(shaderProgramknightpiece, "projection");

    glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model1));
    glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, &view1[0][0]);
    glUniformMatrix4fv(theprojection1, 1, GL_FALSE, &projection1[0][0]);


    glPushMatrix();
    GLUquadricObj* whitecylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -10.0f);
    gluQuadricDrawStyle(whitecylinderrookheight, GLU_FILL);
    gluCylinder(whitecylinderrookheight, 0.52, 0.52, 1.3, 30, 20);
    gluDeleteQuadric(whitecylinderrookheight);

    whitecylinderknighttop();
}
void whitecylinderknight()
{
    //Creating and positioning the black pawn chess piece 
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
    model = glm::translate(model, glm::vec3(-0.5f, 3.5f, 0.0f));
    view = glm::translate(view, cameraknightpiece);

    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



    unsigned int modelLoc = glGetUniformLocation(shaderProgramknightpiece, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgramknightpiece, "view");
    unsigned int theprojection = glGetUniformLocation(shaderProgramknightpiece, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(theprojection, 1, GL_FALSE, &projection[0][0]);


    glPushMatrix();
    GLUquadricObj* whitecylinderrookheight = gluNewQuadric();
    glColor3f(1, 0.5, 0);
    glTranslatef(0.0f, 0.0f, -15.0f);
    gluQuadricDrawStyle(whitecylinderrookheight, GLU_FILL);
    gluCylinder(whitecylinderrookheight, 0.6, 0.6, 0.5, 30, 20);
    gluDeleteQuadric(whitecylinderrookheight);

    whitecylinderheightknight();

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

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, mouse_callback);

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
    GLuint vertexShaderlight = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderlight, 1, &vertexShaderSourcelighting, NULL);
    glCompileShader(vertexShaderlight);
    GLuint vertexShadercube = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShadercube, 1, &vertexShaderSourcecubelight, NULL);
    glCompileShader(vertexShadercube);

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
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint fragmentShadertexture = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint fragmentShaderpawntexture = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint fragmentShaderrooktexture = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint fragmentShaderkingtexture = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint fragmentShaderqueentexture = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint fragmentShaderbishoptexture = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint fragmentShaderknighttexture = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint fragmentShaderlighting = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint fragmentShadercubelight = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShadertexture, 1, &fragmentShaderSourcetexturechess, NULL);
    glCompileShader(fragmentShadertexture);
    //glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //glCompileShader(fragmentShader);
    glShaderSource(fragmentShaderpawntexture, 1, &fragmentShaderSourcepawnchesspiece, NULL);
    glCompileShader(fragmentShaderpawntexture);
    glShaderSource(fragmentShaderrooktexture, 1, &fragmentShaderSourcerookchesspiece, NULL);
    glCompileShader(fragmentShaderrooktexture);
    glShaderSource(fragmentShaderkingtexture, 1, &fragmentShaderSourcekingchesspiece, NULL);
    glCompileShader(fragmentShaderkingtexture);
    glShaderSource(fragmentShaderqueentexture, 1, &fragmentShaderSourcequeenchesspiece, NULL);
    glCompileShader(fragmentShaderqueentexture);
    glShaderSource(fragmentShaderbishoptexture, 1, &fragmentShaderSourcebishopchesspiece, NULL);
    glCompileShader(fragmentShaderbishoptexture);
    glShaderSource(fragmentShaderknighttexture, 1, &fragmentShaderSourceknightchesspiece, NULL);
    glCompileShader(fragmentShaderknighttexture);
    glShaderSource(fragmentShaderlighting, 1, &fragmentShaderSourcecombinelight, NULL);
    glCompileShader(fragmentShaderlighting);
    glShaderSource(fragmentShadercubelight, 1, &fragmentShaderSourcecubelight, NULL);
    glCompileShader(fragmentShadercubelight);

    // Check for compile time errors
    glGetShaderiv(fragmentShaderlighting, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderlighting, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    shaderProgramlighting = glCreateProgram();
    glAttachShader(shaderProgramlighting, vertexShaderlight);
    glAttachShader(shaderProgramlighting, fragmentShaderlighting);
    glLinkProgram(shaderProgramlighting);

    shaderProgramlightingcube = glCreateProgram();
    glAttachShader(shaderProgramlightingcube, vertexShadercube);
    glAttachShader(shaderProgramlightingcube, fragmentShadercubelight);
    glLinkProgram(shaderProgramlightingcube);

    shaderProgramtexture = glCreateProgram();
    glAttachShader(shaderProgramtexture, vertexShader);
    glAttachShader(shaderProgramtexture, fragmentShadertexture);
    glLinkProgram(shaderProgramtexture);

    shaderProgrampawnpiece = glCreateProgram();
    glAttachShader(shaderProgrampawnpiece, vertexShader);
    glAttachShader(shaderProgrampawnpiece, fragmentShaderpawntexture);
    glLinkProgram(shaderProgrampawnpiece);

    shaderProgramrookpiece = glCreateProgram();
    glAttachShader(shaderProgramrookpiece, vertexShader);
    glAttachShader(shaderProgramrookpiece, fragmentShaderrooktexture);
    glLinkProgram(shaderProgramrookpiece);

    shaderProgramkingpiece = glCreateProgram();
    glAttachShader(shaderProgramkingpiece, vertexShader);
    glAttachShader(shaderProgramkingpiece, fragmentShaderkingtexture);
    glLinkProgram(shaderProgramkingpiece);

    shaderProgramqueenpiece = glCreateProgram();
    glAttachShader(shaderProgramqueenpiece, vertexShader);
    glAttachShader(shaderProgramqueenpiece, fragmentShaderqueentexture);
    glLinkProgram(shaderProgramqueenpiece);

    shaderProgrambishoppiece = glCreateProgram();
    glAttachShader(shaderProgrambishoppiece, vertexShader);
    glAttachShader(shaderProgrambishoppiece, fragmentShaderbishoptexture);
    glLinkProgram(shaderProgrambishoppiece);

    shaderProgramknightpiece = glCreateProgram();
    glAttachShader(shaderProgramknightpiece, vertexShader);
    glAttachShader(shaderProgramknightpiece, fragmentShaderknighttexture);
    glLinkProgram(shaderProgramknightpiece);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShadertexture);
    glDeleteShader(fragmentShaderpawntexture);
    glDeleteShader(fragmentShaderrooktexture);
    glDeleteShader(fragmentShaderkingtexture);
    glDeleteShader(fragmentShaderqueentexture);
    glDeleteShader(fragmentShaderbishoptexture);
    glDeleteShader(fragmentShaderknighttexture);


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

    glGenTextures(1, &textureblock4);
    glBindTexture(GL_TEXTURE_2D, textureblock4);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width3, height3, nrChannels3;
    unsigned char* data3 = stbi_load("res/images/brown-wall-texture_1048-4780.jpg", &width3, &height3,
        &nrChannels3, 0);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width3, height3, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data3);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data3);

    glGenTextures(1, &textureblock5);
    glBindTexture(GL_TEXTURE_2D, textureblock5);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width4, height4, nrChannels4;
    unsigned char* data4 = stbi_load("res/images/Wood.jpg", &width4, &height4,
        &nrChannels4, 0);
    if (data4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width4, height4, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data4);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data4);

    
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

    
    
    //CALLING THE LIGHT SHADERS
    Shader lightingShaderobjects("res/shaders/lightingobjects.vs", "res/shaders/lightingobjects.frag");
    Shader cubeshaderobject("res/shaders/lightingcube.vs", "res/shaders/lightingcube.frag");
    
    Shader modelShader("res/shaders/models.vs", "res/shaders/models.frag");

    //LOADING THE MODELS
    Model MyModels("res/models2/chessss.obj");
   
    
    float verticescubelight[] = {
        -0.5f, -0.5f, -0.5f,      0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,     1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,      1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,     1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,     0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,     0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,     0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,      1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,    1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f,   1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,      1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,      0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,      0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,     0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,      1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,   0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,   1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,   1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f
    };

    glm::vec3 firstcubePositions[] = {
        glm::vec3(-5.0f, 0.0f, 3.0f),//dir -left
        glm::vec3(5.0f, 0.0f, 3.0f),//point - right
        glm::vec3(0.0f, 0.0f, 0.0f),//spot - middle

    };
    glm::vec3 secondcubePosition[] = {
        glm::vec3(-5.0f, 0.0f, 4.0f),//dir -left
        glm::vec3(5.0f, 5.0f, 6.0f),//point - right
        glm::vec3(0.0f, 4.0f, 1.0f),//spot - middle

    };
    glm::vec3 thirdcubePosition[] = {
        glm::vec3(0.5f, 1.5f, -1.5f),//point - right
        glm::vec3(0.0f, 1.0f, 1.0f),//spot - middle

    };

    GLuint VBOcube, VAOcube;
    glGenBuffers(1, &VBOcube);
    glGenVertexArrays(1, &VAOcube);
    glBindVertexArray(VAOcube);

    glBindBuffer(GL_ARRAY_BUFFER, VBOcube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticescubelight), verticescubelight, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgramlighting);
    
    int diffusemap = glGetUniformLocation(shaderProgramlighting, "material.diffuse");
    glUniform1i(diffusemap, 0);
    int specularmap = glGetUniformLocation(shaderProgramlighting, "material.specular");
    glUniform1i(specularmap, 1);
    
    

   





    ///Height map
    
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //process input
        moving();


        // Render
        // Clear the colorbuffer
        //glClearColor specifies the color on the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        elapsed_time += deltaTime;
        ++frame_counter;

        if (elapsed_time >= 1) {

            fps = (int)(((float)frame_counter * 60.0f) / (elapsed_time * 100));

            frame_counter = 0;
            elapsed_time = 0.0f;
        }

       


        //Generate Terrain

       

        //activating the light shader for the models
        lightingShaderobjects.use();
       
        glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);
        glm::vec4 lightingColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        
        
        int ColorLocationobjectviewfirst = glGetUniformLocation(lightingShaderobjects.ID, "viewPos");
        glUniform3f(ColorLocationobjectviewfirst, 0.0f, 0.0f, 0.0f);
        int shinymaterial = glGetUniformLocation(lightingShaderobjects.ID, "material.shininess");
        glUniform1f(shinymaterial, 32.0f);
        //directional light
        glUniform3f(glGetUniformLocation(lightingShaderobjects.ID, "dirLight.direction"), 5.0f, 5.0f, 6.0f);
        glUniform3f(glGetUniformLocation(lightingShaderobjects.ID, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShaderobjects.ID, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShaderobjects.ID, "dirLight.specular"), 1.0f, 1.0f, 1.0f);
        //point light 2m behind, by the left side
        glUniform3f(glGetUniformLocation(lightingShaderobjects.ID, "pointLights[0].position"), -5.0f, 0.0f, 4.0f);
        glUniform3f(glGetUniformLocation(lightingShaderobjects.ID, "pointLights[0].ambient"), 2.8f, 2.8f, 2.8f);
        glUniform3f(glGetUniformLocation(lightingShaderobjects.ID, "pointLights[0].diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShaderobjects.ID, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShaderobjects.ID, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShaderobjects.ID, "pointLights[0].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShaderobjects.ID, "pointLights[0].quadratic"), 0.032f);
        //spot light by the model
        glUniform3f(glGetUniformLocation(lightingShaderobjects.ID, "spotLight.position"), 0.0f, 4.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShaderobjects.ID, "spotLight.direction"), 0.0f, -0.8f, -1.0f);
        glUniform3f(glGetUniformLocation(lightingShaderobjects.ID, "spotLight.ambient"), 7.8f, 7.8f, 7.8f);
        glUniform3f(glGetUniformLocation(lightingShaderobjects.ID, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShaderobjects.ID, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShaderobjects.ID, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShaderobjects.ID, "spotLight.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShaderobjects.ID, "spotLight.quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(lightingShaderobjects.ID, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(lightingShaderobjects.ID, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

        //placing the models
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShaderobjects.setMat4("projection", projection);
        lightingShaderobjects.setMat4("view", view);
       
        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, -3.0f)); 
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	
        lightingShaderobjects.setMat4("model", model);
        MyModels.Draw(lightingShaderobjects);

       

        
        //activating the shader for light cube positions
        cubeshaderobject.use();
        glm::mat4 model3 = glm::mat4(1.0f); 
        glm::mat4 view3 = glm::mat4(1.0f);
        glm::mat4 projection3 = glm::mat4(1.0f);
        model3 = glm::rotate(model3, glm::radians(-55.0f), glm::vec3(0.5f, 0.5f, 0.5f));
        model3 = glm::translate(model3, glm::vec3(5.0f, 5.0f, 6.0f));//x is left/right, y is forward/back, z is up/down
        model3 = glm::scale(model3, glm::vec3(0.2f));
        view3 = camera.GetViewMatrix();
        projection3 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc3 = glGetUniformLocation(cubeshaderobject.ID, "model");
        unsigned int viewLoc3 = glGetUniformLocation(cubeshaderobject.ID, "view");
        unsigned int theprojection3 = glGetUniformLocation(cubeshaderobject.ID, "projection");

        glUniformMatrix4fv(modelLoc3, 1, GL_FALSE, glm::value_ptr(model3));
        glUniformMatrix4fv(viewLoc3, 1, GL_FALSE, &view3[0][0]);
        glUniformMatrix4fv(theprojection3, 1, GL_FALSE, &projection3[0][0]);

        glBindVertexArray(VAOcube);
        
        for (GLuint i = 0; i < 4; i++)
        {
            model3 = glm::mat4();
            model3 = glm::translate(model3, secondcubePosition[i]);
            model3 = glm::scale(model3, glm::vec3(0.2f)); 
            glUniformMatrix4fv(modelLoc3, 1, GL_FALSE, glm::value_ptr(model3));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //activating the light shader
        glUseProgram(shaderProgramlighting);
        float x = -1.0f;
        float y = -1.0f;
        float z = -1.0f;
        int normal = glGetUniformLocation(shaderProgramlighting, "aNormal");
        while (x < 1.0f && y < 1.0f && z < 1.0f)
        {
            x += glfwGetTime() * deltaTime;
            y += glfwGetTime() * deltaTime;
            z += glfwGetTime() * deltaTime;

            glUniform3f(normal, x, y, z);
        }
        int ColorLocationobjectviewsecond = glGetUniformLocation(shaderProgramlighting, "viewPos");
        glUniform3f(ColorLocationobjectviewsecond, 0.0f, 0.0f, 0.0f);
        int shinymaterial1 = glGetUniformLocation(shaderProgramlighting, "material.shininess");
        glUniform1f(shinymaterial1, 32.0f);
        //point light 
        glUniform3f(glGetUniformLocation(shaderProgramlighting, "pointLights[0].position"), 0.5f, 1.5f, -1.5f);
        glUniform3f(glGetUniformLocation(shaderProgramlighting, "pointLights[0].ambient"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(shaderProgramlighting, "pointLights[0].diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(shaderProgramlighting, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgramlighting, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgramlighting, "pointLights[0].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(shaderProgramlighting, "pointLights[0].quadratic"), 0.032f);
        //spot light on the chessboard
        glUniform3f(glGetUniformLocation(shaderProgramlighting, "spotLight.position"), 0.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shaderProgramlighting, "spotLight.direction"), 0.0f, -1.0f, -1.0f);
        glUniform3f(glGetUniformLocation(shaderProgramlighting, "spotLight.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(shaderProgramlighting, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shaderProgramlighting, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgramlighting, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgramlighting, "spotLight.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(shaderProgramlighting, "spotLight.quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(shaderProgramlighting, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(shaderProgramlighting, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

        for (int i = 0; i < 81; i++)
        {
            if (i == 0 || i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 20)
            {
                
                glm::mat4 model4 = glm::mat4(1.0f); 
                glm::mat4 view4 = glm::mat4(1.0f);
                glm::mat4 projection4 = glm::mat4(1.0f);
                float radius = 10.0f;
                float camX = static_cast<float>(sin(glfwGetTime()) * radius);
                float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
                view4 = camera.GetViewMatrix();
                model4 = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
                
                projection4 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

                
                unsigned int modelLoc4 = glGetUniformLocation(shaderProgramlighting, "model");
                unsigned int viewLoc4 = glGetUniformLocation(shaderProgramlighting, "view");
                unsigned int theprojection4 = glGetUniformLocation(shaderProgramlighting, "projection");

                glUniformMatrix4fv(modelLoc4, 1, GL_FALSE, glm::value_ptr(model4));
                glUniformMatrix4fv(viewLoc4, 1, GL_FALSE, &view4[0][0]);
                glUniformMatrix4fv(theprojection4, 1, GL_FALSE, &projection4[0][0]);
               
                glBindTexture(GL_TEXTURE_2D, textureblock2);
                glBindVertexArray(VAO[i]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 22 || i == 24 || i == 26 || i == 28 || i == 30 || i == 32 || i == 34 || i == 36 || i == 38 || i == 40 || i == 42)
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

                
                unsigned int modelLoc4 = glGetUniformLocation(shaderProgramlighting, "model");
                unsigned int viewLoc4 = glGetUniformLocation(shaderProgramlighting, "view");
                unsigned int theprojection4 = glGetUniformLocation(shaderProgramlighting, "projection");

                glUniformMatrix4fv(modelLoc4, 1, GL_FALSE, glm::value_ptr(model4));
                glUniformMatrix4fv(viewLoc4, 1, GL_FALSE, &view4[0][0]);
                glUniformMatrix4fv(theprojection4, 1, GL_FALSE, &projection4[0][0]);
                
                glBindTexture(GL_TEXTURE_2D, textureblock2);
                glBindVertexArray(VAO[i]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 44 || i == 46 || i == 48 || i == 50 || i == 52 || i == 54 || i == 56 || i == 58 || i == 60 || i == 62 || i == 64)
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

               
                unsigned int modelLoc4 = glGetUniformLocation(shaderProgramlighting, "model");
                unsigned int viewLoc4 = glGetUniformLocation(shaderProgramlighting, "view");
                unsigned int theprojection4 = glGetUniformLocation(shaderProgramlighting, "projection");

                glUniformMatrix4fv(modelLoc4, 1, GL_FALSE, glm::value_ptr(model4));
                glUniformMatrix4fv(viewLoc4, 1, GL_FALSE, &view4[0][0]);
                glUniformMatrix4fv(theprojection4, 1, GL_FALSE, &projection4[0][0]);
              
                glBindTexture(GL_TEXTURE_2D, textureblock2);
                glBindVertexArray(VAO[i]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 66 || i == 68 || i == 70 || i == 72 || i == 74 || i == 76 || i == 78 || i == 80)
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

                
                unsigned int modelLoc4 = glGetUniformLocation(shaderProgramlighting, "model");
                unsigned int viewLoc4 = glGetUniformLocation(shaderProgramlighting, "view");
                unsigned int theprojection4 = glGetUniformLocation(shaderProgramlighting, "projection");

                glUniformMatrix4fv(modelLoc4, 1, GL_FALSE, glm::value_ptr(model4));
                glUniformMatrix4fv(viewLoc4, 1, GL_FALSE, &view4[0][0]);
                glUniformMatrix4fv(theprojection4, 1, GL_FALSE, &projection4[0][0]);
            
                glBindTexture(GL_TEXTURE_2D, textureblock2);
                glBindVertexArray(VAO[i]);
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

               
                unsigned int modelLoc4 = glGetUniformLocation(shaderProgramlighting, "model");
                unsigned int viewLoc4 = glGetUniformLocation(shaderProgramlighting, "view");
                unsigned int theprojection4 = glGetUniformLocation(shaderProgramlighting, "projection");

                glUniformMatrix4fv(modelLoc4, 1, GL_FALSE, glm::value_ptr(model4));
                glUniformMatrix4fv(viewLoc4, 1, GL_FALSE, &view4[0][0]);
                glUniformMatrix4fv(theprojection4, 1, GL_FALSE, &projection4[0][0]);
                
                glBindTexture(GL_TEXTURE_2D, textureblock3);
                glBindVertexArray(VAO[i]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

        }
        
        glm::mat4 model5 = glm::mat4(1.0f); 
        glm::mat4 view5 = glm::mat4(1.0f);
        glm::mat4 projection5 = glm::mat4(1.0f);
        view5 = camera.GetViewMatrix();
        model5 = glm::rotate(model5, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        
        projection5 = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

       
        unsigned int modelLoc5 = glGetUniformLocation(shaderProgramlighting, "model");
        unsigned int viewLoc5 = glGetUniformLocation(shaderProgramlighting, "view");
        unsigned int theprojection5 = glGetUniformLocation(shaderProgramlighting, "projection");

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

       
        unsigned int modelLoc6 = glGetUniformLocation(shaderProgramlighting, "model");
        unsigned int viewLoc6 = glGetUniformLocation(shaderProgramlighting, "view");
        unsigned int theprojection6 = glGetUniformLocation(shaderProgramlighting, "projection");

        glUniformMatrix4fv(modelLoc6, 1, GL_FALSE, glm::value_ptr(model6));
        glUniformMatrix4fv(viewLoc6, 1, GL_FALSE, &view6[0][0]);
        glUniformMatrix4fv(theprojection6, 1, GL_FALSE, &projection6[0][0]);
       
        glBindTexture(GL_TEXTURE_2D, textureblock1);
        glBindVertexArray(VAO[85]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Activiting the shader and calling the methods for chess pieces
        glUseProgram(shaderProgramtexture);
        glTexCoord2f(0.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, textureblock4);
        whitecylinderpawn();

        glUseProgram(shaderProgramtexture);
        glTexCoord2f(0.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, textureblock4);
        whitecylinderrook();

        glUseProgram(shaderProgramtexture);
        glBindTexture(GL_TEXTURE_2D, textureblock4);
        whitecylinderking();

        glUseProgram(shaderProgramtexture);
        glBindTexture(GL_TEXTURE_2D, textureblock4);
        whitecylinderqueen();

        glUseProgram(shaderProgramtexture);
        glBindTexture(GL_TEXTURE_2D, textureblock4);
        whitecylinderbishop();

        glUseProgram(shaderProgramtexture);
        glBindTexture(GL_TEXTURE_2D, textureblock4);
        whitecylinderknight();

        glUseProgram(shaderProgramtexture);
        glBindTexture(GL_TEXTURE_2D, textureblock5);
        blackpiecepawn();

        glUseProgram(shaderProgramtexture);
        glBindTexture(GL_TEXTURE_2D, textureblock5);
        blackpiecerook();

        glUseProgram(shaderProgramtexture);
        glBindTexture(GL_TEXTURE_2D, textureblock5);
        blackpieceking();

        glUseProgram(shaderProgramtexture);
        glBindTexture(GL_TEXTURE_2D, textureblock5);
        blackpiecequeen();

        glUseProgram(shaderProgramtexture);
        glBindTexture(GL_TEXTURE_2D, textureblock5);
        blackpiecebishop();

        glUseProgram(shaderProgramtexture);
        glBindTexture(GL_TEXTURE_2D, textureblock5);
        blackpieceknight();



        //Skybox
       
        

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    
    

    

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();

    return EXIT_SUCCESS;
}


void moving()
{
    // Camera controls
    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    
    if (Using_mouse) {
        if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        {
            camera.ProcessKeyboard(FORWARD, deltaTime);
            cameraknighttoppiece -= cameraSpeed * cameraforward;
            cameraknightheightpiece -= cameraSpeed * cameraforward;
            cameraknightpiece -= cameraSpeed * cameraforward;
            camerabishoptoppiece -= cameraSpeed * cameraforward;
            camerabishopheightpiece -= cameraSpeed * cameraforward;
            camerabishoppiece -= cameraSpeed * cameraforward;
            cameraqueentoppiece -= cameraSpeed * cameraforward;
            cameraqueenheightpiece -= cameraSpeed * cameraforward;
            cameraqueenpiece -= cameraSpeed * cameraforward;
            camerakingtoppiece -= cameraSpeed * cameraforward;
            camerakingheightpiece -= cameraSpeed * cameraforward;
            camerakingpiece -= cameraSpeed * cameraforward;
            camerarooktoppiece -= cameraSpeed * cameraforward;
            camerarookheightpiece -= cameraSpeed * cameraforward;
            camerarookpiece -= cameraSpeed * cameraforward;
            camerapawntoppiece -= cameraSpeed * cameraforward;
            camerapawnheightpiece -= cameraSpeed * cameraforward;
            camerapawnpiece -= cameraSpeed * cameraforward;
        }

        if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
            cameraknighttoppiece += cameraSpeed * cameraforward;
            cameraknightheightpiece += cameraSpeed * cameraforward;
            cameraknightpiece += cameraSpeed * cameraforward;
            camerabishoptoppiece += cameraSpeed * cameraforward;
            camerabishopheightpiece += cameraSpeed * cameraforward;
            camerabishoppiece += cameraSpeed * cameraforward;
            cameraqueentoppiece += cameraSpeed * cameraforward;
            cameraqueenheightpiece += cameraSpeed * cameraforward;
            cameraqueenpiece += cameraSpeed * cameraforward;
            camerakingtoppiece += cameraSpeed * cameraforward;
            camerakingheightpiece += cameraSpeed * cameraforward;
            camerakingpiece += cameraSpeed * cameraforward;
            camerarooktoppiece += cameraSpeed * cameraforward;
            camerarookheightpiece += cameraSpeed * cameraforward;
            camerarookpiece += cameraSpeed * cameraforward;
            camerapawntoppiece += cameraSpeed * cameraforward;
            camerapawnheightpiece += cameraSpeed * cameraforward;
            camerapawnpiece += cameraSpeed * cameraforward;
        }

        if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        {
            camera.ProcessKeyboard(LEFT, deltaTime);
            cameraknighttoppiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            cameraknightheightpiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            cameraknightpiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerabishoptoppiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerabishopheightpiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerabishoppiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            cameraqueentoppiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            cameraqueenheightpiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            cameraqueenpiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerakingtoppiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerakingheightpiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerakingpiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerarooktoppiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerarookheightpiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerarookpiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerapawntoppiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerapawnheightpiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerapawnpiece += glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
        }

        if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        {
            camera.ProcessKeyboard(RIGHT, deltaTime);
            cameraknighttoppiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            cameraknightheightpiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            cameraknightpiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerabishoptoppiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerabishopheightpiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerabishoppiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            cameraqueentoppiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            cameraqueenheightpiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            cameraqueenpiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerakingtoppiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerakingheightpiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerakingpiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerarooktoppiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerarookheightpiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerarookpiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerapawntoppiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerapawnheightpiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
            camerapawnpiece -= glm::normalize(glm::cross(cameraforward, cameraUp)) * cameraSpeed;
        }

    }
    
       
    
    
}
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ENTER == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        display_fps = display_fps ? false : true;
    }

    if (GLFW_KEY_F == key && GLFW_PRESS == action)
    {
        Using_mouse = Using_mouse ? false : true;
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
   
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (ActiveMouse)
        {
            lastXpos = xpos;
            lastYpos = ypos;
            ActiveMouse = false;
        }

        GLfloat xOffset = xpos - lastXpos;
        GLfloat yOffset = lastYpos - ypos;  // Reversed since y-coordinates go from bottom to left

        lastXpos = xpos;
        lastYpos = ypos;

        if (Using_mouse) {
            camera.ProcessMouseMovement(xOffset, yOffset);
        }
    
    
    
}
