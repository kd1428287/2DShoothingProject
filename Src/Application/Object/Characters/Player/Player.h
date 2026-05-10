#pragma once
#include "../Characters.h"
#include <memory>

#include "Application/Object/Items/Throwables/Throwables.h"

//class Throwables;
enum class ItemType;

struct PlayerParameter
{
	float throwPower = 1.0f;
	float weight = 0.0f;
};

class Player : public Characters
{
public:
	Player() {};
	~Player() override;

	void Init() override;
	void PreUpdate(float dt) override;
	void Update(float dt) override;

	void DrawRequest() override;

	void Damage();
	int GetHP() { return HP; }

	virtual void OnCollision(Collider* self, const HitResult& hit) override;

private:
	void SelectItem();

	int HP = 3;

	PlayerParameter pParameter{};

	// ★変更：ObjectManagerへの所有権譲渡要件を満たすため unique_ptr で一元管理
	std::unique_ptr<Throwables> hadItem = nullptr;
	ItemType nowItem = static_cast<ItemType>(0);

	bool Speach = false;
};