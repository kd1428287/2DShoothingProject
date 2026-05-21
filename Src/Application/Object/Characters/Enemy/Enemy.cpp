#include "Pch.h"
#include "Enemy.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/System/CollisionManager/Collider.h"
#include "Application/System/CollisionManager/CollisionManager.h"
#include "Application/System/ScoreManager/ScoreManager.h"
#include "Application/System/AudioManager/AudioManager.h"
#include <cmath>

BaseEnemy::~BaseEnemy() {
	
}

void BaseEnemy::Init() {
	// 1. 本体判定（共通で作成、サイズは後で各エネミーが調整）
	auto body = std::make_unique<Collider>(this, CollisionLayer::EnemyBody);
	body->SetCircle(24.0f);
	bodyCollider_ = body.get(); // 生成したコライダーの参照を保持
	AddCollider(std::move(body)); // 所有権はBaseObjectのベクタへ移動

	// 2. 攻撃判定（共通で作成、デフォルト無効）
	auto attack = std::make_unique<Collider>(this, CollisionLayer::EnemyAttack);
	attack->SetCircle(30.0f);
	attack->SetEnable(false);
	attackCollider_ = attack.get();
	AddCollider(std::move(attack));

	state_ = EnemyState::Walk;
}

void BaseEnemy::Damage(int damage) {
	if (invincibleTimer_ > 0.0f || state_ == EnemyState::Dead) return;

	charaParameter.HP -= damage;
	AUDIOM.PlaySe("damage");
	invincibleTimer_ = 0.1f;
	groundPos_ = objParameter.position;

	if (charaParameter.HP > 0) {
		state_ = EnemyState::Knockback;
		timer_ = maxKbTime_;
		kbSpeed_ = 120.0f;
	}
	else {
		state_ = EnemyState::Dead;
		timer_ = maxDeadTime_;
		kbSpeed_ = 180.0f;
		if (bodyCollider_) bodyCollider_->SetEnable(false); // 安全なポインタアクセスに変更
		objParameter.tex = RESOURCE.GetTexture("troll3");
		anim = 0;
	}
}

void BaseEnemy::AttackUpdate(float dt)
{
	// 判定表示タイマーの管理
	if (attackActiveTimer_ > 0.0f) {
		attackActiveTimer_ -= dt;
		if (attackActiveTimer_ <= 0.0f)
		{
			AttackEnd();
		}
	}

	attackTimer_ -= dt;
	if (attackTimer_ <= 0.0f) {
		if (attackCollider_) {
			//attackCollider_->SetEnable(true);
			attackActiveTimer_ = attackActiveDuration_;
			isHit = false;
		}
		attackTimer_ = attackInterval_ + attackActiveDuration_;
		isAttack = true; // 攻撃のタイミングが来たことを派生クラスに知らせる
	}
}

void BaseEnemy::AttackEnd()
{
	if (attackCollider_) {
		attackCollider_->SetEnable(false);
	}
}

//void BaseEnemy::OnCollision(Collider* self, const HitResult& hit) {
//
//}

void BaseEnemy::SetParameter()
{
	charaParameter.HP = 3;
	charaParameter.moveDir = { 0, -1 };
	charaParameter.moveSpeed = 100.0f;

	float x = rand() % 1200 - 600;
	objParameter.position = { x, 390.0f };

	Init();
}

void BaseEnemy::Update(float dt)
{
	if (isRemove_) return;

	

	if (invincibleTimer_ > 0.0f) { invincibleTimer_ -= dt; }

	switch (state_)
	{
	case EnemyState::Walk:
		WalkUpdate(dt);
		groundPos_ = objParameter.position;
		break;

	case EnemyState::Attack:
		AttackUpdate(dt);
		if (!isTouchingBarrier_) {
			state_ = EnemyState::Walk;
			objParameter.tex = RESOURCE.GetTexture("troll2");
		}
		break;

	case EnemyState::Knockback:
	{
		groundPos_ += Math::Vector2(0.0f, 1.0f) * kbSpeed_ * dt;
		kbSpeed_ -= kbSpeed_ * 5.0f * dt;

		timer_ -= dt;
		if (timer_ <= 0.0f)
		{
			objParameter.position = groundPos_;
			if (bodyCollider_) bodyCollider_->SetPosition(groundPos_);
			state_ = EnemyState::Walk;
		}
		else
		{
			float progress = 1.0f - (timer_ / maxKbTime_);
			float bounceHeight = std::sin(progress * 3.141592f) * 16.0f;
			objParameter.position = groundPos_ + Math::Vector2(0.0f, bounceHeight);

			if (bodyCollider_) bodyCollider_->SetPosition(groundPos_);
		}
		break;
	}

	case EnemyState::Dead:
	{
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
			float bounceHeight = std::sin(progress * 3.141592f) * 24.0f;
			objParameter.position = groundPos_ + Math::Vector2(0.0f, bounceHeight);
			objParameter.color.w = timer_ / maxDeadTime_;
		}
		break;
	}

	
	}

	isTouchingBarrier_ = false;
}

void BaseEnemy::WalkUpdate(float dt)
{
	Movement(dt);
}

void BaseEnemy::DrawRequest()
{
	if (isRemove_) return;

	DrawShadow(objParameter);
	BaseObject::DrawRequest();
}