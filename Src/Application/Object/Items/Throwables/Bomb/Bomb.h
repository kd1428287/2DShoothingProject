#pragma once
#include "../Throwables.h"

class Bomb : public Throwables
{
public:
	Bomb() {};
	~Bomb() override { Throwables::~Throwables(); };

	void Init() override;
	void Update(float dt) override;

	// ★変更：戻り値を bool に変更し、mouseDir引数を削除
	bool Charge(float dt) override;
	void ThrowStart(Math::Vector2 startPos, float dirAngle, float power) override;

	void DrawRequest() override;
	void OnCollision(Collider* self, const HitResult& hit) override;

private:
	void StartExplosion(); // 起爆して爆風を発生させる処理
	void UpdateAnimation(float dt); // 導火線アニメーションの更新

private:
	float chargeRatio = 1.0f;
	float chargeMax = 700.0f;
	float currentMouseDir = 0.0f; // ★追加：取得したマウス方向を保持

	Math::Vector2 startPos{};
	Math::Vector2 targetPos{};
	Math::Vector2 flightDir{};
	Math::Vector2 groundPos{};

	float flightDistance = 0.0f;
	float currentMoved = 0.0f;
	float moveSpeed = 700.0f; // 爆弾の重さを表現するため少し遅め

	// バウンド・設置・起爆時間の管理
	const float MAX_BOUNCE_TIME = 0.4f;
	float bounceTimer = 0.0f;
	const float INITIAL_EXPLODE_TIME = 5.0f; // 投擲から起爆までのトータル時間
	float timeToExplode = INITIAL_EXPLODE_TIME;
	bool  isPlanted = false;

	// 画像切り替え（導火線アニメーション）管理
	float animTimer = 0.0f;
	bool  isIgnitedSprite = false; // 現在点火状態のコマを表示しているか

	// 爆発演出管理
	bool  isExploding = false;
	float explosionTimer = 0.0f;
	const float MAX_EXPLOSION_TIME = 0.25f; // 爆風が残る時間

	float rotateSpeed = 0.0f;
};