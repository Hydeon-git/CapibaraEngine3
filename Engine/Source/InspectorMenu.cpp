#include "InspectorMenu.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"

#include "GameObject.h"
#include "Resource.h"

#include "Profiling.h"

InspectorMenu::InspectorMenu() : Menu(true)
{
}

InspectorMenu::~InspectorMenu()
{
}

bool InspectorMenu::Update(float dt)
{
	ImGui::Begin("Inspector", &active);
	if (!app->scene->GetRoot()->GetChilds().empty())
	{		
		if (app->editor->GetGO())
		{
			// This line below draws in the inspector only the selected object, Unity style
			if (ImGui::BeginChild("Selected GO"))
			{
				app->editor->GetGO()->DrawEditor();
				ImGui::EndChild();
			}
		}
		else if (app->editor->GetResource())
		{
			app->editor->GetResource()->DrawOnEditor();
		}
	}
	ImGui::End();

	return true;
}