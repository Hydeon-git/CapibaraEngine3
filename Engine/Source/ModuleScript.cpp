#include "Application.h"
#include "Globals.h"
#include "Time.h"

// Modules
#include "ModuleScript.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleEditor.h"

// GameObject & Components
#include "GameObject.h"
#include "TransformComponent.h"


ModuleScript::ModuleScript() {}
ModuleScript::~ModuleScript() {}

bool ModuleScript::Start()
{	
	GameObject* chassisGo = GameObject::FindWithName("TankChassis");
	if (chassisGo != nullptr)
	{
		chassisGoTransform = (TransformComponent*)chassisGo->GetComponent(ComponentType::TRANSFORM);
	}
	
	GameObject* turretGo = GameObject::FindWithName("TankTurret");
	if (turretGo != nullptr)
	{
		turretGoTransform = (TransformComponent*)turretGo->GetComponent(ComponentType::TRANSFORM);
	}
	
	GameObject* rightWheelsGo = GameObject::FindWithName("TankTracksRight");
	if (rightWheelsGo != nullptr)
	{
		rightWheelsGoTransform = (TransformComponent*)rightWheelsGo->GetComponent(ComponentType::TRANSFORM);
	}

	GameObject* leftWheelsGo = GameObject::FindWithName("TankTracksLeft");
	if (leftWheelsGo != nullptr)
	{
		leftWheelsGoTransform = (TransformComponent*)leftWheelsGo->GetComponent(ComponentType::TRANSFORM);
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
	Shoot();
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
			velocity += acceleration * app->scene->gameTimer.GetDeltaTime();
			
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
		{
			velocity -= acceleration * app->scene->gameTimer.GetDeltaTime();
		}

		velocity = (velocity > maxVelocityForward) ? maxVelocityForward : ((velocity < maxVelocityBackward) ? maxVelocityBackward : velocity);
			
		chassisGoTransform->SetPosition(chassisGoTransform->GetPosition() + chassisGoTransform->forward * velocity * app->scene->gameTimer.GetDeltaTime());
		turretGoTransform->SetPosition(chassisGoTransform->GetPosition() + turretGoTransform->forward * velocity * app->scene->gameTimer.GetDeltaTime());
		rightWheelsGoTransform->SetPosition(chassisGoTransform->GetPosition() + rightWheelsGoTransform->forward * velocity * app->scene->gameTimer.GetDeltaTime());
		leftWheelsGoTransform->SetPosition(chassisGoTransform->GetPosition() + leftWheelsGoTransform->forward * velocity * app->scene->gameTimer.GetDeltaTime());

		if (velocity > 0)
		{
			velocity -= friction;
			if (velocity < 0)
			{
				velocity = 0;
			}
		}
		else if (velocity < 0)
		{
			velocity += friction;
			if (velocity > 0)
			{
				velocity = 0;
			}
		}
	}

}

void ModuleScript::Rotate()
{
	if (app->scene->GetGameState() == GameState::PLAYING)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
		{
			angle += rotateVelocity * app->scene->gameTimer.GetDeltaTime();
			chassisGoTransform->SetRotation(Quat::FromEulerXYZ(0, DegToRad(angle), 0));
			rightWheelsGoTransform->SetRotation(chassisGoTransform->GetRotation());
			leftWheelsGoTransform->SetRotation(chassisGoTransform->GetRotation());
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
		{
			angle -= rotateVelocity * app->scene->gameTimer.GetDeltaTime();
			chassisGoTransform->SetRotation(Quat::FromEulerXYZ(0, DegToRad(angle), 0));
			rightWheelsGoTransform->SetRotation(chassisGoTransform->GetRotation());
			leftWheelsGoTransform->SetRotation(chassisGoTransform->GetRotation());
		}

		float2 mouse = { app->input->GetMousePosition().x, app->input->GetMousePosition().y };
		float3 rotation = turretGoTransform->GetRotation().ToEulerXYZ();
		int width = app->editor->gameView->sizeViewport.x;
		int middle = width / 2;
		int height = app->editor->gameView->sizeViewport.x;

		float3 wheels = RadToDeg(chassisGoTransform->GetRotation().ToEulerXYZ());

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

void ModuleScript::Shoot() 
{

}