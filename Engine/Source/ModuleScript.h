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

	TransformComponent* tankGoTransform;
	TransformComponent* turretGoTransform;
	TransformComponent* transform;

	float velocity = 0.0f;
	float acceleration = 4.0f;
	float maxVelocityForward = 40.0f;
	float maxVelocityBackward = -12.0f;
private:
	GameObject* baseGO = nullptr;
};