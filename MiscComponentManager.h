#pragma once

#include "EventManager.h"
#include "FlythroughPlayerController.h"

#include <iostream>
#include <vector>

class MiscComponentManager : public eventSystem::Listener
{
public:
	eventSystem::EventManager* eventManager;
	TransformManager* transformManager;

	void registerSubscriptions(eventSystem::EventManager* em);
	void linkObjects();
private:
	virtual void handleEvent(eventSystem::Event event) override;
	FlythroughPlayerController* fpc;
};

