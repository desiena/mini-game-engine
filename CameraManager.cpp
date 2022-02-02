#include "CameraManager.h"

#include <iostream>

int CameraManager::init(eventSystem::EventManager* em)
{
    this->em = em;
	mainCamera = new Camera{};
	uint32_t argKey = eventSystem::getEventType("mainCamera");
	em->publish({
		eventSystem::getEventType("mainCameraSet"), {argKey, {mainCamera}}
	});
    return 0;
}

void CameraManager::registerSubscriptions(eventSystem::EventManager* em)
{
	em->subscribe(this, "swapchainCreated");
}

void CameraManager::handleEvent(eventSystem::Event event)
{
	switch (event.type)
	{
	case eventSystem::getEventType("swapchainCreated"):
	{
		for (eventSystem::EventArg arg : event.args)
		{
			if (arg.key == eventSystem::getEventType("aspectRatio"))
			{
				mainCamera->setAspectRatio(std::get<float>(arg.value));
				break;
			}
		}
		break;
	}
	default:
		std::cerr << "unkown event heard by camera manager: " << event.type << std::endl;
		break;
	}
}

void Camera::setAspectRatio(float aspectRatio)
{
	view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
}
