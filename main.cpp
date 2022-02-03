#include "Renderer.h"
#include "EventManager.h"
#include "SceneManager.h"

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

	// Todo: add system lookup via game manager to avoid this
	renderer.sceneManager = &sceneManager;

	renderer.registerSubscriptions(&eventManager);
	cameraManager.registerSubscriptions(&eventManager);

	if (cameraManager.init(&eventManager) == -1) return -1;
	if (renderer.init(&eventManager) == -1) return -1;
	if (sceneManager.init(&eventManager) == -1) return -1;

	auto startTime = std::chrono::high_resolution_clock::now();
	while (!renderer.shouldQuit())
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		eventManager.publish({
			eventSystem::getEventType("startFrame"),
			{eventSystem::getEventType("deltaTime"), deltaTime}
		});
	}

	renderer.cleanUp();
}
