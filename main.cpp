#include "Renderer.h"



int main()
{
	// Todo: make main loop into listener class that listens for appQuit events.
	// Then any module can easily trigger a shutdown rather than relying 
	// on a hardcoded quit condition.
	Renderer renderer;
	if (renderer.init() == -1) return -1;

	while (!renderer.shouldQuit())
	{
		renderer.draw_frame();
	}

	renderer.cleanUp();
}
