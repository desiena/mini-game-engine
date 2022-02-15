#pragma once

#include "EventManager.h"
#include "SceneManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/hash.hpp>

#include <iostream>

struct Transform
{
	uint32_t hashedName;
	glm::quat rotation;
	glm::vec3 scale;
	glm::vec3 translation;
	glm::mat4 transform;
};


class TransformManager : public eventSystem::Listener
{
public:
	int init(eventSystem::EventManager* em);
	void registerSubscriptions(eventSystem::EventManager* em);
	Transform* getObjectByID(uint32_t objID);
	SceneManager* sceneManager;
private:
	std::unordered_map<uint32_t, Transform*> transforms;
	virtual void handleEvent(eventSystem::Event event) override;
	void updateMatrix(Transform* transform);
};

