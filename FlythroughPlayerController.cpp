#include "FlythroughPlayerController.h"

FlythroughPlayerController::FlythroughPlayerController(TransformManager* tm, uint32_t objID)
	: transformManager(tm),
	hashedName(objID)
{}

void FlythroughPlayerController::registerSubscriptions(eventSystem::EventManager* em)
{
	em->subscribe(this, "keyPressed:moveForward");
	em->subscribe(this, "keyPressed:moveBack");
	em->subscribe(this, "keyPressed:moveLeft");
	em->subscribe(this, "keyPressed:moveRight");
	em->subscribe(this, "axisInput:turn");
}

void FlythroughPlayerController::handleEvent(eventSystem::Event event)
{
	switch (event.type)
	{
	case eventSystem::getEventType("keyPressed:moveForward"):
	{
		float deltaTime = std::get<float>(event.getArg("deltaTime").value);
		transformManager->moveForward(transform, deltaTime);
		break;
	}
	case eventSystem::getEventType("keyPressed:moveBack"):
	{
		float deltaTime = std::get<float>(event.getArg("deltaTime").value);
		transformManager->moveBack(transform, deltaTime);
		break;
	}
	case eventSystem::getEventType("keyPressed:moveLeft"):
	{
		float deltaTime = std::get<float>(event.getArg("deltaTime").value);
		transformManager->moveLeft(transform, deltaTime);
		break;
	}
	case eventSystem::getEventType("keyPressed:moveRight"):
	{
		float deltaTime = std::get<float>(event.getArg("deltaTime").value);
		transformManager->moveRight(transform, deltaTime);
		break;
	}
	case eventSystem::getEventType("axisInput:turn"):
	{
		int x = std::get<int>(event.getArg("x").value);
		int y = std::get<int>(event.getArg("y").value);
		transformManager->turn(transform, x, y);
		break;
	}
	default:
		std::cerr << "unknown event heard by flythrough player controller: " << event.type << std::endl;
		break;
	}
}
