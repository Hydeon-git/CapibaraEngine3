#include "Capibara.h"
#include "Application.h"
#include "GameObject.h"

Capi::Capi()
{
}

Capi::~Capi()
{
	//app->objects->CancelInvokes(this);
}

bool Capi::IsScriptEnabled() const
{
	return (enabled == nullptr) ? false : *enabled;
}

void Capi::SetScriptEnable(const bool& enable)
{
	if (enable != *enabled) {
		*enabled = enable;
		if (enable) {
			OnEnable();
		}
		else {
			OnDisable();
		}
	}
}

const char* Capi::ToString()
{
	return go->ToString();
}

bool Capi::HasComponent(const ComponentType& component) const
{
	return go->HasComponent(component);
}

Component* Capi::GetComponent(const ComponentType& type)
{
	return go->GetComponent(type);
}

const Component* Capi::GetComponent(const ComponentType& type) const
{
	return go->GetComponent(type);
}

void* Capi::GetComponentScript(const char* script_class_name)
{
	return go->GetComponentScript(script_class_name);
}

const void* Capi::GetComponentScript(const char* script_class_name) const
{
	return go->GetComponentScript(script_class_name);
}

Component* Capi::GetComponentInParent(const ComponentType& type)
{
	return go->GetComponentInParent(type);
}

const Component* Capi::GetComponentInParent(const ComponentType& type) const
{
	return go->GetComponentInParent(type);
}

void* Capi::GetComponentScriptInParent(const char* script_class_name)
{
	return go->GetComponentScriptInParent(script_class_name);
}

const void* Capi::GetComponentScriptInParent(const char* script_class_name) const
{
	return go->GetComponentScriptInParent(script_class_name);
}

Component* Capi::GetComponentInChildren(const ComponentType& type, bool recursive)
{
	return go->GetComponentInChildren(type, recursive);
}

const Component* Capi::GetComponentInChildren(const ComponentType& type, bool recursive) const
{
	return go->GetComponentInChildren(type, recursive);
}

uint Capi::GetComponents(const ComponentType& type, Component*** comp_array)
{
	return go->GetComponents(type, comp_array);
}

const uint Capi::GetComponents(const ComponentType& type, Component*** comp_array) const
{
	return go->GetComponents(type, comp_array);
}

uint Capi::GetComponentsInChildren(const ComponentType& type, Component*** comp_array, bool recursive)
{
	return go->GetComponentsInChildren(type, comp_array, recursive);
}

const uint Capi::GetComponentsInChildren(const ComponentType& type, Component*** comp_array, bool recursive) const
{
	return go->GetComponentsInChildren(type, comp_array, recursive);
}

uint Capi::GetComponentsInParent(const ComponentType& type, Component*** comp_array)
{
	return go->GetComponentsInParent(type, comp_array);
}

const uint Capi::GetComponentsInParent(const ComponentType& type, Component*** comp_array) const
{
	return go->GetComponentsInParent(type, comp_array);
}

uint Capi::GetComponentsScript(const char* script_class_name, void*** script_array)
{
	return go->GetComponentsScript(script_class_name, script_array);
}

const uint Capi::GetComponentsScript(const char* script_class_name, void*** script_array) const
{
	return go->GetComponentsScript(script_class_name, script_array);
}

uint Capi::GetComponentsScriptInChildren(const char* script_class_name, void*** script_array, bool recursive)
{
	return go->GetComponentsScriptInChildren(script_class_name, script_array, recursive);
}

const uint Capi::GetComponentsScriptInChildren(const char* script_class_name, void*** script_array, bool recursive) const
{
	return go->GetComponentsScriptInChildren(script_class_name, script_array, recursive);
}

uint Capi::GetComponentsScriptInParent(const char* script_class_name, void*** script_array)
{
	return go->GetComponentsScriptInParent(script_class_name, script_array);
}

const uint Capi::GetComponentsScriptInParent(const char* script_class_name, void*** script_array) const
{
	return go->GetComponentsScriptInParent(script_class_name, script_array);
}

void Capi::Destroy(GameObject* obj)
{
	GameObject::Destroy(obj);
}

void Capi::DestroyInstantly(GameObject* obj)
{
	GameObject::DestroyInstantly(obj);
}

bool Capi::CompareTag(const char* tag_to_compare) const
{
	return strcmp(go->GetTag(), tag_to_compare) == 0;
}

void Capi::Invoke(std::function<void()> void_no_params_function, float seconds)
{
	if (void_no_params_function != nullptr) {
		app->objects->AddInvoke(void_no_params_function, seconds, this);
	}
}

void Capi::CancelInvoke()
{
	app->objects->CancelInvokes(this);
}

void Capi::InvokeRepeating(std::function<void()> void_no_params_function, float seconds_to_first_invoke, float seconds_between_each_call)
{
	if (void_no_params_function != nullptr) {
		app->objects->AddInvokeRepeating(void_no_params_function, seconds_to_first_invoke, seconds_between_each_call, this);
	}
}