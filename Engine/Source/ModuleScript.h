#pragma once

#include "Module.h"

class GameObject;
class TransformComponent;

class ModuleScript : public Module
{
public:
	ModuleScript();
	~ModuleScript();

	bool Start() override;
	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;	

	void Move();
	void Rotate();
	void Shoot();

	TransformComponent* turretGoTransform;
	TransformComponent* chassisGoTransform;
	TransformComponent* rightWheelsGoTransform;
	TransformComponent* leftWheelsGoTransform;

	float angle = 0.0f;
	float rotateVelocity = 50.0f;


	float velocity = 0.0f;
	float acceleration = 2.5f;
	float maxVelocityForward = 30.0f;
	float maxVelocityBackward = -10.0f;
	float friction = 0.03f;
private:
	GameObject* baseGO = nullptr;
};