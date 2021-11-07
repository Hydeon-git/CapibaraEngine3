#include "CameraComponent.h"
#include "GameObject.h"

CameraComponent::CameraComponent(GameObject* own) : horizontalFov(90.0f), verticalFov(60.0f), nearPlane(1.0f), farPlane(5.0f)
{
	type = ComponentType::CAMERA;
	owner = own;
	camera.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	camera.SetViewPlaneDistances(nearPlane, farPlane);
	camera.SetPerspective(horizontalFov, verticalFov);
	camera.SetFrame(float3(0.0f, 1.5f, 5.0f), float3(0.0f, 0.0f, -1.0f), float3(0.0f, 1.0f, 0.0f));
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Text("Field of view");
		ImGui::SameLine();
		if (ImGui::DragFloat("", &horizontalFov, 0.5f, 0.1f, 179.0f)) SetFov();

		ImGui::Text("Clipping planes");

		ImGui::Text("Near");
		ImGui::SameLine();
		ImGui::PushID("NearPlane");
		if (ImGui::DragFloat("", &nearPlane, 0.5f)) SetPlanes();
		ImGui::PopID();

		ImGui::PushID("farPlane");
		ImGui::Text("Far");
		ImGui::SameLine();
		if (ImGui::DragFloat("", &farPlane, 0.5f, 0.1f)) SetPlanes();
		ImGui::PopItemWidth();
		ImGui::PopID();

	}
	ImGui::PopID();
}

void CameraComponent::SetPlanes()
{
	camera.SetViewPlaneDistances(nearPlane, farPlane);
}

void CameraComponent::SetFov()
{
	verticalFov = 2 * Atan((Tan(horizontalFov / 2)) * camera.AspectRatio());
	camera.SetHorizontalFovAndAspectRatio(horizontalFov, camera.AspectRatio());
}

bool CameraComponent::OnLoad(JsonParsing& node)
{
	return false;
}

bool CameraComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	return false;
}