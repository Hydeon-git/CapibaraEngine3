#include "Move.h"
#include "Tank.h"

Move::Move() : Capi()
{

}

Move::~Move()
{

}

void Move::Start()
{
	bullet.ConvertToGameObject(transform->GetGlobalPosition());


	time2 = Time::GetGameTime();

	if (test_object != nullptr) {
		LOG(test_object->GetName());
	}
}

void Move::Update()
{
	//if (Input::GetFirstKeyDown() == SDL_SCANCODE_1)
	//	test = 12;
	//if (Input::GetKey(SDL_SCANCODE_2) == Input::KEY_DOWN)
	//	test = 13;
	//if (Input::GetKeyRepeat(SDL_SCANCODE_3))
	//	test = 14;
	//if (Input::GetMouseButton(Input::MOUSE_LEFT_BUTTON) == Input::KEY_DOWN)
	//	test = 15;

	//Debug::Log("%i", Random::GetRandomIntBetweenTwo(1, 5));
	//Debug::Log("%f", Random::GetRandomFloatBetweenTwo(1, 5));

	//if (Time::GetGameTime() != Time::GetTimeSinceStart()) {
	//	Time::GetDT();
	//}

	//Debug::Log("Time: %f", Time::GetTimeSinceStart());
	//Camera::GetAllCameras();
	//Camera::GetCurrentCamera();
	//Camera::GetNumCameras();

	//ComponentLight* light = new ComponentLight(NULL);
	//light->ambient = Color::Yellow();

	//ComponentMesh* mesh = new ComponentMesh(nullptr);
	//mesh->IsEnabled();

	//ComponentScript* script = new ComponentScript(nullptr);
	//script->SetEnable(false);

	//GameObject* pl = GameObject::FindWithTag("Player");
	//go->SetName("NewNAME");
	//Debug::Log(go->GetTag());

	//GameObject* en = GameObject::FindWithTag("Enemy");
	//if (en->IsEnabled()) {
	//	int i = 0;
	//}
	////GameObject* namee = GameObject::FindWithName("Light");


	GameObject** untags = nullptr;
	GameObject** players = nullptr;
	GameObject** enemies = nullptr;
	uint s_untags = GameObject::FindGameObjectsWithTag("UnTagged", &untags);
	uint s_players = GameObject::FindGameObjectsWithTag("Player", &players);
	uint s_enemies = GameObject::FindGameObjectsWithTag("Enemy", &enemies);
	uint count = 0;
	for (uint i = 0; i < s_untags; ++i) {
		if (untags[i] != nullptr && untags[i]->IsEnabled()) {
			count++;
			LOG(untags[i]->GetName());
		}
	}
	LOG("%i", count);
	GameObject::FreeArrayMemory((void***)&untags);

	//go->ToDelete();
	//Tank* tank = nullptr;
	//Tank* tank2 = nullptr;
	//tank = (Tank*)go->GetComponentScript(VARAIBLE_TO_STRING(Tank));
	//tank2 = (Tank*)go->GetComponentScript("Tank");
	//
	//if (time < Time::GetGameTime() - 2) {
	//	go->SetNewParent(GameObject::FindWithName("Light"));
	//	SetScriptEnable(false);
	//}

	//GameObject* obj = nullptr;
	//obj->GetChild(0);
}

void Move::OnDrawGizmos()
{
	LOG("GIZMO :D");
	Gizmos::DrawCube(transform->GetGlobalPosition(), { 4,2,3 }, Color::Yellow());
}
