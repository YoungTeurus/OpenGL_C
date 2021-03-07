#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void use();
	void setFloat(const std::string& name, float value) const;
	unsigned ID();

private:
	unsigned programID;
	void checkCompileErrors(unsigned shader, std::string type);
};

