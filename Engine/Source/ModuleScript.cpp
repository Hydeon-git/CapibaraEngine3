#include "Application.h"
#include "Globals.h"
#include "Time.h"

// Modules
#include "ModuleScript.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

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
	
	GameObject* tankChassisGo = GameObject::FindWithName("TankChassis");
	if (tankChassisGo != nullptr)
	{
		tankChassisGoTransform = (TransformComponent*)tankChassisGo->GetComponent(ComponentType::TRANSFORM);
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
	Rotate();
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
	if (app->scene->GetGameState() == GameState::PLAYING)
	{
		if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
		{
			velocity += acceleration;
			
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
		{
			velocity -= acceleration;
		}

		velocity = (velocity > maxVelocityForward) ? maxVelocityForward : ((velocity < maxVelocityBackward) ? maxVelocityBackward : velocity);

		//float3 pos = { tankGoTransform->GetPosition().x,tankGoTransform->GetPosition().y, velocity };
		tankChassisGoTransform->SetPosition(turretGoTransform->GetPosition() + tankChassisGoTransform->forward * velocity);
	}

}

void ModuleScript::Rotate()
{
	if (app->scene->GetGameState() == GameState::PLAYING)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
		{
			angle += rotateVelocity;
			tankChassisGoTransform->SetRotation(Quat::FromEulerXYZ(0, DegToRad(angle), 0));
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
		{
			angle -= rotateVelocity;
			tankChassisGoTransform->SetRotation(Quat::FromEulerXYZ(0, DegToRad(angle), 0));
		}

		float2 mouse = { app->input->GetMousePosition().x, app->input->GetMousePosition().y };
		float3 rotation = turretGoTransform->GetRotation().ToEulerXYZ();
		int width = app->window->width;
		int middle = width / 2;
		int height = app->window->height;

		float3 wheels = RadToDeg(tankChassisGoTransform->GetRotation().ToEulerXYZ());

		if (wheels.x == 0 && wheels.y > 0)
		{
			rotation.y = (-(mouse.x * 180 / width) + 90) + (wheels.y);

			if (mouse.y > (height / 2))
			{
				rotation.x = -180;
				rotation.z = -180;
				rotation.y -= wheels.y * 2;
			}
			else
			{
				rotation.x = 0;
				rotation.z = 0;
			}
		}

		else if (wheels.x == 0 && wheels.y < 0)
		{
			rotation.y = ((mouse.x * 180 / width)) - (wheels.y);

			if (mouse.y > (height / 2))
			{
				rotation.y -= (-90 - wheels.y * 2);
				rotation.x = 0;
				rotation.z = 0;
			}
			else
			{
				rotation.y -= 270;
				rotation.x = -180;
				rotation.z = -180;
			}
		}

		else if ((wheels.x == -180 || wheels.x == 180) && wheels.y > 0)
		{
			rotation.y = (-(mouse.x * 180 / width)) - (wheels.y);

			if (mouse.y > (height / 2))
			{
				rotation.y -= (90 - wheels.y * 2);
				rotation.x = -180;
				rotation.z = -180;
			}
			else
			{
				rotation.y -= 90;
				rotation.x = 0;
				rotation.z = 0;
			}
		}

		else if ((wheels.x == -180 || wheels.x == 180) && wheels.y < 0)
		{
			rotation.y = (-(mouse.x * 180 / width));

			if (mouse.y > (height / 2))
			{
				rotation.x = -180;
				rotation.z = -180;
				rotation.y += wheels.y - 90;
			}
			else
			{
				rotation.x = 0;
				rotation.z = 0;
				rotation.y -= wheels.y + 90;
			}
		}

		turretGoTransform->SetRotation(Quat::FromEulerXYZ(DegToRad(rotation.x), DegToRad(rotation.y), DegToRad(rotation.z)));
	}

}