#pragma once
#include "../Characters.h"

enum class EnemyState {
	Walk, Attack, Knockback, Dead
};

class BaseEnemy : public Characters {
public:
	BaseEnemy() {};
	virtual ~BaseEnemy() override;

	virtual void SetParameter();
	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void DrawRequest() override;

	virtual void Damage(int damage) override;
	//virtual void OnCollision(Collider* self, const HitResult& hit) override {};

	bool IsDeadCompletely() const { return isRemove_; }

protected:
	virtual void WalkUpdate(float dt);
	virtual void AttackUpdate(float dt);
	void AttackEnd();

	// 状態変数
	EnemyState state_ = EnemyState::Walk;

	// --- 攻撃用 (共通) ---
	float attackInterval_ = 1.0f;
	float attackTimer_ = 0.0f;
	float attackActiveDuration_ = 0.1f; // 攻撃判定の持続時間
	float attackActiveTimer_ = 0.0f;
	bool isTouchingBarrier_ = false;
	bool isAttack = false;
	bool isHit = false;

	// ★修正: 配列のインデックス依存をなくすため、それぞれのポインタを保持する
	Collider* bodyCollider_ = nullptr;
	Collider* attackCollider_ = nullptr;

	// --- ノックバック・死亡用 (共通パラメータ) ---
	Math::Vector2 groundPos_{};
	float kbSpeed_ = 0.0f;
	float timer_ = 0.0f;
	float maxKbTime_ = 0.25f;
	float maxDeadTime_ = 0.4f;
	float invincibleTimer_ = 0.0f;

	bool isRemove_ = false;

	float anim = 0.0f;
};