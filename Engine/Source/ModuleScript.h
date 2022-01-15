#pragma once

#include "Module.h"

class GameObject;


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

	GameObject* GetRoot(bool ignore_prefab);

private:
	GameObject* baseGO = nullptr;
};