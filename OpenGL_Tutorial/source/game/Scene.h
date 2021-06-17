#pragma once
#include <vector>


#include "SceneObject.h"
#include "../light/BaseLight.h"

using namespace std;

class Scene
{
public:
	glm::vec4 backgroundColor;
	vector<SceneObject*> objects;
	vector<BaseLight*> lights;
};