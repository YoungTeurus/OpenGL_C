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
	vector<Texture*> textures_loaded;
	vector<Mesh*>* meshes;
	string directory;

public:
	Model(const string& path, bool isUV_flipped){
		meshes = new vector<Mesh*>();
		loadModel(path, isUV_flipped);
	}
	void draw(const Shader& shader);
private:
	void loadModel(string path, bool isUV_flipped);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);
};
