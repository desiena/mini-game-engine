#include "MiscComponentManager.h"

void MiscComponentManager::registerSubscriptions(eventSystem::EventManager* em)
{
	em->subscribe(this, "componentAdded:flythroughPlayerController");
}

void MiscComponentManager::linkObjects()
{
	fpc->transform = transformManager->getObjectByID(fpc->hashedName);
}

void MiscComponentManager::handleEvent(eventSystem::Event event)
{
	switch (event.type)
	{
	case eventSystem::getEventType("componentAdded:flythroughPlayerController"):
	{
		auto objIDArg = event.getArg("objectID");
		uint32_t objID = std::get<uint32_t>(objIDArg.value);
		FlythroughPlayerController* fpc = new FlythroughPlayerController{transformManager, objID};
		fpc->registerSubscriptions(eventManager);
		this->fpc = fpc;
		break;
	}
	default:
		std::cerr << "unknown event heard by misc component manager: " << event.type << std::endl;
		break;
	}
}
