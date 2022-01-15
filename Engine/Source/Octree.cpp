#include "Octree.h"
#include "MeshComponent.h"
#include "ModuleScripting.h"
#include "Application.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "glew/include/GL/glew.h"

#include "ModuleRenderer3D.h"

OctreeNode::OctreeNode(const float3& min, const float3& max)
{
	section.minPoint = min;
	section.maxPoint = max;
}

OctreeNode::~OctreeNode()
{
	if (!children.empty()) {
		std::vector<OctreeNode*>::iterator item = children.begin();
		for (; item != children.end(); ++item) {
			if (*item != nullptr) {
				delete* item;
				*item = nullptr;
			}
		}
		children.clear();
	}
	if (!gameObjects.empty())
		gameObjects.clear();
}

void OctreeNode::Insert(GameObject* object, const AABB& sect)
{
	if (section.Contains(sect)) {
		if (children.empty()) {
			if (gameObjects.size() < app->scripting->octree.GetBucket()) {
				AddGameObject(object);
			}
			else {
				Subdivide();
				if (!AddToChildren(object, sect)) { // after the subdivision test if gameobject fits in children, if not delete childrena and object belogns to parent
					Regrup();
					AddGameObject(object);
				}
			}
		}
		else if (!children.empty()) {
			if (!AddToChildren(object, sect)) {
				AddGameObject(object);
			}
		}
	}
	else {
		app->scripting->octree.Recalculate(object);
	}
}

void OctreeNode::Regrup()
{
	std::vector<OctreeNode*>::iterator item = children.begin();
	for (; item != children.end(); ++item) {
		if (*item != nullptr) {
			delete* item;
			*item = nullptr;
		}
	}
	children.clear();
}

bool OctreeNode::Remove(GameObject* object)
{
	bool ret = false;
	if (!gameObjects.empty()) {
		std::vector<GameObject*>::iterator item = gameObjects.begin();
		for (; item != gameObjects.end(); ++item) {
			if (*item != nullptr && *item == object) {
				gameObjects.erase(item);
				ret = true;
				return ret;
			}
		}
	}

	if (!children.empty()) {
		std::vector<OctreeNode*>::iterator item = children.begin();
		for (; item != children.end(); ++item) {
			if (*item != nullptr) {
				ret = (*item)->Remove(object);
				if (ret)
					break;
			}
		}
	}
	return ret;
}

void OctreeNode::DrawNode()
{
	glColor3f(app->scripting->octree_line_color.r, app->scripting->octree_line_color.g, app->scripting->octree_line_color.b);
	glLineWidth(app->scripting->octree_line_width);
	glBegin(GL_LINES);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.minPoint.y, section.minPoint.z);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.minPoint.x, section.minPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.minPoint.x, section.maxPoint.y, section.minPoint.z);

	glVertex3f(section.maxPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.minPoint.z);

	glVertex3f(section.maxPoint.x, section.minPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.minPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.maxPoint.y, section.minPoint.z);
	glVertex3f(section.minPoint.x, section.maxPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.maxPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.minPoint.z);

	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.minPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.maxPoint.z);

	glVertex3f(section.maxPoint.x, section.minPoint.y, section.maxPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.maxPoint.z);
	glVertex3f(section.maxPoint.x, section.minPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.minPoint.y, section.maxPoint.z);
	glVertex3f(section.minPoint.x, section.maxPoint.y, section.maxPoint.z);

	glVertex3f(section.minPoint.x, section.maxPoint.y, section.maxPoint.z);
	glVertex3f(section.maxPoint.x, section.maxPoint.y, section.maxPoint.z);

	glEnd();

	glLineWidth(1);

	if (!children.empty()) {
		std::vector<OctreeNode*>::iterator item = children.begin();
		for (; item != children.end(); ++item) {
			if (*item != nullptr) {
				(*item)->DrawNode();
			}
		}
	}
}

void OctreeNode::AddGameObject(GameObject* obj)
{
	gameObjects.push_back(obj);
}

void OctreeNode::AddNode(const float3& min, const float3& max)
{
	OctreeNode* node = new OctreeNode(min, max);
	node->parent = this;
	children.push_back(node);
}

