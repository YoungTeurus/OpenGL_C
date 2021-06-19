#pragma once
#include "../textures/Texture.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../textures/TexturesLoader.h"

using namespace std;

class Model
{
public:
    Model(string path, bool isUV_flipped = false)
    {
        loadModel(path, isUV_flipped);
    }
    void draw(Shader& shader);
    void drawSelected(Shader& shader, Shader &coloringShader);
private:

    // vector<Texture> textures_loaded;
    vector<Mesh> meshes;
    string directory;

    void loadModel(string path, const bool isUV_flipped);
    void processNode(aiNode* node, const aiScene* scene, const bool isUV_flipped);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, const bool isUV_flipped);
    vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType, const bool isUV_flipped);
};

inline void Model::draw(Shader& shader)
{
    shader.use();
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].draw(shader);
}

inline void Model::drawSelected(Shader &shader, Shader &coloringShader) {
    shader.use();
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].draw(shader);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    coloringShader.use();

    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].draw(coloringShader);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);
}

inline void Model::loadModel(string path, const bool isUV_flipped)
{
    Assimp::Importer importer;
    const aiScene* scene;
	//if (isUV_flipped)
	//{
	//	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	//} else
	//{
		scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals  | aiProcess_CalcTangentSpace);
	//}
	
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene, isUV_flipped);
}

inline void Model::processNode(aiNode* node, const aiScene* scene, const bool isUV_flipped)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, isUV_flipped));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, isUV_flipped);
    }
}

inline Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, const bool isUV_flipped)
{
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture*> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords = vec;
        }
        else
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);

    	if (mesh->mTangents == nullptr)
    	{
            vector.x = 0.0f;
            vector.y = 0.0f;
            vector.z = 0.0f;
    	}
        else
        {
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
        }
        vertex.tangent = vector;

        if (mesh->mBitangents == nullptr)
        {
            vector.x = 0.0f;
            vector.y = 0.0f;
            vector.z = 0.0f;
        }
        else
        {
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
        }
        vertex.bitangent = vector;
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE, isUV_flipped);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR, isUV_flipped);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture*> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::NORMAL, isUV_flipped);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture*> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::HEIGHT, isUV_flipped);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh(vertices, indices, textures);
}

inline vector<Texture*> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType, const bool isUV_flipped)
{
    vector<Texture*> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;

        TexturesLoader *texturesLoader = TexturesLoader::getInstance();
    	Texture* loadedTexture = texturesLoader->getOrLoadByFileNameAndDirectory2DTexture(string(str.C_Str()), this->directory, isUV_flipped);
    	loadedTexture->setType(textureType);
    	textures.push_back(loadedTexture);
    	
        // for (unsigned int j = 0; j < textures_loaded.size(); j++)
        // {
        //     if (std::strcmp(textures_loaded[j].getDirectory().data(), str.C_Str()) == 0)
        //     {
        //         textures.push_back(textures_loaded[j]);
        //         skip = true;
        //         break;
        //     }
        // }
        // if (!skip)
        // {
        //     Texture texture;
        //     texture.loadFromFile(string(str.C_Str()), this->directory, isUV_flipped);
        // 	texture.setType(textureType);
        // 	
        //     textures.push_back(texture);
        //     textures_loaded.push_back(texture);
        // }
    }
    return textures;
}
