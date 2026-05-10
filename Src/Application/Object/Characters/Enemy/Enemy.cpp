#include "Pch.h"
#include "Enemy.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/System/CollisionManager/Collider.h"
#include "Application/System/CollisionManager/CollisionManager.h"
#include "Application/System/ScoreManager/ScoreManager.h"
#include <cmath> // std::sin用

BaseEnemy::~BaseEnemy()
{
	Characters::~Characters();

	
}

void BaseEnemy::SetParameter()
{
	charaParameter.HP = 3;
	charaParameter.moveDir = { 0, -1 };
	charaParameter.moveSpeed = 100.0f;

	float x = rand() % 1200 - 600;
	objParameter.position = { x, 390.0f };

	Init();
}

void BaseEnemy::Init()
{
	objParameter.tex = RESOURCE.GetTexture("troll");
	objParameter.scale = { 0.2f, 0.2f };
	objParameter.size = { 500, 500 };
	objParameter.color = { 1.0f, 1.0f, 1.0f, 1.0f };

	auto body = std::make_unique<Collider>(this, CollisionLayer::EnemyBody);
	body->SetCircle(24.0f);
	body->SetEnable(true);
	AddCollider(std::move(body));

	state_ = EnemyState::Walk;
}

void BaseEnemy::Update(float dt)
{
	if (isRemove_) return;

	if (invincibleTimer_ > 0.0f)
	{
		invincibleTimer_ -= dt;
	}

	switch (state_)
	{
	case EnemyState::Walk:
		WalkUpdate(dt);
		groundPos_ = objParameter.position;
		// Walk中もコライダーの位置を本体にしっかり追従させる
		if (!colliders_.empty()) {
			colliders_[0]->SetPosition(objParameter.position);
		}
		break;

	case EnemyState::Knockback:
	{
		// 1. 地面位置を {0, 1} 方向へ後退させる
		groundPos_ += Math::Vector2(0.0f, 1.0f) * kbSpeed_ * dt;

		// 摩擦で移動速度を少し減速させる
		kbSpeed_ -= kbSpeed_ * 5.0f * dt;

		timer_ -= dt;
		if (timer_ <= 0.0f)
		{
			// 着地して完了（座標を地面位置に戻す）
			objParameter.position = groundPos_;
			if (!colliders_.empty()) {
				colliders_[0]->SetPosition(groundPos_);
			}
			state_ = EnemyState::Walk;
		}
		else
		{
			// 2. 跳ねる高さ（バウンド）の計算
			// 進行度 (0.0：開始直後 〜 1.0：着地寸前)
			float progress = 1.0f - (timer_ / maxKbTime_);

			// sin波でピョンと1回跳ねる（最大で16px上空へ浮く）
			float bounceHeight = std::sin(progress * 3.141592f) * 16.0f;

			// 描画位置 ＝ 地面位置 ＋ 上方向へのバウンド高さ
			objParameter.position = groundPos_ + Math::Vector2(0.0f, bounceHeight);

			// ★当たり判定は浮いていても「地面（影）の位置」に残すのが2Dの鉄則
			if (!colliders_.empty()) {
				colliders_[0]->SetPosition(groundPos_);
			}
		}
		break;
	}

	case EnemyState::Dead:
	{
		// 死亡時も {0, 1} 方向へ後退させつつ跳ねて消える
		groundPos_ += Math::Vector2(0.0f, 1.0f) * kbSpeed_ * dt;
		kbSpeed_ -= kbSpeed_ * 3.0f * dt;

		timer_ -= dt;
		if (timer_ <= 0.0f)
		{
			isRemove_ = true;
			objParameter.color.w = 0.0f;

			ScoreManager::Instance().AddScore(100);

			Destroy();
		}
		else
		{
			float progress = 1.0f - (timer_ / maxDeadTime_);

			// 死亡時は少し高めに跳ねる（最大24px浮く）
			float bounceHeight = std::sin(progress * 3.141592f) * 24.0f;
			objParameter.position = groundPos_ + Math::Vector2(0.0f, bounceHeight);

			// フェードアウト（透明度減衰）
			objParameter.color.w = timer_ / maxDeadTime_;

			
		}
		break;
	}
	}
}

void BaseEnemy::WalkUpdate(float dt)
{
	Movement(dt);
	
}

void BaseEnemy::DrawRequest()
{
	if (isRemove_) return;

	// 1. 影の描画依頼
	// 敵の画像の中心が原点なら、足元は少し下にあるため offset で微調整できます
	// （例: 64x64の画像なら足元は -30.0f あたり）
	DrawShadow(groundPos_, -45.0f);

	objParameter.footPosition = groundPos_.y;

	// 2. 本体の描画依頼
	BaseObject::DrawRequest();
}
void BaseEnemy::OnCollision(Collider* self, const HitResult& hit)
{
	if (state_ == EnemyState::Dead || !hit.other) return;

	if (hit.other->GetLayer() == CollisionLayer::PlayerProjectile)
	{
		if (invincibleTimer_ > 0.0f) return;

		charaParameter.HP -= 1;
		invincibleTimer_ = 0.1f;

		// 現在の座標を跳ねる基準位置（地面）として記憶
		groundPos_ = objParameter.position;

		// ★要望通り、ノックバック方向は強制的に {0, 1} 固定！
		// 後退距離をもっと短くするため、初速スピードを小さく設定します

		if (charaParameter.HP > 0)
		{
			state_ = EnemyState::Knockback;
			maxKbTime_ = 0.25f; // ピョンと短く素早く跳ねる時間
			timer_ = maxKbTime_;

			// 初速120.0f程度にすれば、減速も含めて合計約15〜20pxしか後退しません（短い距離）
			kbSpeed_ = 120.0f;
		}
		else
		{
			state_ = EnemyState::Dead;
			maxDeadTime_ = 0.4f; // 0.4秒で跳ねながら消える
			timer_ = maxDeadTime_;

			// 死亡時も距離は抑えめにするが、生存時よりは少し後退させる
			kbSpeed_ = 180.0f;

			if (!colliders_.empty()) {
				colliders_[0]->SetEnable(false);
			}
		}
	}
}