#pragma once
#include "../Enemy.h"

class Goblin : public BaseEnemy {
public:
	Goblin() {};
	void SetParameter() override;
	void Init() override;
	void DrawRequest()override;
	void OnCollision(Collider* self, const HitResult& hit) override;

protected:
	void WalkUpdate(float dt) override;
	void AttackUpdate(float dt) override;

private:
	
	float rectSize = 64.0f;
};