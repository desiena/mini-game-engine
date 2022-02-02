#include "Renderer.h"
#include "EventManager.h"

#include <iostream>

int main()
{
	// Todo: make main loop into GameManager class that listens for appQuit events.
	// Then any module can easily trigger a shutdown rather than relying 
	// on a hardcoded quit condition.

	eventSystem::EventManager eventManager;
	Renderer renderer;
	CameraManager cameraManager;

	renderer.registerSubscriptions(&eventManager);
	cameraManager.registerSubscriptions(&eventManager);

	if (cameraManager.init(&eventManager) == -1) return -1;
	if (renderer.init(&eventManager) == -1) return -1;

	while (!renderer.shouldQuit())
	{
		eventManager.publish({ eventSystem::getEventType("startFrame") });
	}

	renderer.cleanUp();
}
