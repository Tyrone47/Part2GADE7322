#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "stb_image.h"
#include <vector>

class Terrain
{

private:

	GLuint vertex_array_id = 0;


public:

	int numStrips;
	int numTrisPerStrip;

	void generateTerrain();

	GLuint getVertexArrayId();

};


