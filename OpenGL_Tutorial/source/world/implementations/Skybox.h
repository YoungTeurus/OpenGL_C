#pragma once
#include "../interfaces/DrawableObject.h"
#include "../interfaces/PositionedObject.h"

class Skybox : public PositionedObject, public DrawableObject
{
private:
	VOsAndIndices* skyboxVOsAndIndices = VAOBuilder::getInstance()->getSkybox();
	CubeMap* cubeMap;

	std::string cubeMapFolderName;
	std::string cubeMapFileName;
public:
	Skybox(Renderer* renderer, const std::string& cubeMapFolderName, const std::string& cubeMapFileName)
		:PositionedObject({}), DrawableObject(renderer, "skybox"),
		 cubeMapFolderName(cubeMapFolderName), cubeMapFileName(cubeMapFileName)
	{
		cubeMap = TexturesLoader::getInstance()->getOrLoadCubeMap(cubeMapFileName, FilePaths::getPathToTexturesFolderWithTrailingSplitter() + cubeMapFolderName);
	}

	void drawAction() override
	{
		// Отрисовка skybox:
		glDepthFunc(GL_LEQUAL);
		shader->use();
		
		// Избавляемся от перемещения камеры, сохраняя поворот
		glm::mat4 skyboxView = glm::mat4(glm::mat3(getAttachedRenderer()->getView()));
		
		shader->setFloatMat4("projection", getAttachedRenderer()->getProjection());
		shader->setFloatMat4("view", skyboxView);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->getId());
		glBindVertexArray(skyboxVOsAndIndices->vao);
		glDrawElements(GL_TRIANGLES, skyboxVOsAndIndices->indices.size(), GL_UNSIGNED_INT, NULL);
		glDepthMask(GL_TRUE);  // Последующие элементы влияют на Depth
		
		glDepthFunc(GL_LESS);
		// Конец отрисовки skybox-а
	}
};
