#include "SceneManager.h"

int SceneManager::init(eventSystem::EventManager* em)
{
    eventManager = em;

    // ToDo: add validation
    SceneObject so{};

    nlohmann::json sceneJSON;
    std::ifstream i("scenes/scene.json");
    i >> sceneJSON;

    for (auto& obj : sceneJSON)
    {
        so.name = obj["name"].get<std::string>();
        so.hashedName = eventSystem::getEventType(so.name);
        so.transform = extractTransform(obj);
        for (auto& [component, data] : obj["componentData"].items())
        {
            for (auto& [key, value] : data.items())
            {
                so.sceneData[eventSystem::getEventType(component)][eventSystem::getEventType(key)] = value;
            }
        }

        sceneObjects[so.hashedName] = so;

        for (auto& [component, data] : obj["componentData"].items())
        {
            eventManager->publish({
                eventSystem::getEventType("componentAdded:" + component),
                {{eventSystem::getEventType("objectID"), so.hashedName}}
                });
        }
    }
    return 0;
}

void SceneManager::registerSubscriptions(eventSystem::EventManager* em)
{
}

SceneObject SceneManager::getObjectByID(uint32_t objID)
{
    return sceneObjects[objID];
}

void SceneManager::handleEvent(eventSystem::Event event)
{
}

glm::mat4 SceneManager::extractTransform(nlohmann::json obj)
{
    // ToDo: Move to srt representation for better readability.
    // ToDo: verify row major ordering
    glm::vec4 row0 = {
        obj["transform"][0][0],
        obj["transform"][0][1],
        obj["transform"][0][2],
        obj["transform"][0][3],
    };
    glm::vec4 row1 = {
        obj["transform"][1][0],
        obj["transform"][1][1],
        obj["transform"][1][2],
        obj["transform"][1][3],
    };
    glm::vec4 row2 = {
        obj["transform"][2][0],
        obj["transform"][2][1],
        obj["transform"][2][2],
        obj["transform"][2][3],
    };
    glm::vec4 row3 = {
        obj["transform"][3][0],
        obj["transform"][3][1],
        obj["transform"][3][2],
        obj["transform"][3][3],
    };
    return glm::mat4{row0, row1, row2, row3};
}

std::unordered_map<uint32_t, std::variant<bool, uint64_t, uint32_t, std::string, float>> SceneObject::getComponentData(std::string componentName)
{
    return sceneData[eventSystem::getEventType(componentName)];
}
