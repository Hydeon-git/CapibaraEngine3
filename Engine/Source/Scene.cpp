#include "Scene.h"
#include "Application.h"
#include "FileSystem.h"
#include "ResourceManager.h"

Scene::Scene() : Resource()
{
	type = ResourceType::SCENE;
}

Scene::~Scene()
{
}

bool Scene::CreateMetaData(const u64& force_id)
{
	if (force_id != 0) {
		ID = force_id;
	}
	else {
		resourceManager->GetRandomID();
	}

	app->fs->NormalizePath(path);
	name = app->fs->GetBaseFileName(path.data());

	App->file_system->CreateNewFile(path.data());

	JSON_Value* value = json_value_init_object();
	JSON_Object* json_object = json_value_get_object(value);

	std::string meta_path = std::string(App->file_system->GetPathWithoutExtension(path) + "_meta.alien");

	json_serialize_to_file_pretty(value, meta_path.data());

	if (value != nullptr && json_object != nullptr) {

		JSONfilepack* file = new JSONfilepack(meta_path, json_object, value);
		file->StartSave();
		file->SetString("Meta.ID", std::to_string(ID));
		file->FinishSave();
		delete file;
	}

	meta_data_path = LIBRARY_SCENES_FOLDER + std::to_string(ID) + ".alienScene";
	App->file_system->CreateNewFile(meta_data_path.data());
	App->resources->AddResource(this);
	return true;
}

bool ResourceScene::ReadBaseInfo(const char* assets_file_path)
{
	path = std::string(assets_file_path);
	name = App->file_system->GetBaseFileName(path.data());

	std::string meta_path = std::string(App->file_system->GetPathWithoutExtension(path) + "_meta.alien");

	ID = App->resources->GetIDFromAlienPath(meta_path.data());

	if (ID != 0) {
		meta_data_path = LIBRARY_SCENES_FOLDER + std::to_string(ID) + ".alienScene";

		if (!App->file_system->Exists(meta_data_path.data())) {
			App->file_system->Copy(assets_file_path, meta_data_path.data());
		}
		else {
			// TODO: look what to do when game mode
			struct stat meta_file;
			struct stat assets_file;
			stat(path.data(), &assets_file);
			stat(meta_data_path.data(), &meta_file);
			if (assets_file.st_mtime != meta_file.st_mtime) {
				remove(meta_data_path.data());
				App->file_system->Copy(path.data(), meta_data_path.data());
			}
		}
		App->resources->AddResource(this);
		return true;
	}
	return false;
}

void ResourceScene::ReadLibrary(const char* meta_data)
{
	meta_data_path = meta_data;

	ID = std::stoull(App->file_system->GetBaseFileName(meta_data_path.data()));

	JSON_Value* value = json_parse_file(meta_data_path.data());
	JSON_Object* object = json_value_get_object(value);

	if (value != nullptr && object != nullptr)
	{
		JSONfilepack* meta = new JSONfilepack(meta_data_path, object, value);
		name = meta->GetString("Scene.Name");
		delete meta;
	}

	App->resources->AddResource(this);
}

bool ResourceScene::DeleteMetaData()
{
	remove(meta_data_path.data());
	return true;
}
