#version 330 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;
layout ( location = 2 ) in vec2 tex;
uniform mat4 VP;
uniform mat4 model;
uniform mat4 view;
uniform vec3 aNormal;
uniform mat4 projection;
out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;
void main()
{
FragPos = vec3(model * vec4(position, 1.0));
Normal = normal;
TexCoords = tex;
gl_Position = projection * view * vec4(FragPos, 1.0);
}