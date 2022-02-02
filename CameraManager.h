#pragma once
#include "EventManager.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>


class Camera
{
public:
	glm::mat4 view;
	glm::mat4 proj;
	void setAspectRatio(float aspectRatio);
};

class CameraManager : public eventSystem::Listener
{
public:
	int init(eventSystem::EventManager* em);
	void registerSubscriptions(eventSystem::EventManager* em);
private:
	Camera* mainCamera;
	eventSystem::EventManager* em;


	virtual void handleEvent(eventSystem::Event event) override;
};

