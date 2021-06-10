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

	// TODO: �����������, ��� ����� tangent!
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

// TODO: ������� � ��������� �����?
// ������� �������
struct Attribute
{
	int id;				// ������������� �������� (���������� ����� � �������)
	int size;			// ������ ��������
	int type;			// ��� ��������
	int sizeOfType;		// ������ ���� ��������
	int offset;			// �������� (� ������)
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
	std::vector<Vertex>			*vertices;		// ������ � �������� �������
	std::vector<Indice>			*indices;		// ������ � ������ �������
	std::vector<Texture*>		*textures;		// ��������, ��������� � ������ ��������

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

