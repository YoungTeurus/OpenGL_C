// GLAD подключаем ПЕРЕД всеми библиотеками с подключением OpenGL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
 
#include "light/PointLight.h"
#include "light/SpotLight.h"
#include "light/DirectionalLight.h"
#include "shader/Shader.h"
#include "camera/Camera.h"
#include "level/LevelLoader.h"
#include "model/VAOBuilder.h"
#include "model/VOsAndIndices.h"
#include "renderer/Renderer.h"
#include "renderer/Scene.h"
#include "shader/ShaderLoader.h"
#include "text/Font.h"
#include "text/FontLoader.h"
#include "world/animations/Animations.h"
#include "world/implementations/BrickWall.h"
#include "world/implementations/Ground.h"
#include "world/implementations/LightCube.h"
#include "world/implementations/ParticleGenerator.h"
#include "world/implementations/Skybox.h"
#include "world/implementations/Tank.h"
#include "world/implementations/TextString.h"

float deltaTime = 0.0f;									 // Разница во времени между последним и предпоследним кадрами
float lastFrameTime = 0.0f;								 // Время последнего кадра
 
int windowInitialWidth = 1200, windowInitialHeight = 1200; // Стартовые размеры окна
 
float lastCursorX = (float)windowInitialWidth / 2,
 	lastCursorY = (float)windowInitialHeight / 2;			 // Предыдущее положение курсора (стартовое - по центру окна)
 
bool firstMouse = true;

bool canMoveCamera = false;
 
Renderer* renderer = Renderer::getInstance();
Camera *mainCamera = renderer->getMainCamera();

Scene* mainScene;
Tank* playerTank;
TextString* debugTextString;
ParticleGenerator* particleGenerator;

bool wireframeMode = false;
 
float globalExposure = 1.0f;
float globalGamma = 2.2f;

void togglePolygoneDrawMode()
{
	wireframeMode = !wireframeMode;
}
 
void changeSpotlightState(SpotLight *spotLight)
{
	spotLight->setIsActive(!spotLight->getIsActive());
}
 
SpotLight *flashlight;
 
void changeFlashlightState()
{
	changeSpotlightState(flashlight);
}
 
struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};
 
// Callback функция, вызываемая при изменении размеров окна.
void OnResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	renderer->setScreenWidthAndHeight(width, height);
}
 
// Работа с устройствами ввода
void processInput(GLFWwindow* window)
	{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
 		glfwSetWindowShouldClose(window, true);
	}
	 
	// Перемещение камеры:
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
 		mainCamera->handleKeyboard(MovementDirection::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
 		mainCamera->handleKeyboard(MovementDirection::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
 		mainCamera->handleKeyboard(MovementDirection::STRAFE_LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
 		mainCamera->handleKeyboard(MovementDirection::STRAFE_RIGHT, deltaTime);
	}
	 
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
 		globalExposure = min( globalExposure + 1.0f * deltaTime, 10.0f );
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
 		globalExposure = max( globalExposure - 1.0f * deltaTime, 0.01f );
	}
	 
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
 		globalGamma = min( globalGamma + 0.5f * deltaTime, 5.0f );
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
 		globalGamma = max( globalGamma - 0.5f * deltaTime, 0.01f );
	}

	// Управление танком:
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
 		playerTank->offsetPosition( glm::vec3(0.0f, 0.0f, -1.0f) * 10.0f * deltaTime );
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
 		playerTank->offsetPosition( glm::vec3(-1.0f, 0.0f, 0.0f) * 10.0f * deltaTime );
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
 		playerTank->offsetPosition( glm::vec3(0.0f, 0.0f, 1.0f) * 10.0f * deltaTime );
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
 		playerTank->offsetPosition( glm::vec3(1.0f, 0.0f, 0.0f) * 10.0f * deltaTime );
	}

	// Поворот башни:
	if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
 		playerTank->offsetTurretRotationAngleDegrees( 30.0f * deltaTime );
	}
	if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
 		playerTank->offsetTurretRotationAngleDegrees( -30.0f * deltaTime );
	}

	// Поворот танка:
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
 		playerTank->offsetRotationAngleDegrees( 30.0f * deltaTime );
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
 		playerTank->offsetRotationAngleDegrees( -30.0f * deltaTime );
	}
 		
	// Поднятие-спуск камеры:
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
 		mainCamera->handleKeyboard(MovementDirection::UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
 		mainCamera->handleKeyboard(MovementDirection::DOWN, deltaTime);
	}
}

