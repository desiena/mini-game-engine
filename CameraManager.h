#pragma once
#include "EventManager.h"
#include "SceneManager.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/hash.hpp>


class Camera
{
public:
	glm::mat4 view;
	glm::mat4 proj;

	void update(float deltaTime);
};

class CameraManager : public eventSystem::Listener
{
public:
	SceneManager* sceneManager;

	int init(eventSystem::EventManager* em);
	void registerSubscriptions(eventSystem::EventManager* em);
	void setAspectRatio(float aspectRatio);
private:
	float aspectRatio;
	Camera* mainCamera;
	eventSystem::EventManager* em;


	virtual void handleEvent(eventSystem::Event event) override;
};