void OctreeNode::SaveGameObjects(std::vector<GameObject*>* to_save, AABB* new_section)
{
	if (!gameObjects.empty()) {
		std::vector<GameObject*>::iterator item = gameObjects.begin();
		for (; item != gameObjects.end(); ++item) {
			if (*item != nullptr) {
				MeshComponent* mesh = (MeshComponent*)(*item)->GetComponent(ComponentType::MESH_RENDERER);
				AABB aabb = mesh->GetLocalAABB();
				new_section->minPoint = { min(new_section->minPoint.x, aabb.minPoint.x), min(new_section->minPoint.y, aabb.minPoint.y),min(new_section->minPoint.z, aabb.minPoint.z) };
				new_section->maxPoint = { max(new_section->maxPoint.x, aabb.maxPoint.x), max(new_section->maxPoint.y, aabb.maxPoint.y),max(new_section->maxPoint.z, aabb.maxPoint.z) };
				to_save->push_back((*item));
			}
		}
	}

	if (!children.empty()) {
		std::vector<OctreeNode*>::iterator item = children.begin();
		for (; item != children.end(); ++item) {
			if (*item != nullptr) {
				(*item)->SaveGameObjects(to_save, new_section);
			}
		}
	}
}

bool OctreeNode::AddToChildren(GameObject * obj, const AABB& sect)
{
	bool ret = false;
	std::vector<OctreeNode*>::iterator item = children.begin();
	uint intersections = 0;
	for (; item != children.end(); ++item) {
		if (*item != nullptr && (*item)->section.Contains(sect)) {
			(*item)->Insert(obj, sect);
			ret = true;
			break;
		}
	}
	return ret;
}

void OctreeNode::SetStaticDrawList(std::vector<std::pair<float, GameObject*>>* to_draw, const CameraComponent* camera)
{
	if (app->renderer3D->IsInsideFrustum(camera, section)) {
		if (!gameObjects.empty()) {
			std::vector<GameObject*>::iterator item = gameObjects.begin();
			for (; item != gameObjects.end(); ++item) {
				if (*item != nullptr && (*item)->IsParentEnabled()) {
					MeshComponent* mesh = (MeshComponent*)(*item)->GetComponent(ComponentType::MESH_RENDERER);
					if (mesh != nullptr && mesh->mesh != nullptr) {
						if (app->renderer3D->IsInsideFrustum(camera, mesh->GetLocalAABB())) {
							float3 obj_pos = static_cast<TransformComponent*>((*item)->GetComponent(ComponentType::TRANSFORM))->GetGlobalPosition();
							float distance = camera->camera.pos.Distance(obj_pos);
							to_draw->push_back({ distance, *item });
						}
					}
				}
			}
		}
		if (!children.empty()) {
			std::vector<OctreeNode*>::iterator item = children.begin();
			for (; item != children.end(); ++item) {
				if (*item != nullptr) {
					if (app->renderer3D->IsInsideFrustum(camera, (*item)->section)) {
						(*item)->SetStaticDrawList(to_draw, camera);
					}
				}
			}
		}
	}
}

void OctreeNode::Subdivide()
{
	float3 mid_point = section.minPoint + (section.maxPoint - section.minPoint) * 0.5F;

	AddNode(section.minPoint, mid_point); // 0
	AddNode(mid_point, section.maxPoint); // 2

	float3 point = { section.minPoint.x,section.maxPoint.y,section.minPoint.z };

	AddNode(float3{min(point.x,mid_point.x), min(point.y,mid_point.y), min(point.z,mid_point.z) },
		float3{ max(point.x,mid_point.x), max(point.y,mid_point.y), max(point.z,mid_point.z) });

	point = { section.maxPoint.x,section.minPoint.y,section.maxPoint.z };

	AddNode(float3{ min(point.x,mid_point.x), min(point.y,mid_point.y), min(point.z,mid_point.z) },
		float3{ max(point.x,mid_point.x), max(point.y,mid_point.y), max(point.z,mid_point.z) });

	point = { section.maxPoint.x,section.maxPoint.y,section.minPoint.z };

	AddNode(float3{ min(point.x,mid_point.x), min(point.y,mid_point.y), min(point.z,mid_point.z) },
		float3{ max(point.x,mid_point.x), max(point.y,mid_point.y), max(point.z,mid_point.z) });

	point = { section.maxPoint.x,section.minPoint.y,section.minPoint.z };

	AddNode(float3{ min(point.x,mid_point.x), min(point.y,mid_point.y), min(point.z,mid_point.z) },
		float3{ max(point.x,mid_point.x), max(point.y,mid_point.y), max(point.z,mid_point.z) });

	point = { section.minPoint.x,section.maxPoint.y,section.maxPoint.z };

	AddNode(float3{ min(point.x,mid_point.x), min(point.y,mid_point.y), min(point.z,mid_point.z) },
		float3{ max(point.x,mid_point.x), max(point.y,mid_point.y), max(point.z,mid_point.z) });

	point = { section.minPoint.x,section.minPoint.y,section.maxPoint.z };
	
	AddNode(float3{ min(point.x,mid_point.x), min(point.y,mid_point.y), min(point.z,mid_point.z) },
		float3{ max(point.x,mid_point.x), max(point.y,mid_point.y), max(point.z,mid_point.z) });

	// reorder the gameobjects to the new nodes if possible, if intersects parent keep them
	std::vector<GameObject*>::iterator objs = gameObjects.begin();
	while (objs != gameObjects.end()) {
		bool to_delete = false;
		if (*objs != nullptr) {
			std::vector<OctreeNode*>::iterator item = children.begin();
			uint intersections = 0;
			for (; item != children.end(); ++item) {
				ComponentMesh* mesh = (ComponentMesh*)(*objs)->GetComponent(ComponentType::MESH);
				if (*item != nullptr && (*item)->section.Contains(mesh->GetGlobalAABB().minPoint) && (*item)->section.Contains(mesh->GetGlobalAABB().maxPoint)) {
					(*item)->Insert(*objs, mesh->GetGlobalAABB());
					to_delete = true;
					break;
				}
			}
		}
		if (to_delete) {
			objs = gameObjects.erase(objs);
		}
		else {
			++objs;
		}
	}
}

