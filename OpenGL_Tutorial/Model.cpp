#include "Model.h"
#include <stb_image.h>

void Model::draw(Shader& shader)
{
	for(auto& mesh : meshes)
	{
		mesh.draw(shader);
	}
}

void Model::loadModel(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// Обрабатываем все mesh-и, связанные с текущим node
	for (unsigned i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// Обрабатываем все child node-ы
	for (unsigned i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<Indice> indices;
	vector<Texture> textures;

	// Обработка положения вершины:
	for (unsigned i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		aiVector3D &currentVertexPosition = mesh->mVertices[i];

		glm::vec3 tempVector;
		tempVector.x = currentVertexPosition.x;
		tempVector.y = currentVertexPosition.y;
		tempVector.z = currentVertexPosition.z;
		vertex.position = tempVector;

		if (mesh->HasNormals()) {
			aiVector3D& currentVertexNormal = mesh->mNormals[i];
			tempVector.x = currentVertexNormal.x;
			tempVector.y = currentVertexNormal.y;
			tempVector.z = currentVertexNormal.z;
			vertex.normal = tempVector;
		}

		// Если у модели есть текстурные координаты...
		// Работаем только с первой текстурой (из 8-и возможных).
		if (mesh->mTextureCoords[0]) {
			glm::vec2 textureCoords;
			textureCoords.x = mesh->mTextureCoords[0][i].x;
			textureCoords.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = textureCoords;
		}
		else {
			vertex.texCoords = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}

	// Обработка indices:
	for (unsigned i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// Обработка материалов:
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType)
{
	vector<Texture> textures;
	for (unsigned i = 0; i < mat->GetTextureCount(type); i++) {
		aiString textureFilename;
		mat->GetTexture(type, i, &textureFilename);

		string pathToTexture = directory + '/' + string(textureFilename.C_Str());
		
		Texture *texture = new Texture(pathToTexture.c_str(), textureType);
		texture->filename = string(textureFilename.C_Str());
		textures.push_back(*texture);
	}
	return vector<Texture>();
}
