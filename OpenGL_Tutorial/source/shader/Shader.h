#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/fwd.hpp>
#include <glad/glad.h>

class Shader;

enum class ValueTypes
{
	Bool,
	Int,
	Float,
	FloatVec2,
	FloatVec3,
	FloatVec4,
	FloatMat3,
	FloatMat4,
};

class Shader
{
private:
	std::string filename;
	std::string folder;
	bool withGeometry;
public:
	Shader(const std::string& shaderFileName, const std::string& pathToShaderFiles, const bool withGeometry = false);
	~Shader();

	void use() const;
	unsigned getProgramID() const;
	void setBool(const std::string& name, const bool value) const;
	void setInt(const std::string& name, const int value) const;
	void setFloat(const std::string& name, const float value) const;

	void setFloatVec2(const std::string& name, const float value, const float value2) const;
	void setFloatVec2(const std::string& name, const glm::vec2& vec) const;
	void setFloatVec3(const std::string& name, const float value, const float value2, const float value3) const;
	void setFloatVec3(const std::string& name, const glm::vec3& vec) const;
	void setFloatVec4(const std::string& name, const float value, const float value2, const float value3, const float value4) const;
	void setFloatVec4(const std::string& name, const glm::vec4& vec) const;

	void setFloatMat3(const std::string& name, const glm::mat3& matrix) const;
	void setFloatMat4(const std::string& name, const glm::mat4& matrix) const;

private:
	unsigned programID;
};