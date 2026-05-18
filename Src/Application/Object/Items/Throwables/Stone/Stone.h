#pragma once
#include "../Throwables.h"

class Stone : public Throwables
{
public:
	Stone() {};
	~Stone() override { Throwables::~Throwables(); };

	void Init() override;
	void Update(float dt) override;

	bool Charge(float dt) override;
	void ThrowStart(Math::Vector2 startPos, float dirAngle, float power) override;
	void OnCollision(Collider* self, const HitResult& hit)override;

	void DrawRequest() override;

private:
	float chargeRatio = 1.0f;
	float chargeMax = 700.0f;
	float currentMouseDir = 0.0f;

	// 飛翔・移動計算用のパラメータ
	Math::Vector2 startPos{};
	Math::Vector2 targetPos{};
	Math::Vector2 flightDir{};   // 飛翔方向の正規化ベクトル
	
	float flightDistance = 0.0f; // スタートから着弾点までの総距離
	float currentMoved = 0.0f;   // 現在進んだ距離
	float moveSpeed = 800.0f;    // 飛翔速度

	// 着弾後（バウンド＆フェードアウト）の管理
	const float MAX_BOUNCE_TIME = 0.6f; // 消滅までの総バウンド時間
	float lifeTimer = 0.0f;
	bool  isHitDeactivated = false;     // 当たり判定をオフにしたかフラグ

	float rotateSpeed = 0.0f;

	bool isEnemyHit = false;
};