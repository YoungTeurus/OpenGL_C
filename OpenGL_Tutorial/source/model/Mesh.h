#pragma once
#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>


#include "../textures/Texture.h"
#include "../shader/Shader.h"

using namespace std;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

class Mesh
{
public:
    vector<Vertex>          vertices;
    vector<unsigned int>    indices;
    vector<Texture*>        textures;
    unsigned int            VAO;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture*> textures);
    void draw(Shader& shader);
private:

    unsigned int VBO, EBO;

    void setupMesh();
};

inline Mesh::Mesh(vector<Vertex> vertices, vector<unsigned> indices, vector<Texture*> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

inline void Mesh::draw(Shader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        string textureNumber;

    	const Texture *currentTexture = textures[i];
    	const TextureType currentTextureType = currentTexture->getType();

        switch (currentTextureType)
        {
        case TextureType::DIFFUSE: textureNumber = std::to_string(diffuseNr++); break;
        case TextureType::SPECULAR: textureNumber = std::to_string(specularNr++); break;
        case TextureType::NORMAL: textureNumber = std::to_string(normalNr++); break;
        case TextureType::HEIGHT: textureNumber = std::to_string(heightNr++); break;
        default: ;
        }

    	string currentTextureShaderName = textureTypeShaderNames.at(currentTextureType) + textureNumber;
    	shader.setInt(currentTextureShaderName, i);

        glBindTexture(GL_TEXTURE_2D, currentTexture->getId());
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

inline void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
}

