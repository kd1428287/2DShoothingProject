#pragma once
#include "../Characters.h"

enum class EnemyState
{
	Walk,      // 通常の移動状態
	Knockback, // 被弾してピョンと短く跳ねて後退している状態
	Dead       // HP0になり、跳ねながら消滅中の状態
};

class BaseEnemy : public Characters
{
public:
	BaseEnemy() {};
	virtual ~BaseEnemy() override;

	virtual void SetParameter();
	virtual void Init() override;
	virtual void Update(float dt) override;
	virtual void DrawRequest() override;

	virtual void OnCollision(Collider* self, const HitResult& hit) override;

	bool IsDeadCompletely() const { return isRemove_; }

protected:
	virtual void WalkUpdate(float dt);

protected:
	EnemyState state_ = EnemyState::Walk;

	// バウンド・ノックバック用パラメータ
	Math::Vector2 groundPos_{};   // 跳ねる際の地面（影）の基準位置
	float kbSpeed_ = 0.0f;  // ノックバックの後退スピード
	float timer_ = 0.0f;  // 状態タイマー
	float maxKbTime_ = 0.25f; // 生存時の跳ねる時間（短め）
	float maxDeadTime_ = 0.4f;  // 死亡時の跳ねて消える時間

	float invincibleTimer_ = 0.0f;

	bool isRemove_ = false;
};