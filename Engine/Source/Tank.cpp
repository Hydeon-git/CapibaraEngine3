#include "Application.h"
#include "Globals.h"

// Modules
#include "Tank.h"
#include "ModuleInput.h"
#include "ModuleScene.h"

// GameObject & Components
#include "GameObject.h"
#include "TransformComponent.h"
#include "Resource.h"
#include "ResourceManager.h"

Tank::Tank() {}
Tank::~Tank() {}

bool Tank::Start()
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
	
	tankGo = GameObject::FindWithName("Tank");
	if (tankGo != nullptr)
	{
		tankGoTransform = (TransformComponent*)tankGo->GetComponent(ComponentType::TRANSFORM);
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

bool Tank::Update(float dt)
{
	Move();
	Rotate();
	Shoot();
	return true;
}

void Tank::Move()
{
	if (app->scene->GetGameState() == GameState::PLAYING)
	{
		if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
			velocity += acceleration * app->scene->gameTimer.GetDeltaTime();

		if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
			velocity -= acceleration * app->scene->gameTimer.GetDeltaTime();

		velocity = (velocity > maxVelocityForward) ? maxVelocityForward : ((velocity < maxVelocityBackward) ? maxVelocityBackward : velocity);
			
		chassisGoTransform->SetPosition(chassisGoTransform->GetPosition() + chassisGoTransform->forward * velocity * app->scene->gameTimer.GetDeltaTime());
		turretGoTransform->SetPosition(chassisGoTransform->GetPosition() + turretGoTransform->forward * velocity * app->scene->gameTimer.GetDeltaTime());
		rightWheelsGoTransform->SetPosition(chassisGoTransform->GetPosition() + rightWheelsGoTransform->forward * velocity * app->scene->gameTimer.GetDeltaTime());
		leftWheelsGoTransform->SetPosition(chassisGoTransform->GetPosition() + leftWheelsGoTransform->forward * velocity * app->scene->gameTimer.GetDeltaTime());

		if (velocity > 0)
		{
			velocity -= friction;
			if (velocity < 0)
				velocity = 0;
		}

		else if (velocity < 0)
		{
			velocity += friction;
			if (velocity > 0)
				velocity = 0;
		}
	}
}

void Tank::Rotate()
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
		float2 pos = { (tankGoTransform->GetPosition().x * 6.5f), (tankGoTransform->GetPosition().z * 6.5f)};

		float2 dir = mouse - pos;
		float rotation = atan2(dir.y, dir.x);		

		turretGoTransform->SetRotation(Quat::FromEulerXYZ(0, -(rotation -(pi/2)), 0));
	}
}

void Tank::Shoot()
{
	if (app->scene->GetGameState() == GameState::PLAYING)
	{
		if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN && !bulletAlive)
		{
			object = app->scene->CreateGameObject(tankGo);
			bulletGoTransform = (TransformComponent*)object->GetComponent(ComponentType::TRANSFORM);
			std::string path;
			if (object != nullptr)
			{
				object->SetName("Bullet");
				path = "Settings/EngineResources/__Sphere.mesh";
				if (!path.empty())
				{
					MeshComponent* mesh = (MeshComponent*)object->CreateComponent(ComponentType::MESH_RENDERER);
					mesh->SetMesh(ResourceManager::GetInstance()->LoadResource(path));
				}
				bulletDir = turretGoTransform->forward;
				bulletGoTransform->SetPosition({ turretGoTransform->GetPosition().x, turretGoTransform->GetPosition().y + 2.f, turretGoTransform->GetPosition().z });
				bulletGoTransform->SetScale({0.5,0.5,0.5});
				shoot = true;
				bulletAlive = true;
			}
		}

		if(shoot)
			bulletGoTransform->SetPosition(bulletGoTransform->GetPosition() + bulletDir.Mul(bulletVelocity * app->scene->gameTimer.GetDeltaTime()));

		if(bulletAlive)
		{
			if (bulletGoTransform->GetPosition().x < 0 || bulletGoTransform->GetPosition().x > 80 || bulletGoTransform->GetPosition().z < 15 || bulletGoTransform->GetPosition().z > 65)
			{
				tankGo->RemoveChild(object);
				object = nullptr;
				bulletAlive = false;
				shoot = false;
			}
		}
	}
}