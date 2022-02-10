#pragma once

#include "EventManager.h"

#include <iostream>
#include <vector>
#include <cassert>

#define SDL_MAIN_HANDLED
#include <SDL_events.h>
#include <SDL.h>

struct BoundKey
{
	SDL_Keycode key;
	std::string eventName;
	bool keyDown = false;
};

class InputManager : public eventSystem::Listener
{
public:
	void registerSubscriptions(eventSystem::EventManager* em);
	int init(eventSystem::EventManager* em);
	bool shouldQuit();
private:
	bool quitSignalled = false;
	SDL_Event sdlEvent;
	std::vector<BoundKey*> boundKeys;
	eventSystem::EventManager* eventManager;

	virtual void handleEvent(eventSystem::Event event) override;
	void publishKeyEvent(Uint8 type, BoundKey* key);
};

