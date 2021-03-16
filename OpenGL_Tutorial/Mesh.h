#pragma once
#include <list>
#include <glad/glad.h>

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

// TODO: вынести в отдельный класс?
// Информация о данных вершин
struct VertexDataConfig
{
	int sizeOfVertex;				// Размер одного элемента данных вершины
	std::list<Attribute> attributes;// Атрибуты вершины, хранимые в данных
};

// TODO: вынести в другое место?
// Стандартная информация для вершины, содержащей позицию, цвет, нормаль и координаты текстуры.
static VertexDataConfig pos_color_normal_texture_VDC = VertexDataConfig{
	12,
	{
		// Position:
		Attribute{0, 3, GL_FLOAT, sizeof(float), 0},
		// Color:
		Attribute{1, 4, GL_FLOAT, sizeof(float), 3},
		// Texture:
		Attribute{2, 2, GL_FLOAT, sizeof(float), 10},
		// Normal
		Attribute{3, 3, GL_FLOAT, sizeof(float), 7},
	}
};

class Mesh
{
private:
	const float		*vertex_data_;		// Данные о вершинах объекта
	const unsigned	*indices_data_;		// Данные о гранях объекта
	const int size_of_vertex_data_;
	const int size_of_indices_data_;

	const VertexDataConfig *vdc_;		// Настройка данных о вершинах объекта

	unsigned vao_;
	unsigned vbo_;
	unsigned ebo_;
public:
	Mesh(const int vertex_data_length, const float *vertex_data, const int indices_data_length, const unsigned *indices_data, const VertexDataConfig *vdc)
	: vertex_data_(vertex_data), indices_data_(indices_data), vdc_(vdc), size_of_vertex_data_(vertex_data_length), size_of_indices_data_(indices_data_length)
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size_of_vertex_data_, vertex_data_, GL_STATIC_DRAW);

		// EBO:
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * size_of_indices_data_, indices_data_, GL_STATIC_DRAW);

		// Сохраняем данные о атрибутах:
		for (const auto& attr : vdc_->attributes)
		{
			glVertexAttribPointer(attr.id, attr.size, attr.type, GL_FALSE, vdc_->sizeOfVertex * attr.sizeOfType, (void*)(static_cast<long>(attr.offset) * attr.sizeOfType));
			glEnableVertexAttribArray(attr.id);
		}
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeOfVertex_cube * sizeof(float), (void*)0);
		// glEnableVertexAttribArray(0);
		// glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeOfVertex_cube * sizeof(float), (void*)(3 * sizeof(float)));
		// glEnableVertexAttribArray(1);
		// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeOfVertex_cube * sizeof(float), (void*)(10 * sizeof(float)));
		// glEnableVertexAttribArray(2);
		// glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeOfVertex_cube * sizeof(float), (void*)(7 * sizeof(float)));
		// glEnableVertexAttribArray(3);

		// Сбрасываем VAO:
		glBindVertexArray(0);
	}

	void use() const
	{
		glBindVertexArray(vao_);
		glDrawElements(GL_TRIANGLES, size_of_indices_data_, GL_UNSIGNED_INT, 0);
	}
};

