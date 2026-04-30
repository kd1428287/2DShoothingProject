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

	void DrawRequest()override;

	virtual void OnCollision(Collider* self, const HitResult& hit)override;

private:

	void Throw(float pow, float dir);
	void Charge(float dt);
	void CalculateCursorDirection();

	PlayerParameter pParameter{};

	float chargeRatio = 0.0f;
	float mouseDir{};
	float mouseDis{};


};