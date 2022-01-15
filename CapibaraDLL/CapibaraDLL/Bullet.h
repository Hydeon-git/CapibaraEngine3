#pragma once
#include "Capibara.h"
#include "CapibaraScripts.h"

class CAPIBARA_ENGINE_API Bullet : public Capi {

public:

	Bullet();
	virtual ~Bullet();

	void Start();
	void Update();

public:

	float life_time = 3.0f;
	float time = 0.0f;
	float velocity = 10.0f;
	float3 bullet_direction = { 0,0,0 };
};

CAPIBARA_FACTORY Bullet* CreateBullet() {
	Bullet* bullet = new Bullet();
	// To show in inspector here
	SHOW_IN_INSPECTOR_AS_DRAGABLE_FLOAT(bullet->life_time);
	SHOW_IN_INSPECTOR_AS_DRAGABLE_FLOAT(bullet->velocity);
	return bullet;
} 
