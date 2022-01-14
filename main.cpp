#include "Renderer.h"
#include "EventManager.h"

#include <iostream>

int main()
{
	// Todo: make main loop into GameManager class that listens for appQuit events.
	// Then any module can easily trigger a shutdown rather than relying 
	// on a hardcoded quit condition.

	eventSystem::EventManager eventManger;

	Renderer renderer;
	if (renderer.init() == -1) return -1;

	eventManger.subscribe(&renderer, "startFrame");

	while (!renderer.shouldQuit())
	{
		eventManger.publish({ eventSystem::getEventType("startFrame") });
	}

	renderer.cleanUp();
}
