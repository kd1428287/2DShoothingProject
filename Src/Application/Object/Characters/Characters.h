#pragma once
#include "../BaseObject.h"

struct CharacterParameter
{
	int HP{};
	Math::Vector2 moveDir{};
	float moveSpeed{};


};
class Characters :public BaseObject
{
public:
	Characters() {};
	~Characters()override;

	virtual void Init()override;
	virtual void PreUpdate(float dt)override;
	virtual void Update(float dt)override;

	virtual void OnCollision(Collider* self, const HitResult& hit)override {};

protected:
	void Movement(float dt);

	CharacterParameter charaParameter{};

};