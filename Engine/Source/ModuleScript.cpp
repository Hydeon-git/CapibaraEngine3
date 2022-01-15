#include "Application.h"
#include "Globals.h"
#include "Time.h"

// Modules
#include "ModuleScript.h"
#include "ModuleScene.h"
#include "ModuleInput.h"

// GameObject & Components
#include "GameObject.h"
#include "TransformComponent.h"


ModuleScript::ModuleScript() {}
ModuleScript::~ModuleScript() {}

bool ModuleScript::Start()
{
	GameObject* tankGo = GameObject::FindWithName("Tank");
	if (tankGo != nullptr)
	{
		tankGoTransform = (TransformComponent*)tankGo->GetComponent(ComponentType::TRANSFORM);
	}
	
	GameObject* turretGo = GameObject::FindWithName("TankTurret");
	if (turretGo != nullptr)
	{
		turretGoTransform = (TransformComponent*)turretGo->GetComponent(ComponentType::TRANSFORM);
	}
	

	return true;
}

bool ModuleScript::PreUpdate(float dt)
{
	return true;
}

bool ModuleScript::Update(float dt)
{
	Move();
	return true;
}

bool ModuleScript::PostUpdate()
{
	return true;
}

bool ModuleScript::CleanUp()
{
	return true;
}

void ModuleScript::Move()
{
	if ((app->scene->GetGameState() == GameState::PLAYING) && (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_DOWN))
	{
		velocity += acceleration * app->scene->gameTimer.GetDeltaTime();
	}
	if ((app->scene->GetGameState() == GameState::PLAYING) && (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_DOWN))
	{
		velocity -= acceleration * app->scene->gameTimer.GetDeltaTime();
	}
}