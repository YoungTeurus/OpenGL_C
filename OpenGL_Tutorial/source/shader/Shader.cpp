#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

unsigned Shader::getProgramID() const {
	return programID;
}

Shader::Shader(const std::string& shaderFileName, const std::string& pathToShaderFiles, const bool withGeometry)
	:filename(shaderFileName), folder(pathToShaderFiles), withGeometry(withGeometry)
{
	std::string vertexCode, fragmentCode, geometryCode;
	std::string vTempString, fTempString, gTempString;

	std::ifstream vShaderFile, fShaderFile, gShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	if (withGeometry){
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	}

	try {
		std::stringstream vShaderStream, fShaderStream, gShaderStream;
		// Открываем файлы и считываем их в буферы:
		vShaderFile.open(pathToShaderFiles + shaderFileName + ".vert");
		vShaderStream << vShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.open(pathToShaderFiles + shaderFileName + ".frag");
		fShaderStream << fShaderFile.rdbuf();
		fShaderFile.close();
		// Получаем строку из строкового потока:
		vTempString = vShaderStream.str();
		fTempString = fShaderStream.str();
		if (withGeometry){
			gShaderFile.open(pathToShaderFiles + shaderFileName + ".geom");
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();

			gTempString = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << pathToShaderFiles << shaderFileName << std::endl;
	}
	const GLchar *vShaderCode = vTempString.c_str();
	const GLchar *fShaderCode = fTempString.c_str();
	const GLchar *gShaderCode;
	if (withGeometry)
	{
		gShaderCode = gTempString.c_str();
	}

	unsigned vertexShaderId, fragmentShaderId, geometryShaderId;
	GLint compileStatus;
	char infoLog[512];

	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vShaderCode, NULL);
	glCompileShader(vertexShaderId);
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShaderId);
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	if (withGeometry)
	{
		geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShaderId, 1, &gShaderCode, NULL);
		glCompileShader(geometryShaderId);
		glGetShaderiv(geometryShaderId, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus != GL_TRUE) {
			GLint infoLogLength;
			glGetShaderiv(geometryShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar* buffer = new GLchar[infoLogLength];
			GLsizei bufferSize;
			
			glGetShaderInfoLog(geometryShaderId, infoLogLength, &bufferSize, buffer);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << buffer << std::endl;

			delete[] buffer;
		}
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderId);
	glAttachShader(programID, fragmentShaderId);
	if (withGeometry)
	{
		glAttachShader(programID, geometryShaderId);
	}
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &compileStatus);
	if (!compileStatus) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	if (withGeometry)
	{
		glDeleteShader(geometryShaderId);
	}
}

void Shader::use() const {
	glUseProgram(programID);
}

void Shader::setBool(const std::string& name, const bool value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, const int value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, const float value) const
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setFloatVec2(const std::string& name, const float value, const float value2) const
{
	glUniform2f(glGetUniformLocation(programID, name.c_str()), value, value2);
}

void Shader::setFloatVec2(const std::string& name, const glm::vec2& vec) const
{
	glUniform2f(glGetUniformLocation(programID, name.c_str()), vec.x, vec.y);
}

void Shader::setFloatVec3(const std::string& name, const float value, const float value2, const float value3) const
{
	glUniform3f(glGetUniformLocation(programID, name.c_str()), value, value2, value3);
}

void Shader::setFloatVec3(const std::string& name, const glm::vec3& vec) const
{
	glUniform3f(glGetUniformLocation(programID, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::setFloatVec4(const std::string& name, const float value, const float value2, const float value3, const float value4) const
{
	glUniform4f(glGetUniformLocation(programID, name.c_str()), value, value2, value3, value4);
}

void Shader::setFloatVec4(const std::string& name, const glm::vec4& vec) const
{
	glUniform4f(glGetUniformLocation(programID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void Shader::setFloatMat3(const std::string& name, const glm::mat3& matrix) const
{
	glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setFloatMat4(const std::string& name, const glm::mat4& matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

Shader::~Shader() {
	glDeleteProgram(programID);
}