#pragma once

#include "EventManager.h"
#include "TransformManager.h"

class FlythroughPlayerController : public eventSystem::Listener
{
public:
	uint32_t hashedName;
	Transform* transform;
	FlythroughPlayerController(TransformManager* tm, uint32_t objID);
	void registerSubscriptions(eventSystem::EventManager* em);
private:
	TransformManager* transformManager;
	virtual void handleEvent(eventSystem::Event event) override;
};

