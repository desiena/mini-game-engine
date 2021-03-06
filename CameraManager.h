#pragma once
#include "EventManager.h"
#include "SceneManager.h"
#include "TransformManager.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/hash.hpp>


class Camera
{
public:
	uint32_t hashedName;
	Transform* transform;

	glm::mat4 view;
	glm::mat4 proj;
};

class CameraManager : public eventSystem::Listener
{
public:
	SceneManager* sceneManager;
	TransformManager* transformManager;

	int init(eventSystem::EventManager* em);
	void registerSubscriptions(eventSystem::EventManager* em);
	void linkObjects();
	void setAspectRatio(float aspectRatio);
	static void updateView(Camera* camera);
private:
	float aspectRatio;
	Camera* mainCamera;
	eventSystem::EventManager* em;

	virtual void handleEvent(eventSystem::Event event) override;
};

