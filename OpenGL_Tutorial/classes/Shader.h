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
	unsigned ID();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	void setFloatVec2(const std::string& name, float value, float value2) const;
	void setFloatVec3(const std::string& name, float value, float value2, float value3) const;
	void setFloatVec4(const std::string& name, float value, float value2, float value3, float value4) const;

	void setFloatMat3(const std::string& name, float* mat);
	void setFloatMat4(const std::string& name, float* mat);

private:
	unsigned programID;
};

