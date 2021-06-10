#pragma once

#include <list>
#include <vector>
#include <glad/glad.h>
#include "classes/Texture.h"
#include "classes/Shader.h"

typedef unsigned int Indice;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

	// TODO: разобраться, что такое tangent!
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

// TODO: вынести в отдельный класс?
// Атрибут шейдера
struct Attribute
{
	int id;				// Идентификатор атрибута (порядковый номер в шейдере)
	int size;			// Размер атрибута
	int type;			// Тип атрибута
	int sizeOfType;		// Размер типа атрибута
	int offset;			// Смещение (в байтах)
};

struct MeshAttributesConfig {
	std::list<Attribute> attributes;
};

static MeshAttributesConfig defaultConfig_positionNormalTexture = MeshAttributesConfig{
	{
		// Position:
		Attribute{0, 3, GL_FLOAT, sizeof(float), 0},
		// Color:
		Attribute{1, 4, GL_FLOAT, sizeof(float), 3},
		// Texture:
		Attribute{2, 2, GL_FLOAT, sizeof(float), 7},
	}
};

class Mesh
{
public:
	std::vector<Vertex>			*vertices;		// Данные о вершинах объекта
	std::vector<Indice>			*indices;		// Данные о гранях объекта
	std::vector<Texture*>		*textures;		// Текстуры, связанные с данным объектом

private:
	unsigned vao_;
	unsigned vbo_;
	unsigned ebo_;

public:
	Mesh(std::vector<Vertex> *vertices, std::vector<Indice> *indices, std::vector<Texture*> *textures);

	~Mesh()
	{
		printf("Goodbye, cruel world!\n");
	}

	void draw(const Shader &shader) const;

private:
	void setupMesh();
};

