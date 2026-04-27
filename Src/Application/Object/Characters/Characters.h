#pragma once
#include "../BaseObject.h"

struct CharacterParameter
{
	int HitPoint{};
	Math::Vector2 moveDir{};
};
class Characters :public BaseObject
{
public:
	Characters() {};
	~Characters()override {};

	virtual void Init()override;
	virtual void PreUpdate(float dt)override;
	virtual void Update(float dt)override;

	virtual void OnCollision(Collider* self, const HitResult& hit)override {};

protected:
	void Movement(float dt);

	CharacterParameter charaParameter{};

};