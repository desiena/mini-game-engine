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
	em->subscribe(this, "keyPressed:moveForward");
	em->subscribe(this, "keyPressed:moveBack");
	em->subscribe(this, "keyPressed:moveLeft");
	em->subscribe(this, "keyPressed:moveRight");
	em->subscribe(this, "axisInput:turn");
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
	case eventSystem::getEventType("startFrame"):
	{
		float deltaTime = std::get<float>(event.getArg("deltaTime").value);
		break;
	}
	// ToDo: move to a controller.
	case eventSystem::getEventType("keyPressed:moveForward"):
	{
		float deltaTime = std::get<float>(event.getArg("deltaTime").value);
		transformManager->moveForward(mainCamera->transform, deltaTime);
		break;
	}
	case eventSystem::getEventType("keyPressed:moveBack"):
	{
		float deltaTime = std::get<float>(event.getArg("deltaTime").value);
		transformManager->moveBack(mainCamera->transform, deltaTime);
		break;
	}
	case eventSystem::getEventType("keyPressed:moveLeft"):
	{
		float deltaTime = std::get<float>(event.getArg("deltaTime").value);
		transformManager->moveLeft(mainCamera->transform, deltaTime);
		break;
	}
	case eventSystem::getEventType("keyPressed:moveRight"):
	{
		float deltaTime = std::get<float>(event.getArg("deltaTime").value);
		transformManager->moveRight(mainCamera->transform, deltaTime);
		break;
	}
	case eventSystem::getEventType("axisInput:turn"):
	{
		int x = std::get<int>(event.getArg("x").value);
		int y = std::get<int>(event.getArg("y").value);
		transformManager->turn(mainCamera->transform, x, y);
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
