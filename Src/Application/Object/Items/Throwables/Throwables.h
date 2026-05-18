#pragma once

#include "../Items.h"
#include "Application/System/CollisionManager/Collider.h"

enum class ThrowState
{
	None,
	Throw, // 飛翔中
	Point  // 着弾（効果発動中）
};

struct ThrowablesParameter
{
	ThrowState state = ThrowState::None;
	float damageRange{};
	int damage = 10;
	float moveSpeed = 800.0f; // 飛翔速度
};

class Throwables : public Items
{
public:
	Throwables() {};
	virtual ~Throwables() override { Items::~Items(); };

	virtual void Init() override;
	virtual void Update(float dt) override;

	// ★変更：投げられた瞬間に true を返すインターフェースに変更
	virtual bool Charge(float dt) { return false; };

	// 投擲開始の共通インターフェース
	// プレイヤーからスタート座標、飛ぶ方向、チャージ量（飛距離）を受け取る
	virtual void ThrowStart(Math::Vector2 startPos, float dirAngle, float power);

	void OnCollision(Collider* self, const HitResult& hit)override;

protected:
	// 着弾した瞬間に呼ばれる仮想関数（石と爆弾で個別実装）
	virtual void OnLanding() {};

protected:
	ThrowablesParameter thParameter{};

	Math::Vector2 startPos_{};
	Math::Vector2 targetPos_{};

	// 着弾後の効果時間（エフェクトや判定を残すフレーム/時間）
	float hitCount_ = 0.0f;
	float lifeTimer_ = 0.0f;
	bool isDead_ = false; // 削除用フラグ（必要に応じてObjectManager等で回収）
};