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
			{ rotation[0], rotation[1], rotation[2], rotation[3] },
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
	transform->transform = glm::scale(glm::mat4(1.f), transform->scale);
	transform->transform *= glm::mat4_cast(transform->rotation);
	transform->transform = glm::translate(transform->transform, transform->translation);
}