Octree::Octree()
{
}

Octree::~Octree()
{
	if (root != nullptr)
		delete root;
}

void Octree::Insert(GameObject* object, bool add_children)
{
	ComponentMesh* mesh_parent = (ComponentMesh*)object->GetComponent(ComponentType::MESH);

	if (mesh_parent != nullptr && mesh_parent->mesh != nullptr) {
		if (root == nullptr) {
			Init(mesh_parent->GetGlobalAABB().minPoint, mesh_parent->GetGlobalAABB().maxPoint);
		}
		if (!Exists(object)) {
			allObjects.push_back(object);
			root->Insert(object, mesh_parent->GetGlobalAABB());
		}
	}

	if (add_children && !object->children.empty()) {
		std::vector<GameObject*>::iterator item = object->children.begin();
		for (; item != object->children.end(); ++item) {
			if (*item != nullptr) {
				Insert((*item), add_children);
			}
		}
	}
}

void Octree::Remove(GameObject* object)
{
	if (root == nullptr)
		return;

	RemoveRecursively(object);

	if (allObjects.empty()) {
		Clear();
	}
	else {
		App->objects->octree.Recalculate(nullptr);
	}
}

void Octree::Clear()
{
	if (root != nullptr) {
		delete root;
		root = nullptr;
	}
}

void Octree::Draw()
{
	if (root != nullptr)
		root->DrawNode();
}

const uint& Octree::GetBucket() const
{
	return bucket;
}

void Octree::SetBucket(const uint& bucket)
{
	this->bucket = bucket;
}

void Octree::Init(const float3& min, const float3& max)
{
	if (root != nullptr) {
		delete root;
		allObjects.clear();
	}
	root = new OctreeNode(min, max);
}

void Octree::Recalculate(GameObject* new_object)
{
	if (root == nullptr)
		return;

	std::vector<GameObject*> to_save;

	AABB new_section;
	new_section.SetNegativeInfinity();

	if (new_object != nullptr) {
		ComponentMesh* mesh = (ComponentMesh*)new_object->GetComponent(ComponentType::MESH);
		new_section = mesh->GetGlobalAABB();
	}

	// get all gameobjects in octree and get the min & max points
	root->SaveGameObjects(&to_save, &new_section);

	if (new_object != nullptr) {
		to_save.push_back(new_object);
	}

	// delete the old octree and create it again
	Init(new_section.minPoint, new_section.maxPoint);

	// insert all the game objects
	std::vector<GameObject*>::iterator item = to_save.begin();
	for (; item != to_save.end(); ++item) {
		if (*item != nullptr) {
			Insert((*item), false);
		}
	}
	to_save.clear();
}

void Octree::SetStaticDrawList(std::vector<std::pair<float, GameObject*>>* to_draw, const ComponentCamera* camera)
{
	if (root == nullptr) {
		return;
	}

	root->SetStaticDrawList(to_draw, camera);
}

bool Octree::Exists(GameObject* object)
{
	bool ret = false;
	if (!allObjects.empty()) {
		ret = std::find(allObjects.begin(), allObjects.end(), object) == allObjects.begin();
	}
	return ret;
}

void Octree::RemoveRecursively(GameObject* obj)
{
	ComponentMesh* mesh_parent = (ComponentMesh*)obj->GetComponent(ComponentType::MESH);
	if (mesh_parent != nullptr && mesh_parent->mesh != nullptr) {
		if (root->Remove(obj))
			allObjects.remove(obj);
	}

	if (!obj->children.empty()) {
		std::vector<GameObject*>::iterator item = obj->children.begin();
		for (; item != obj->children.end(); ++item) {
			if (*item != nullptr) {
				RemoveRecursively((*item));
			}
		}
	}
}
