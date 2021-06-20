#pragma once
#include "../interfaces/DrawableObject.h"
#include "../interfaces/PositionedObject.h"

class Skybox : public PositionedObject, public DrawableObject
{
private:
	VOsAndIndices* skyboxVOsAndIndices = VAOBuilder::getInstance()->getSkybox();
	CubeMap* cubeMap = nullptr;

	std::string cubeMapFolderName;
	std::string cubeMapFileName;
public:
	Skybox(const std::string& cubeMapFolderName, const std::string& cubeMapFileName)
		:PositionedObject({}), DrawableObject("skybox"),
		 cubeMapFolderName(cubeMapFolderName), cubeMapFileName(cubeMapFileName)
	{
	}

	void drawAction(Renderer* renderer) override
	{
		// Отрисовка skybox:

		// Проверка загруженности текстур:
		if(cubeMap == nullptr)
		{
			cubeMap = renderer->getTexturesLoader()->getOrLoadCubeMap(cubeMapFileName, FilePaths::getPathToTexturesFolderWithTrailingSplitter() + cubeMapFolderName);
		}
		
		glDepthFunc(GL_LEQUAL);
		shader->use();
		
		// Избавляемся от перемещения камеры, сохраняя поворот
		glm::mat4 skyboxView = glm::mat4(glm::mat3(renderer->getView()));
		
		shader->setFloatMat4("projection", renderer->getProjection());
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
