#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Indice> indices, std::vector<Texture> textures)
	:vertices(vertices), indices(indices), textures(textures)
{
	setupMesh();
}

void Mesh::draw(const Shader& shader) const
{
	// ����������� �������:
	unsigned diffuseNum = 1;
	unsigned specularNum = 1;
	unsigned normalNum = 1;
	unsigned heightNum = 1;
	for (unsigned i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);  // ���������� ����������� �������� ����� ������������

		std::string textureNumber;
		
		const Texture &currentTexutre = textures[i];
		const TextureType currentTexutureType = currentTexutre.type;
		
		switch (currentTexutureType)
		{
		case TextureType::DIFFUSE:
			textureNumber = std::to_string(diffuseNum++);
			break;
		case TextureType::SPECULAR:
			textureNumber = std::to_string(specularNum++);
			break;
		case TextureType::NORMAL:
			textureNumber = std::to_string(normalNum++);
			break;
		case TextureType::HEIGHT:
			textureNumber = std::to_string(heightNum++);
			break;
		default:
			throw new std::runtime_error("Mesh::draw: TextureType of currentTexutre is undefined for switch case!");
		}
		std::string currentTextureShaderName = textureTypeShaderNames.at(currentTexutureType);

		shader.setFloat( (currentTextureShaderName + textureNumber).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, currentTexutre.ID());
	}
	glActiveTexture(GL_TEXTURE0);

	// ����������� mesh-�:
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	// VAO �������� - �������� ������ � ���������
	// VBO �������� - �������� ������ � �����������
	// EBO �������� - �������� ������ � ����� � ������
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);

	// �������� VAO:
	glBindVertexArray(vao_);

	// ��������� � VAO:
	glBindBuffer(GL_ARRAY_BUFFER, vao_); // ����� ��� �������� ������
	// ������ ������ � VBO:
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// EBO:
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indice) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ��������� ������ � ���������:
	// ������� ��������:
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::position));
	glEnableVertexAttribArray(0);
	// ������� � ��������:
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::normal));
	glEnableVertexAttribArray(1);
	// ���������� ������� ��������:
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::texCoords));
	glEnableVertexAttribArray(2);
	// ���������� tangent-�:
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::tangent));
	glEnableVertexAttribArray(3);
	// ���������� bitangent-�:
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::bitangent));
	glEnableVertexAttribArray(4);

	// ���������� VAO:
	glBindVertexArray(0);
}