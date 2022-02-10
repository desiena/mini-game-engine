#include "InputManager.h"

void InputManager::registerSubscriptions(eventSystem::EventManager* em)
{
	em->subscribe(this, "startFrame");
}

int InputManager::init(eventSystem::EventManager* em)
{
	eventManager = em;
	BoundKey* bk = new BoundKey{ SDLK_w, "moveForward" };
	boundKeys.push_back(bk);
	bk = new BoundKey{ SDLK_s, "moveBack" };
	boundKeys.push_back(bk);
	bk = new BoundKey{ SDLK_a, "moveLeft" };
	boundKeys.push_back(bk);
	bk = new BoundKey{ SDLK_d, "moveRight" };
	boundKeys.push_back(bk);
	return 0;
}

bool InputManager::shouldQuit()
{
	return quitSignalled;
}

void InputManager::handleEvent(eventSystem::Event event)
{
	switch (event.type)
	{
	case eventSystem::getEventType("startFrame"):
	{
		float deltaTime = std::get<float>(event.getArg("deltaTime").value);
		while (SDL_PollEvent(&sdlEvent)) {
			switch (sdlEvent.type) {
			case SDL_QUIT:
				quitSignalled = true;
				break;
			case SDL_KEYDOWN:
				for (BoundKey* bk : boundKeys)
				{
					if (bk->key == sdlEvent.key.keysym.sym)
					{
						bk->keyDown = true;
						break;
					}
				}
				break;

			case SDL_KEYUP:
				for (BoundKey* bk : boundKeys)
				{
					if (bk->key == sdlEvent.key.keysym.sym)
					{
						bk->keyDown = false;
						break;
					}
				}
				break;

			default:
				break;
			}
		}
		for (BoundKey* bk : boundKeys)
		{
			if (bk->keyDown)
			{
				eventManager->publish({
					eventSystem::getEventType("keyPressed:" + bk->eventName),
					{eventSystem::getEventType("deltaTime"), deltaTime}
					});
			}
		}
		break;
	}
	default:
		std::cerr << "unknown event heard by input manager: " << event.type << std::endl;
		break;
	}
}

void InputManager::publishKeyEvent(Uint8 type, BoundKey* key)
{
}