void blowTank(Tank* tank, ParticleGenerator* particleGenerator, const float& duration = 1.5f)
{
	ParticleGeneratorSettings pgs{
			tank->getPosition(),
			30, 100,
			0.01f, 0.1f, 0.05f,
			glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f),
			glm::vec3(0.75f),
			glm::vec3(0.0f), glm::vec3(10.0f)
	};
	ParticleGeneratorSettings pgs_end{
			tank->getPosition(),
			30, 200,
			0.3f, 3.5f, 1.0f,
			glm::vec3(0.05f), glm::vec3(0.0f),
			glm::vec3(0.75f),
			glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.25f)
	};

	particleGenerator->updateSettings(pgs);
	particleGenerator->activate();
	particleGenerator->addAnimation(Animations::changeParticleSettings(particleGenerator, duration, pgs, pgs_end));
	particleGenerator->addAnimation(Animations::deactivateIn(particleGenerator, duration));

	tank->addAnimation(Animations::blowTank(tank, duration / 2, 5.0f));
}
 
void onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
 		switch (key)
 		{
		case GLFW_KEY_F1:
			debugTextString->setVisible(!(debugTextString->isVisible()));
 			break;
		case GLFW_KEY_F2:
			mainScene->toggleCollidersDrawing();
			break;
		case GLFW_KEY_F3:
			canMoveCamera = !canMoveCamera;
 			if (canMoveCamera)
 			{
 				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Захватываем курсор
 			} else
 			{
 				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  // Захватываем курсор
 			}
			break;
 		case GLFW_KEY_F4:
			togglePolygoneDrawMode();
 			break;
 		case GLFW_KEY_F:
 			changeFlashlightState();
 			break;
		case GLFW_KEY_B:
			blowTank(playerTank, particleGenerator, 3.0f);
 			break;
		case GLFW_KEY_9:
			mainCamera->setPosition(glm::vec3(-7.3f, 40.0f, 17.0f));
 			mainCamera->setYawAndPitch(-90.0f, -50.0f);
 			break;
 		case GLFW_KEY_0:
			playerTank->setPosition(glm::vec3(0.0f));
 			playerTank->setRotationAngleDegrees(0.0f);
 			playerTank->setTurretRotationAngleDegrees(0.0f);
 			playerTank->setExplosionMagnitude(0.0f);
 			playerTank->setTimeSinceExplosion(0.0f);
 			playerTank->setVisible(true);
 			particleGenerator->deactivate();
 			break;
 		case GLFW_KEY_1:
			playerTank->setShader(ShaderLoader::getInstance()->getOrLoad("model_mixLightWithExplosion", true));
			break;
 		case GLFW_KEY_2:
			playerTank->setShader(ShaderLoader::getInstance()->reload("model_hotSwap", true));
			break;

 		// Поворот танка:
		case GLFW_KEY_UP:
			playerTank->setAnimation(Animations::rotateToAngle(playerTank, 0.15f, 180.0f), false);
			break;
 		case GLFW_KEY_DOWN:
			playerTank->setAnimation(Animations::rotateToAngle(playerTank, 0.15f, 0.0f), false);
			break;
 		case GLFW_KEY_LEFT:
			playerTank->setAnimation(Animations::rotateToAngle(playerTank, 0.15f, 270.0f), false);
			break;
 		case GLFW_KEY_RIGHT:
			playerTank->setAnimation(Animations::rotateToAngle(playerTank, 0.15f, 90.0f), false);
			break;
 		}
	}
}
 
// Обработка движения колёсика мыши
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	mainCamera->handleMouseScroll((float)yOffset);
}

void rotateTankTurretToWorldPoint(Tank* tank, const glm::vec3& worldPoint)
{
	glm::vec3 tankPosition = tank->getPosition();

	float tankBaseAngle = tank->getRotationAngleDegrees();
	float tankTurretAngle = tank->getTurretRotationAngleDegrees();

	float dx = worldPoint.x - tankPosition.x;
	float dz = worldPoint.z - tankPosition.z;

	float angleTang = dx / dz;

	float angleDegreeToSouthRay = glm::degrees(std::atan(angleTang));

	if (dz < 0.0f)
	{
		angleDegreeToSouthRay += 180.0f;
	}

	float rotationAngleRelativeToBase = angleDegreeToSouthRay - tankBaseAngle;
	tank->setAnimation(Animations::rotateTurretToAngle(tank, 0.25f, rotationAngleRelativeToBase));
}
 
