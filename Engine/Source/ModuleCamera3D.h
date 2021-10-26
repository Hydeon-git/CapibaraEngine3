#pragma once
#include "Module.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool startEnabled = true);
	~ModuleCamera3D();

	bool Start();
	bool Update(float dt) override;
	bool CleanUp();

	bool LoadConfig(JsonParsing& node) override;
	bool SaveConfig(JsonParsing& node) const override;

	void Look(const Vec3 &pos, const Vec3 &ref, bool rotateAroundReference = false);
	void LookAt(const Vec3 &spot);
	void Move(const Vec3 &movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	Vec3 x, y, z, position, reference, objectSelected;

private:

	Mat4x4 viewMatrix, viewMatrixInverse;
};