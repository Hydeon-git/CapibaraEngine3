#pragma once

#include "Resource.h"
#include <vector>
#include "MathGeoLib/src/Math/float3.h"
#include <list>

class Prefab;
class ResourceMesh;
class GameObject;
class ResourceManager;


class Scene : public Resource {

public:

	Scene();
	virtual ~Scene();

	// meta data
	bool CreateMetaData(const u64& force_id = 0);
	bool ReadBaseInfo(const char* assets_file_path);
	void ReadLibrary(const char* meta_data);
	bool DeleteMetaData();

private:
	ResourceManager* resourceManager;
};