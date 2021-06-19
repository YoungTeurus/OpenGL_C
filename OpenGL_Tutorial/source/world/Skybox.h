#pragma once
#include "DrawableObject.h"
#include "../model/VAOBuilder.h"
#include "../model/VOsAndIndices.h"

class Skybox : public PositionedWorldObject, public DrawableObject
{
private:
	VOsAndIndices* skyboxVOsAndIndices = VAOBuilder::getInstance()->getSkybox();
	CubeMap* cubeMap = nullptr;

	std::string cubeMapFolderName;
	std::string cubeMapFileName;
public:
	Skybox(const std::string& cubeMapFolderName, const std::string& cubeMapFileName)
		:PositionedWorldObject({}), DrawableObject("skybox"),
		 cubeMapFolderName(cubeMapFolderName), cubeMapFileName(cubeMapFileName)
	{
	}

	void draw(Renderer* renderer) override
	{
		// ��������� skybox:

		// �������� ������������� �������:
		if(cubeMap == nullptr)
		{
			cubeMap = renderer->getTexturesLoader()->getOrLoadByFileNameAndDirectoryCubeMap(cubeMapFileName, FilePaths::getPathToTexturesFolderWithTrailingSplitter() + cubeMapFolderName);
		}
		
		glDepthFunc(GL_LEQUAL);
		shader->use();
		
		// ����������� �� ����������� ������, �������� �������
		glm::mat4 skyboxView = glm::mat4(glm::mat3(renderer->getView()));
		
		shader->setFloatMat4("projection", renderer->getProjection());
		shader->setFloatMat4("view", skyboxView);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->getId());
		glBindVertexArray(skyboxVOsAndIndices->vao);
		glDrawElements(GL_TRIANGLES, skyboxVOsAndIndices->indices.size(), GL_UNSIGNED_INT, NULL);
		glDepthMask(GL_TRUE);  // ����������� �������� ������ �� Depth
		
		glDepthFunc(GL_LESS);
		// ����� ��������� skybox-�
	}
};
