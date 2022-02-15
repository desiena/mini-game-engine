#include "Renderer.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "InputManager.h"

#include <iostream>
#include <chrono>

int main()
{
	// Todo: make main loop into GameManager class that listens for appQuit events.
	// Then any module can easily trigger a shutdown rather than relying 
	// on a hardcoded quit condition.

	eventSystem::EventManager eventManager;
	Renderer renderer;
	CameraManager cameraManager;
	SceneManager sceneManager;
	InputManager inputManager;
	TransformManager transformManager;

	// Todo: add system lookup via game manager to avoid this
	renderer.sceneManager = &sceneManager;
	renderer.transformManager = &transformManager;
	cameraManager.sceneManager = &sceneManager;
	transformManager.sceneManager = &sceneManager;

	transformManager.registerSubscriptions(&eventManager);
	renderer.registerSubscriptions(&eventManager);
	cameraManager.registerSubscriptions(&eventManager);
	inputManager.registerSubscriptions(&eventManager);

	if (cameraManager.init(&eventManager) == -1) return -1;
	if (renderer.init(&eventManager) == -1) return -1;
	if (sceneManager.init(&eventManager) == -1) return -1;
	if (inputManager.init(&eventManager) == -1) return -1;

	auto startTime = std::chrono::high_resolution_clock::now();
	auto lastTime = startTime;
	float measurementsPerSecond = 20;
	int fps = 0;
	while (!inputManager.shouldQuit())
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
		lastTime = currentTime;
		fps++;
		if (currentTime - startTime > std::chrono::milliseconds((long)(1000.f/measurementsPerSecond)))
		{
			std::cout << fps * measurementsPerSecond << std::endl;
			fps = 0;
			startTime = currentTime;
		}
		eventManager.publish({
			eventSystem::getEventType("startFrame"),
			{eventSystem::getEventType("deltaTime"), deltaTime}
		});
	}

	renderer.cleanUp();
}
