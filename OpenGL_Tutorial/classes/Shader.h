#pragma once

#include <cstdarg>
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <map>
#include <glm/glm.hpp>

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

// struct ShaderParameter
// {
// 	std::string name;		// Название параметра
// 	ValueTypes type;		// Тип параметра
// 
// 	template<typename T>
// 	void use(Shader *shader, T value)
// 	{
// 		switch (type)
// 		{
// 		case ValueTypes::Bool:
// 			shader->setBool(name, value);
// 			break;
// 		case ValueTypes::Int:
// 			shader->setInt(name, value);
// 			break;
// 		case ValueTypes::Float:
// 			shader->setFloat(name, value);
// 			break;
// 		case ValueTypes::FloatVec2:
// 			shader->setFloatVec2(name, value);
// 			break;
// 		case ValueTypes::FloatVec3:
// 			shader->setFloatVec3(name, value);
// 			break;
// 		case ValueTypes::FloatVec4:
// 			shader->setFloatVec4(name, value);
// 			break;
// 		case ValueTypes::FloatMat3:
// 			shader->setFloatMat3(name, value);
// 			break;
// 		case ValueTypes::FloatMat4:
// 			shader->setFloatMat4(name, value);
// 			break;
// 		}
// 	}
// };

class Shader
{
private:
	// std::map<std::string, ShaderParameter> parameters_;
	// std::list<ShaderParameter> parameters_ = {
	// 	ShaderParameter{"model", ValueTypes::FloatMat4, 1},
	// };
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void use() const;
	unsigned ID() const;
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	void setFloatVec2(const std::string& name, float value, float value2) const;
	void setFloatVec2(const std::string& name, glm::vec2 vec) const;
	void setFloatVec3(const std::string& name, float value, float value2, float value3) const;
	void setFloatVec3(const std::string& name, glm::vec3 vec) const;
	void setFloatVec4(const std::string& name, float value, float value2, float value3, float value4) const;
	void setFloatVec4(const std::string& name, glm::vec4 vec) const;

	void setFloatMat3(const std::string& name, float* mat) const;
	void setFloatMat4(const std::string& name, float* mat) const;

	// template<typename T>
	// void setValue(const std::string& name, T value) const
	// {
	// 	const auto parameter = parameters_.find(name);
	// 	if (parameter != parameters_.end())
	// 	{
	// 		parameter->second.use(this, value);
	// 	}
	// 	// parameters_.find(name)->second.use(this, value);
	// };

	// void setValue(ValueTypes value_type, const std::string& name, ...) const;


	// setValue(Int, "mode", 3);
	// setValue(Float, "speed", 3.0f);
	// setValue(FloatVec3, "position", 3.0f, 3.0f, 3.0f);
	// setValue(FloatMat3, "matrix", glf::value_ptr(SMTH));

	// Хочется сделать функцию, которая будет принимать любое количество аргументов любых типов, а затем
	// вызывать требуемые методы в зависимости от ShaderParameter в paramaters_, то есть:

	// shader.setValues(pos_.x, pos_.y, pos_.z, scale_.x, scale_.y, scale_.z, );

	// template<typename T>
	// void setValues(T arg, ...)
	// {
	// 	va_list arguments;
	// 	std::list<void*> par;
	// 	for (const auto& shader_parameter : parameters_)
	// 	{
	// 		for(int i = 0; i < shader_parameter.parameterCount; i++)
	// 		{
	// 			arg = va_arg(a);
	// 			par.emplace_back();
	// 		}
	// 		par.clear();
	// 	}
	// }

private:
	unsigned programID;
};