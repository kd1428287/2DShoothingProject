#pragma once
#include "../Characters.h"

struct PlayerParameter
{
	float throwPower = 1.0f;
	float weight = 0.0f;
};

class Player :public Characters
{
public:
	Player() {};
	~Player()override {};

	void Init()override;
	void PreUpdate(float dt)override;
	void Update(float dt)override;

	virtual void OnCollision(Collider* self, const HitResult& hit)override;

private:
	PlayerParameter pParameter{};
};