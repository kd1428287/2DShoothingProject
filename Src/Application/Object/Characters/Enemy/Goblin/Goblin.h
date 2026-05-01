#pragma once

#include "../Enemy.h"

class Goblin : public BaseEnemy
{
public:
	Goblin() {};
	~Goblin() {};

	void OnCollision(Collider* self, const HitResult& hit);

private:


};