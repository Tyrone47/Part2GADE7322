
#include "Terrain.h"

void Terrain::generateTerrain() {

    //Step 1
    stbi_set_flip_vertically_on_load(true);
    int width, height, nChannels;
    unsigned char* data = stbi_load("res/heightmap.png",
        &width, &height, &nChannels,
        0);

    //Step 2
    std::vector<float> vertices;
    float yScale = 64.0f / 256.0f, yShift = 16.0f;
    int rez = 1;
    unsigned bytePerPixel = nChannels;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
            unsigned char y = pixelOffset[0];

            // vertex
            vertices.push_back(-height / 2.0f + height * i / (float)height);
            vertices.push_back((int)y * yScale - yShift);
            vertices.push_back(-width / 2.0f + width * j / (float)width);

            
            vertices.push_back((float)j / (float)width);
            vertices.push_back((float)i / (float)height);

            
        }
    }
    stbi_image_free(data);


    //Step 4
    std::vector<unsigned> indices;
    for (unsigned i = 0; i < height - 1; i += rez)
    {
        for (unsigned j = 0; j < width; j += rez)
        {
            for (unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + width * (i + k * rez));
            }
        }
    }
    std::cout << "Loaded " << indices.size() << " indices" << std::endl;

    //Step 5
    this->numStrips = (height - 1) / rez;
    this->numTrisPerStrip = (width / rez) * 2 - 2;


    unsigned int terrainVAO, terrainVBO, terrainIBO;
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    //5 Used to be a 3
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Texture attributes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &terrainIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

    this->vertex_array_id = terrainVAO;
}

GLuint Terrain::getVertexArrayId() {
	return this->vertex_array_id;
}
