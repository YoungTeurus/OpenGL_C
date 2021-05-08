#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Indice> indices, std::vector<Texture> textures)
	:vertices(vertices), indices(indices), textures(textures)
{
	setupMesh();
}

void Mesh::draw(Shader& shader) const
{
	// Подключение текстур:
	unsigned diffuseNum = 1;
	unsigned specularNum = 1;
	for (unsigned i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);  // Активируем необходимую текстуру перед подключением

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
		default:
			throw new std::runtime_error("Mesh::draw: TextureType of currentTexutre is undefined for switch case!");
			break;
		}
		std::string currentTextureShaderName = textureTypeShaderNames.at(currentTexutureType);

		shader.setFloat( ("material." + currentTextureShaderName + textureNumber).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, currentTexutre.ID());
	}
	glActiveTexture(GL_TEXTURE0);

	// Отображение mesh-а:
	glBindVertexArray(vao_);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	// VAO полигона - хранение данных о атрибутах
	// VBO полигона - хранение данных в видеопамяти
	// EBO полигона - хранение данных о рёбрах и гранях
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);

	// Привязка VAO:
	glBindVertexArray(vao_);

	// Сохраняем в VAO:
	glBindBuffer(GL_ARRAY_BUFFER, vao_); // Буфер для хранения данных
	// Читаем данные в VBO:
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// EBO:
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indice) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// Сохраняем данные о атрибутах:
	// TODO: Избавиться от magic values при помощи Attribute?
	// Позиция вертекса:
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::position));
	// Нормаль к вертексу:
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::normal));
	// Координаты текстур вертекса:
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::texCoords));

	// Сбрасываем VAO:
	glBindVertexArray(0);
}
