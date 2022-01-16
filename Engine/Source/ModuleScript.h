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

	TransformComponent* tankGoTransform;
	TransformComponent* turretGoTransform;
	TransformComponent* tankChassisGoTransform;

	float angle = 0.0f;
	float rotateVelocity = 1.0f;


	float velocity = 0.0f;
	float acceleration = 0.5f;
	float maxVelocityForward = 40.0f;
	float maxVelocityBackward = -12.0f;
private:
	GameObject* baseGO = nullptr;
};