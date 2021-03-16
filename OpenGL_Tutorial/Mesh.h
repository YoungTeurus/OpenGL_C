#pragma once
#include <list>
#include <glad/glad.h>

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

// TODO: ������� � ��������� �����?
// ���������� � ������ ������
struct VertexDataConfig
{
	int sizeOfVertex;				// ������ ������ �������� ������ �������
	std::list<Attribute> attributes;// �������� �������, �������� � ������
};

// TODO: ������� � ������ �����?
// ����������� ���������� ��� �������, ���������� �������, ����, ������� � ���������� ��������.
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
	const float		*vertex_data_;		// ������ � �������� �������
	const unsigned	*indices_data_;		// ������ � ������ �������
	const int size_of_vertex_data_;
	const int size_of_indices_data_;

	const VertexDataConfig *vdc_;		// ��������� ������ � �������� �������

	unsigned vao_;
	unsigned vbo_;
	unsigned ebo_;
public:
	Mesh(const int vertex_data_length, const float *vertex_data, const int indices_data_length, const unsigned *indices_data, const VertexDataConfig *vdc)
	: vertex_data_(vertex_data), indices_data_(indices_data), vdc_(vdc), size_of_vertex_data_(vertex_data_length), size_of_indices_data_(indices_data_length)
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size_of_vertex_data_, vertex_data_, GL_STATIC_DRAW);

		// EBO:
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * size_of_indices_data_, indices_data_, GL_STATIC_DRAW);

		// ��������� ������ � ���������:
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

		// ���������� VAO:
		glBindVertexArray(0);
	}

	void use() const
	{
		glBindVertexArray(vao_);
		glDrawElements(GL_TRIANGLES, size_of_indices_data_, GL_UNSIGNED_INT, 0);
	}
};

