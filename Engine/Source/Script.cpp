#include "Script.h"
#include "Globals.h"

#include "Application.h"
#include "FileSystem.h"

#include "glew/include/GL/glew.h"

#include "Profiling.h"

Script::Script(uint uid, std::string& assets, std::string& library) : data(nullptr), width(0), height(0), id(0), Resource(uid, ResourceType::TEXTURE, assets, library), parameters({}){
	
}

Script::~Script()
{
	
}

void Script::Load()
{
	
}

void Script::UnLoad()
{
	
}