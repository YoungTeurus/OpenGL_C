#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec4 fColor;
out vec2 fTexCoord;
out vec3 fNormal;
out vec3 fragPosition;  // ��������� ����� � ������� �����������

// ������� �������� ������� ������������� � ��� ����������� �������
// � ����� ������� - �����, ��� ������ ������, � ������ - ������ �������� �� �����?
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transformation;  // ������������ ��� ������ ���� � �������: P * V * M
uniform mat3 normal;  // ������� �������� - ������������ ��� ��������/���������

void main()
{
	gl_Position = transformation * vec4(aPos, 1.0f);
	fragPosition = vec3(model * vec4(aPos, 1.0f));
	fNormal = normal * aNormal;

	fColor = aColor;
	fTexCoord = aTexCoord;
}