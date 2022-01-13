#pragma once

#include "Resource.h"

#include <string>

typedef unsigned char GLubyte;

struct ScriptParameters
{
	
};

class Script : public Resource
{
public:
	Script(uint uid, std::string& assets, std::string& library);

	~Script();

	void Load() override;
	void UnLoad() override;

	inline const std::string& GetPath() const { return path; }
	inline const int& GetWidth() const { return width; }
	inline const int& GetId() const { return id; }
	inline const int& GetHeight() const { return height; }

private:

private:
	unsigned int id;
	int width;
	int height;

	std::string path;
	GLubyte* data;

	ScriptParameters parameters;
};