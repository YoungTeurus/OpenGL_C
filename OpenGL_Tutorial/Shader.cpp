#include "Shader.h"

unsigned Shader::ID() {
	return programID;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	const char* vShaderCode, *fShaderCode;
	std::string vTempString, fTempString;

	std::ifstream vShaderFile, fShaderFile;
}
