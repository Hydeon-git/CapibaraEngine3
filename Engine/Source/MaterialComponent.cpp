#include "MaterialComponent.h"
#include "Globals.h"
#include "Imgui/imgui.h"

#include "mmgr/mmgr.h"

MaterialComponent::MaterialComponent(int i, int w, int h, std::string p) : id(i), width(w), height(h), path(p)
{
	texBuffer = new TextureBuffer(i, w, h);

	texBuffer->Unbind();
}

MaterialComponent::~MaterialComponent()
{
	RELEASE(texBuffer);
}

void MaterialComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::Text("Path: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", path.c_str());
		ImGui::Separator();
	}
}

void MaterialComponent::SetNewMaterial(int i, int w, int h, std::string p)
{
	RELEASE(texBuffer);

	texBuffer = new TextureBuffer(i, w, h);

	texBuffer->Unbind();
}

void MaterialComponent::BindTexture()
{
	if (texBuffer) texBuffer->Bind();
}

void MaterialComponent::UnbindTexture()
{
	if (texBuffer) texBuffer->Unbind();
}