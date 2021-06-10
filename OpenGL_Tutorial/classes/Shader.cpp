#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

unsigned Shader::getProgramID() const {
	return programID;
}

Shader::Shader(const GLchar *name) {
	std::string vertexCode, fragmentCode;
	std::string vTempString, fTempString;

	std::ifstream vShaderFile, fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		std::stringstream vShaderStream, fShaderStream;
		// ��������� ����� � ��������� �� � ������:
		vShaderFile.open(R"(.\shaders\)" + std::string(name) + ".vert");
		vShaderStream << vShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.open(R"(.\shaders\)" + std::string(name) + ".frag");
		fShaderStream << fShaderFile.rdbuf();
		fShaderFile.close();
		// �������� ������ �� ���������� ������:
		vTempString = vShaderStream.str();
		fTempString = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar *vShaderCode = vTempString.c_str();
	const GLchar *fShaderCode = fTempString.c_str();

	unsigned vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertex);
	glAttachShader(programID, fragment);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() const {
	glUseProgram(programID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setFloatVec2(const std::string& name, float value, float value2) const
{
	glUniform2f(glGetUniformLocation(programID, name.c_str()), value, value2);
}

void Shader::setFloatVec3(const std::string& name, float value, float value2, float value3) const
{
	glUniform3f(glGetUniformLocation(programID, name.c_str()), value, value2, value3);
}

void Shader::setFloatVec3(const std::string& name, glm::vec3 vec) const
{
	glUniform3f(glGetUniformLocation(programID, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::setFloatVec4(const std::string& name, float value, float value2, float value3, float value4) const
{
	glUniform4f(glGetUniformLocation(programID, name.c_str()), value, value2, value3, value4);
}

void Shader::setFloatMat3(const std::string& name, glm::mat3& matrix) const
{
	glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setFloatMat4(const std::string& name, glm::mat4& matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

Shader::~Shader() {
	glDeleteProgram(programID);
}