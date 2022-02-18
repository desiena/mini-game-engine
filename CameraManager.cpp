#include "CameraManager.h"

#include <iostream>

int CameraManager::init(eventSystem::EventManager* em)
{
    this->em = em;
	mainCamera = new Camera{};
    return 0;
}

void CameraManager::registerSubscriptions(eventSystem::EventManager* em)
{
	em->subscribe(this, "swapchainCreated");
	em->subscribe(this, "componentAdded:camera");
}

void CameraManager::linkObjects()
{
	mainCamera->transform = transformManager->getObjectByID(mainCamera->hashedName);
}

void CameraManager::setAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
}

void CameraManager::handleEvent(eventSystem::Event event)
{
	switch (event.type)
	{
	case eventSystem::getEventType("swapchainCreated"):
	{
		auto aspectArg = event.getArg("aspectRatio");
		setAspectRatio(std::get<float>(aspectArg.value));
		break;
	}
	case eventSystem::getEventType("componentAdded:camera"):
	{
		auto objIDArg = event.getArg("objectID");
		uint32_t objID = std::get<uint32_t>(objIDArg.value);
		SceneObject obj = sceneManager->getObjectByID(objID);

		// ToDo: serialize proj
		mainCamera->proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
		mainCamera->hashedName = objID;

		uint32_t argKey = eventSystem::getEventType("mainCamera");
		em->publish({
			eventSystem::getEventType("mainCameraSet"), {argKey, {mainCamera}}
			});
		break;
	}
	default:
		std::cerr << "unknown event heard by camera manager: " << event.type << std::endl;
		break;
	}
}

void CameraManager::updateView(Camera* camera)
{
	camera->view = glm::inverse(camera->transform->transform);
}
