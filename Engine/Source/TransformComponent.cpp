#include "Application.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ModuleScene.h"
#include "Globals.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_internal.h"
#include "Imgui/ImGuizmo.h"

#include <stack>

#include "Profiling.h"

TransformComponent::TransformComponent(GameObject* own)
{
	type = ComponentType::TRANSFORM;
	owner = own;

	position = { 0.0f, 0.0f, 0.0f }; 
	rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	scale = { 1.0f, 1.0f, 1.0f };
	localMatrix = float4x4::FromTRS(position, rotation, scale);
	
	if (owner->GetParent() != nullptr)
	{
		TransformComponent* tr = owner->GetParent()->GetComponent<TransformComponent>();
		if (tr != nullptr)
			globalMatrix = localMatrix * tr->GetGlobalTransform();
	}
	else
	{
		globalMatrix = localMatrix;
	}

	up = { 2 * (rotation.x * rotation.y - rotation.w * rotation.z),
			1 - 2 * (rotation.x * rotation.x + rotation.z * rotation.z),
			2 * (rotation.y * rotation.z + rotation.w * rotation.x) };
	forward = { 2 * (rotation.x * rotation.z + rotation.w * rotation.y),
			2 * (rotation.y * rotation.z - rotation.w * rotation.x),
			1 - 2 * (rotation.x * rotation.x + rotation.y * rotation.y) };
	right = { 1 - 2 * (rotation.y * rotation.y + rotation.z * rotation.z),
		 2 * (rotation.x * rotation.y + rotation.w * rotation.z),
		 2 * (rotation.x * rotation.z + rotation.w * rotation.y) };


	for (int i = 0; i < 3; ++i)
		rotationEditor[i] = 0;

	collapsed = false;
	active = true;
}

TransformComponent::TransformComponent(TransformComponent* trans)
{
	type = ComponentType::TRANSFORM;

	position = trans->GetPosition();
	rotation = trans->GetRotation();
	scale = trans->GetScale();
	globalMatrix = trans->GetGlobalTransform();
	localMatrix = trans->GetLocalTransform();
	rotationEditor = trans->GetRotEditor();
}

TransformComponent::~TransformComponent()
{
}

bool TransformComponent::Update(float dt)
{
	if (changeTransform)
	{
		std::stack<GameObject*> stack;

		UpdateTransform();

		for (int i = 0; i < owner->GetChilds().size(); ++i)
			stack.push(owner->GetChilds()[i]);

		while (!stack.empty())
		{
			GameObject* go = stack.top();

			UpdateChildTransform(go);

			stack.pop();

			for (int i = 0; i < go->GetChilds().size(); ++i)
				stack.push(go->GetChilds()[i]);
		}

		SetAABB();

		changeTransform = false;
	}

	return true;
}

void TransformComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::PushItemWidth(90);		
		
		ShowTransformationInfo();

		ImGui::Separator();
	}
}

void TransformComponent::SetPosition(const float3& pos)
{
	position = pos;

	UpdateTransform();
}

void TransformComponent::SetScale(const float3& sca)
{
	scale = sca;

	UpdateTransform();
}

void TransformComponent::SetRotation(const Quat& rot)
{
	rotation.Set(rot.x, rot.y, rot.z, rot.w);

	rotationEditor = rot.ToEulerXYZ();
	rotationEditor.x = RadToDeg(rotationEditor.x);
	rotationEditor.y = RadToDeg(rotationEditor.y);
	rotationEditor.z = RadToDeg(rotationEditor.z);

	UpdateTransform();
}

void TransformComponent::SetTransform(float3 pos, Quat rot, float3 sca)
{
	position = pos;
	rotation = rot;
	scale = sca;

	globalMatrix = float4x4::FromTRS(position, rotation, scale);	
	DEBUG_LOG("This is %s", owner->GetName());
}

void TransformComponent::SetTransform(float4x4 trMatrix)
{
	globalMatrix = trMatrix;
	globalMatrix.Decompose(position, rotation, scale);
	
	changeTransform = true;
}

const float3 TransformComponent::GetGlobalPosition() const
{
	float3 pos, scale;
	Quat rot;

	globalMatrix.Decompose(pos, rot, scale);

	return pos;
}


bool TransformComponent::OnLoad(JsonParsing& node)
{
	active = node.GetJsonBool("Active");
	position = node.GetJson3Number(node, "Position");
	float4 quat = node.GetJson4Number(node, "Quaternion");
	rotation = Quat(quat.x, quat.y, quat.z, quat.w);
	scale = node.GetJson3Number(node, "Scale");
	rotationEditor = node.GetJson3Number(node, "RotationEditor");

	changeTransform = true;

	return true;
}

