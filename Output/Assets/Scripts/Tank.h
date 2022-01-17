#pragma once

#include "Module.h"

class GameObject;
class TransformComponent;

class Tank : public Module
{
public:
	Tank();
	~Tank();

	bool Start() override;
	bool Update(float dt) override;

	void Move();
	void Rotate();
	void Shoot();


private:
	TransformComponent* turretGoTransform;
	TransformComponent* tankGoTransform;
	TransformComponent* chassisGoTransform;
	TransformComponent* rightWheelsGoTransform;
	TransformComponent* leftWheelsGoTransform;
	TransformComponent* bulletGoTransform = nullptr;

	float angle = 0.0f;
	float rotateVelocity = 50.0f;

	float3 bulletDir = { 0,0,0 };
	float bulletVelocity = 20.0f;

	float velocity = 0.0f;
	float acceleration = 2.5f;
	float maxVelocityForward = 5.0f;
	float maxVelocityBackward = -5.0f;
	float friction = 0.03f;

	bool shoot = false;
	bool bulletAlive = false;

	GameObject* tankGo;
	GameObject* object = nullptr;
};