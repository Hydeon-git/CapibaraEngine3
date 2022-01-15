#include "Application.h"
#include "Globals.h"

// Modules
#include "ModuleScript.h"

#include "GameObject.h"

ModuleScript::ModuleScript() {}

ModuleScript::~ModuleScript() {}

bool ModuleScript::Start()
{
	//GameObject* wheels = GameObject::FindWithName("Lower_Tank");

	return true;
}

bool ModuleScript::PreUpdate(float dt)
{
	return true;
}

bool ModuleScript::Update(float dt)
{
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

GameObject* ModuleScript::GetRoot(bool ignore_prefab)
{	
	if (baseGO->GetChilds().size() == 0)
	{
		return baseGO;
	}			
	else
	{
		return baseGO->GetChilds().back();
	}			
}