// Обработка движения мыши
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (canMoveCamera)
	{
		if (firstMouse) {
 			lastCursorX = (float)xpos;
 			lastCursorY = (float)ypos;
 			firstMouse = false;
		}
		float dx = (float)xpos - lastCursorX,
 			  dy = lastCursorY - (float)ypos;
		lastCursorX = (float)xpos; lastCursorY = (float)ypos;
		 
		mainCamera->handleMouseMovement(dx, dy);
	} else {
		glm::vec3 worldPoint = mainCamera->screenCoordToWorldCoords(glm::vec2(xpos, ypos));
		rotateTankTurretToWorldPoint(playerTank, worldPoint);
	}
}

void onMouseClick(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:			
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			break;
		}
	}
}

int main()
{
	using namespace std;

	srand(141254);
 		
	#pragma region GLFW INIT
	// Настройка glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	 
	int windowWidth = windowInitialWidth, windowHeight = windowInitialHeight;
	 
	GLFWwindow* win = glfwCreateWindow(windowWidth, windowHeight, "OpenGL Window",
 		                                NULL, NULL);
	renderer = Renderer::getInstance();
	renderer->setScreenWidthAndHeight(windowWidth, windowHeight);
	 
	if (win == NULL)
	{
 		cout << "Error while creating the window.";
 		glfwTerminate();
 		return -1;
	}
	glfwMakeContextCurrent(win);
	 
	// Получение функций видеокарты
	if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
	{
 		cout << "Error while loading GLAD.";
 		glfwTerminate();
 		return -1;
	}
	 
	// glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Захватываем курсор
	glfwSetCursorPosCallback(win, mouseCallback);  // Функция, вызываемая при перемещении курсора
	glfwSetScrollCallback(win, scrollCallback);
	glfwSetKeyCallback(win, onKeyAction);
	glfwSetMouseButtonCallback(win, onMouseClick);
	 
	glfwSetFramebufferSizeCallback(win, OnResize);
	 
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_MULTISAMPLE);
	 
	// TODO: вернуть отбрасывание граней.
	// glEnable(GL_CULL_FACE);
	// Конец настройки glfw
	#pragma endregion

	// Загрузка шрифта:
	Font *arialFont = FontLoader::getInstance()->getOrLoad("arial", 24);
	Shader* fontShader = ShaderLoader::getInstance()->getOrLoad("font");
	debugTextString = new TextString(renderer, arialFont,
		string(
			"Player tank position: {%tank.x%, %tank.y%, %tank.z%}\n") + 
			"Camera position: {%camera.x%, %camera.y%, %camera.z%}\n" +
			"Camera rotations: yaw={%camera.yaw%}, pitch={%camera.pitch%}\n" +
			"\n" +
			"Controls:\n"+
			"F1 - Show or hide this information.\n" +
			"F2 - Show colliders.\n" +
			"F3 - Turn on/off camera free mouse movement.\n" +
			"F4 - Turn on/off wireframe mode.\n"
			"'W','A','S','D', 'Space', 'Shift' and mouse - Move and rotate camera.\n" +
			"Arrows - Mode player tank.\n" +
			"'<' and '>' - Rotate player tank's turret.\n" +
			"'K' and 'L' - Rotate player tank.\n" +
			"'Q' and 'E' - Increase/decrease exposure.\n" +
			"'Z' and 'C' - Increase/decrease gamma.\n" +
			"'F' - Turn on/off flashlight.\n"
			"'B' - Play exploding animation.\n" +
			"'9' - Restore camera position.\n"
			"'0' - Restore tank position and visibility.\n" +
			"'1' and '2' - Turn on default shader/hot-swappable shader.",
		  glm::vec2(20.0f, windowHeight / 2), 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));
	
	mainCamera->setViewSize({windowInitialWidth, windowInitialHeight});
	mainCamera->setPosition(glm::vec3(-7.3f, 40.0f, 17.0f));
 	mainCamera->setYawAndPitch(-90.0f, -50.0f);
	 
	// Загрузка внешних данных:
	Shader* screenRenderQuadShaderWithBlur = ShaderLoader::getInstance()->getOrLoad("screenRenderQuadShaderWithBlur");
	Shader* screenRenderQuadWithHDRShader = ShaderLoader::getInstance()->getOrLoad("screenRenderQuadShaderWithHDR");

	auto* level = LevelLoader::loadFromFile("level1.map", 16, 16);
	mainScene = level->generateScene(renderer, glm::vec3(0.0f));
	
	// Подготовка источников освещения:
	vector<PositionedLight*> positionedLights;
	 
	PointLight *pointLight = new PointLight(
 			glm::vec3(0.1f),
 			glm::vec3(2.0f, 2.0f, 0.2f),
 			glm::vec3(10.0f, 10.0f, 0.2f),
 			1.0f, 0.01f, 0.009f,
 			glm::vec3(0.0f, 10.0f, -10.0f),
 			"VERY bright yellow lamp"
 		);
	 
	renderer->addLight(pointLight);
	positionedLights.push_back(pointLight);

	pointLight = new PointLight(
 			glm::vec3(0.1f),
 			glm::vec3(0.2f, 2.0f, 2.0f),
 			glm::vec3(0.2f, 10.0f, 10.0f),
 			1.0f, 0.01f, 0.009f,
 			glm::vec3(-20.0f, 10.0f, -10.0f),
 			"VERY bright green lamp"
 		);
	 
	renderer->addLight(pointLight);
	positionedLights.push_back(pointLight);
 		
	flashlight = new SpotLight(
 		glm::radians(10.f), glm::radians(20.f),
 		glm::vec3(0.0f, 0.0f, 0.0f),
 		glm::vec3(0.0f, 0.0f, 0.0f),
 		glm::vec3(0.0f, 0.0f, 3.6f),
 		glm::vec3(0.0f, 0.0f, 8.7f),
 		1.0f, 0.05f, 0.009f,
 		glm::vec3(0.0f, 0.0f, 0.0f),
 		"FlashLight"
	);
	renderer->addLight(flashlight);
	
	playerTank = new Tank(renderer, {glm::vec3(-5.0f, 0.0f, -15.0f)});

	particleGenerator = new ParticleGenerator(renderer, 
		ModelTransformations{glm::vec3(-10.0f, 10.0f, -10.0f)},
		500, 25,
		0.025f,
		2.5f, 1.5f,
		glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(3.0f, 0.0f, 0.0f),
		glm::vec3(0.25f),
		glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(2.0f)
	);

	ParticleGenerator *xAxis = new ParticleGenerator(renderer, 
		ModelTransformations{glm::vec3(0.0f)},
		50, 1,
		0.05f,
		3.0f, 0.0f,
		glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)
	);
	xAxis->activate();
	ParticleGenerator *yAxis = new ParticleGenerator(renderer, 
		ModelTransformations{glm::vec3(0.0f)},
		50, 1,
		0.05f,
		3.0f, 0.0f,
		glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
	);
	yAxis->activate();
	ParticleGenerator *zAxis = new ParticleGenerator(renderer, 
		ModelTransformations{glm::vec3(0.0f)},
		50, 1,
		0.05f,
		3.0f, 0.0f,
		glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f)
	);
	zAxis->activate();
	 
	Ground ground(renderer, "grass.png", "grass_specular.png", 100.f);
	Skybox skybox(renderer, "skybox", "sky.jpg");
	 
	mainScene->addSkybox(&skybox);
	
	mainScene->addTank(playerTank);
	
	mainScene->addDrawableObject(&ground);
	
	for (auto *positionedLight : positionedLights)
	{
 		mainScene->addDrawableObject(new LightCube(renderer, positionedLight));
	}
	
	mainScene->addDrawableUpdatableObject(particleGenerator);
	mainScene->addDrawableUpdatableObject(xAxis);
	mainScene->addDrawableUpdatableObject(yAxis);
	mainScene->addDrawableUpdatableObject(zAxis);
	 
	#pragma region Инициализация Framebuffer-а и разных VAO
	 
	// Рендеринг в текстуру:
	unsigned frameBuffer;
	// Создание фреймбуффера:
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	 
	// Создание текстуры, на котороую будут выводится рендер frameBuffer-а.
	unsigned frameBufferColorTexturesID[2];
	glGenTextures(2, frameBufferColorTexturesID);
	for(unsigned i = 0; i < 2; i++)
	{
 		glBindTexture(GL_TEXTURE_2D, frameBufferColorTexturesID[i]);
 		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 		// Присоединяем созданную текстуру к framebuffer
 		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, frameBufferColorTexturesID[i], 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	 
	unsigned attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, attachments);
 		
	// Создание render-буфера для depth и stencil тестов:
	// Render-буфер работает в режиме write-only.
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	 
	// Присоединяем созданный buffer к framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
 		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	 
	// Переключаемся на стандартный framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	 
	// Создание framebuffer-ов для размытия Гаусса:
	unsigned pingpongFrameBuffers[2];
	unsigned pingpongTextureIds[2];
	glGenFramebuffers(2, pingpongFrameBuffers);
	glGenTextures(2, pingpongTextureIds);
	for(unsigned i = 0; i < 2; i++)
	{
 		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFrameBuffers[i]);
 		glBindTexture(GL_TEXTURE_2D, pingpongTextureIds[i]);
 		glTexImage2D(
 		    GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL
 		);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 		glFramebufferTexture2D(
 		    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTextureIds[i], 0
 		);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#pragma endregion
 		
	VOsAndIndices *screenQuadVOsAndIndices = VAOBuilder::getInstance()->get2DQuad();
 		
	#pragma endregion

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	while (!glfwWindowShouldClose(win))
	{
 		float currentTime = (float)glfwGetTime();
 		deltaTime = currentTime - lastFrameTime;
 		lastFrameTime = currentTime;
	 
 		processInput(win);
	 
 		// Update некоторых объектов сцены:
 		flashlight->setPosition(mainCamera->position
 			- mainCamera->up * 0.3f
 		);
 		flashlight->setDirection(mainCamera->front);

		// Проверка столкновений:
		mainScene->update(currentTime);
		
		// Подготовка к отрисовке:
		renderer->updateViewAndProjection();
	 
 		// Отрисовка в framebuffer:
 		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
 		glEnable(GL_DEPTH_TEST);

		// Переключение wireframeMode:
		if (wireframeMode) {
 			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
 			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	 
 		mainScene->draw();
 		// Конец отрисовки в framebuffer.

 		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	 
 		// Отрисовка в frambuffer для размытия:
 		bool horizontal = true, firstIteration = true;
 		unsigned amount = 5;
 		screenRenderQuadShaderWithBlur->use();
 		for(unsigned i = 0; i < amount * 2; i++)
 		{
 			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFrameBuffers[horizontal]);
 			screenRenderQuadShaderWithBlur->setInt("horizontal", horizontal);
 			glBindTexture(GL_TEXTURE_2D, firstIteration ? frameBufferColorTexturesID[1] : pingpongTextureIds[!horizontal]);
	 
 			glBindVertexArray(screenQuadVOsAndIndices->vao);
 			glDrawElements(GL_TRIANGLES, screenQuadVOsAndIndices->indices.size(), GL_UNSIGNED_INT, NULL);
 				
 			horizontal = !horizontal;
 			if (firstIteration)
 			{
 				firstIteration = false;
 			}
 		}
 			
 		// Отрисовка в стандартный framebuffer quad-а, занимающего весь экран:
 		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	 
 		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 			
 		screenRenderQuadWithHDRShader->use();
 		screenRenderQuadWithHDRShader->setFloat("exposure", globalExposure);
 		screenRenderQuadWithHDRShader->setFloat("gamma", globalGamma);
 		glDisable(GL_DEPTH_TEST);
 		glActiveTexture(GL_TEXTURE0);
 		glBindTexture(GL_TEXTURE_2D, frameBufferColorTexturesID[0]);
 		screenRenderQuadWithHDRShader->setInt("hdrBuffer", 0);
 		glActiveTexture(GL_TEXTURE1);
 		glBindTexture(GL_TEXTURE_2D, pingpongTextureIds[0]);
 		screenRenderQuadWithHDRShader->setInt("blurBuffer", 1);
 		glBindVertexArray(screenQuadVOsAndIndices->vao);
 		glDrawElements(GL_TRIANGLES, screenQuadVOsAndIndices->indices.size(), GL_UNSIGNED_INT, NULL);

		// Отрисовка текста:

		debugTextString->setKeysValues({
			{"tank.x", to_string(playerTank->getPosition().x)},
			{"tank.y", to_string(playerTank->getPosition().y)},
			{"tank.z", to_string(playerTank->getPosition().z)},
			{"camera.x", to_string(mainCamera->position.x)},
			{"camera.y", to_string(mainCamera->position.y)},
			{"camera.z", to_string(mainCamera->position.z)},
			{"camera.yaw", to_string(mainCamera->yaw)},
			{"camera.pitch", to_string(mainCamera->pitch)},
		});
		debugTextString->draw();
		
 		// Конец отрисовки в стандартный framebuffer.
	 
 		glfwSwapBuffers(win);
 		glfwPollEvents();
	}
	 
	glfwTerminate();
	return 0;
}
