#pragma once

#include "Mesh.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

class Model
{
private:
	vector<Mesh> meshes;
	string directory;

public:
	Model(char* path) {
		loadModel(path);
	}
	void draw(Shader& shader);
private:
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);
};

