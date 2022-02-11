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

	glm::mat4 rotation = glm::mat4(1.f);
	glm::mat4 translation = glm::mat4(1.f);;

	void update(float deltaTime);
	void moveForward(float deltaTime);
	void moveBack(float deltaTime);
	void moveLeft(float deltaTime);
	void moveRight(float deltaTime);
	void turn(int x, int y);
private:
	void updateView();
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

