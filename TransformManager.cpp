#include "TransformManager.h"

int TransformManager::init(eventSystem::EventManager* em)
{
	return 0;
}

void TransformManager::registerSubscriptions(eventSystem::EventManager* em)
{
	em->subscribe(this, "componentAdded:transform");
}

Transform* TransformManager::getObjectByID(uint32_t objID)
{
	return transforms[objID];
}

void TransformManager::handleEvent(eventSystem::Event event)
{
	switch (event.type)
	{
	case eventSystem::getEventType("componentAdded:transform"):
	{
		auto objIDArg = event.getArg("objectID");
		uint32_t objID = std::get<uint32_t>(objIDArg.value);
		SceneObject obj = sceneManager->getObjectByID(objID);
		// ToDo: clean this up. SceneObject.getDataField?
		auto transformData = obj.getComponentData("transform");
		std::array<float, 3> translation = std::get<std::array<float, 3>>(transformData[eventSystem::getEventType("translation")]);
		std::array<float, 3> scale = std::get<std::array<float, 3>>(transformData[eventSystem::getEventType("scale")]);
		std::array<float, 4> rotation = std::get<std::array<float, 4>>(transformData[eventSystem::getEventType("rotation")]);
		Transform* transform = new Transform{
			objID,
			{ rotation[3], rotation[0], rotation[1], rotation[2] },
			{ scale[0], scale[1], scale[2] },
			{ translation[0], translation[1], translation[2] }
		};
		updateMatrix(transform);
		transforms[objID] = transform;
		break;
	}
	default:
		std::cerr << "unknown event heard by Transform Manager: " << event.type << std::endl;
		break;
	}
}

void TransformManager::updateMatrix(Transform* transform)
{
	transform->transform = glm::translate(glm::mat4(1.f), transform->translation);
	transform->transform *= glm::mat4_cast(transform->rotation);
	transform->transform = glm::scale(transform->transform, transform->scale);
}

void TransformManager::moveForward(Transform* transform, float distance)
{
	transform->translation += transform->rotation * glm::vec3(0, 0, distance * -1.0f);
	updateMatrix(transform);
}

void TransformManager::moveBack(Transform* transform, float distance)
{
	transform->translation += transform->rotation * glm::vec3(0, 0, distance * 1.0f);
	updateMatrix(transform);
}

void TransformManager::moveLeft(Transform* transform, float distance)
{
	transform->translation += transform->rotation * glm::vec3(distance * -1.0f, 0, 0);
	updateMatrix(transform);
}

void TransformManager::moveRight(Transform* transform, float distance)
{
	transform->translation += transform->rotation * glm::vec3(distance * 1.0f, 0, 0);
	updateMatrix(transform);
}

void TransformManager::turn(Transform* transform, float x, float y)
{
	transform->rotation = glm::rotate(transform->rotation, glm::radians(x * -0.1f), glm::vec3(0.0f, 0.0f, 1.0f) * transform->rotation);
	transform->rotation = glm::rotate(transform->rotation, glm::radians(y * -0.1f), glm::vec3(1, 0, 0));
	updateMatrix(transform);
}