bool TransformComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);
	file.SetNewJson3Number(file, "Position", position);
	file.SetNewJson4Number(file, "Quaternion", rotation);
	file.SetNewJson3Number(file, "RotationEditor", rotationEditor);
	file.SetNewJson3Number(file, "Scale", scale);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void TransformComponent::UpdateTransform()
{
	localMatrix = float4x4::FromTRS(position, rotation, scale);

	if (owner->GetParent() && owner->GetParent() != app->scene->GetRoot())
	{
		TransformComponent* parentTr = owner->GetParent()->GetComponent<TransformComponent>();
		if (parentTr) globalMatrix = parentTr->globalMatrix * localMatrix;
	}
	else
	{
		globalMatrix = localMatrix;
	}
	up = { 2 * (rotation.x * rotation.y - rotation.w * rotation.z),
			1 - 2 * (rotation.x * rotation.x + rotation.z * rotation.z),
			2 * (rotation.y * rotation.z + rotation.w * rotation.x) };
	forward = { 2 * (rotation.x * rotation.z + rotation.w * rotation.y),
				2 * (rotation.y * rotation.z - rotation.w * rotation.x),
				1 - 2 * (rotation.x * rotation.x + rotation.y * rotation.y) };
	right = { 1 - 2 * (rotation.y * rotation.y + rotation.z * rotation.z),
			 2 * (rotation.x * rotation.y + rotation.w * rotation.z),
			 2 * (rotation.x * rotation.z + rotation.w * rotation.y) };

}

void TransformComponent::UpdateChildTransform(GameObject* go)
{
	TransformComponent* transform = go->GetComponent<TransformComponent>();
	GameObject* parent = go->GetParent();
	TransformComponent* parentTrans = parent->GetComponent<TransformComponent>();
	if (transform)
	{
		transform->globalMatrix = parentTrans->GetGlobalTransform() * transform->localMatrix;
	}
}

void TransformComponent::SetAABB()
{
	std::vector<GameObject*> goList = owner->GetChilds();
	owner->ClearAABB();
	OBB childOBB;
	for (int i = 0; i < goList.size(); ++i)
	{
		TransformComponent* tr = goList[i]->GetComponent<TransformComponent>();
		tr->SetAABB();
		childOBB = tr->owner->GetAABB();
		owner->SetAABB(childOBB);
	}
	if (owner->GetComponent<MeshComponent>())
	{
		OBB newObb = owner->GetComponent<MeshComponent>()->GetLocalAABB().ToOBB();
		newObb.Transform(globalMatrix);
		owner->SetAABB(newObb);
	}
	app->scene->RecalculateAABB(owner);
	app->scene->ResetQuadtree();
}

bool TransformComponent::DrawVec3(std::string& name, float3& vec)
{
	float3 lastVec = vec;
	ImGui::PushID(name.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 100.0f);
	ImGui::Text(name.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,0 });

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	ImGui::Button("X");
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &vec.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
	ImGui::Button("Y");
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &vec.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
	ImGui::Button("Z");
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &vec.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();

	if (lastVec.x != vec.x || lastVec.y != vec.y || lastVec.z != vec.z) return true;
	else return false;
}

void TransformComponent::ShowTransformationInfo()
{
	if (DrawVec3(std::string("Position: "), position)) changeTransform = true;

	float3 rotationInEuler;
	rotationInEuler.x = RADTODEG * rotationEditor.x;
	rotationInEuler.y = RADTODEG * rotationEditor.y;
	rotationInEuler.z = RADTODEG * rotationEditor.z;
	if (DrawVec3(std::string("Rotation: "), rotationInEuler))
	{
		rotationInEuler.x = DEGTORAD * rotationInEuler.x;
		rotationInEuler.y = DEGTORAD * rotationInEuler.y;
		rotationInEuler.z = DEGTORAD * rotationInEuler.z;

		Quat rotationDelta = Quat::FromEulerXYZ(rotationInEuler.x - rotationEditor.x, rotationInEuler.y - rotationEditor.y, rotationInEuler.z - rotationEditor.z);
		rotation = rotation * rotationDelta;
		rotationEditor = rotationInEuler;
		
		changeTransform = true;
	}

	if (DrawVec3(std::string("Scale: "), scale)) changeTransform = true;
}