#pragma once

#include <iostream>
#include <map>
#include <string>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

class TextRenderer
{

public:

	void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);
    Shader setupRenderer();

    struct Character {
        unsigned int TextureID;
        glm::ivec2   Size;
        glm::ivec2   Bearing;
        unsigned int Advance;
    };

    std::map<char, Character> Characters;
    unsigned int VAO, VBO;

    unsigned int SCR_WIDTH = 800;
    unsigned int SCR_HEIGHT = 600;

    TextRenderer(unsigned int width, unsigned int height) {
        SCR_WIDTH = width;
        SCR_HEIGHT = height;
    }

};

