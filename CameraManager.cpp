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
	em->subscribe(this, "startFrame");
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

		mainCamera->view = obj.transform;
		mainCamera->proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);

		uint32_t argKey = eventSystem::getEventType("mainCamera");
		em->publish({
			eventSystem::getEventType("mainCameraSet"), {argKey, {mainCamera}}
			});
		break;
	}
	case eventSystem::getEventType("startFrame"):
	{
		float deltaTime = std::get<float>(event.getArg("deltaTime").value);
		mainCamera->update(deltaTime);
		break;
	}
	default:
		std::cerr << "unkown event heard by camera manager: " << event.type << std::endl;
		break;
	}
}

void Camera::update(float deltaTime)
{
	glm::quat facing = glm::quat_cast(view);
	view = glm::translate(view, glm::vec3( 0, 0, deltaTime * 0.000001f) * facing);
}
