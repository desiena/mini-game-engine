#include "SceneManager.h"

int SceneManager::init(eventSystem::EventManager* em)
{
    eventManager = em;
    // ToDo: load yaml and add data to event.
    eventManager->publish({
        eventSystem::getEventType("gameObjectIntroduced"),
        {{eventSystem::getEventType("objectID"), eventSystem::getEventType("vikingRoom")}}
        });
    return 0;
}

void SceneManager::registerSubscriptions(eventSystem::EventManager* em)
{
}

SceneObject SceneManager::getObjectByID(uint32_t objID)
{
    return {
        "vikingRoom",
        eventSystem::getEventType("vikingRoom"),
        glm::mat4(1.0f),
        {
            {eventSystem::getEventType("modelPath"), "models/viking_room.obj"},
            {eventSystem::getEventType("texturePath"), "textures/viking_room.png"},
        }
    };
}

void SceneManager::handleEvent(eventSystem::Event event)
{
}
