#pragma once
#include "EventManager.h"

#include <vector>
#include <unordered_map>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <nlohmann/json.hpp>

struct SceneObject
{
	std::string name;
	uint32_t hashedName;
	glm::mat4 transform;
	std::unordered_map<uint32_t, std::variant<bool, uint64_t, uint32_t, std::string, float>> serializationData;
};

class SceneManager : public eventSystem::Listener
{
public:
	int init(eventSystem::EventManager* em);
	void registerSubscriptions(eventSystem::EventManager* em);
	SceneObject getObjectByID(uint32_t objID);
private:
	eventSystem::EventManager* eventManager;
	std::unordered_map<uint32_t, SceneObject> sceneObjects;

	virtual void handleEvent(eventSystem::Event event) override;
	glm::mat4 extractTransform(nlohmann::json obj);
};

