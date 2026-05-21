#include "Goblin.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/System/CollisionManager/Collider.h"
#include "Application/System/CollisionManager/CollisionManager.h"
#include "Application/System/AudioManager/AudioManager.h"
#include "Application/Object/ObjectManager.h"

void Goblin::SetParameter() {
	charaParameter.HP = 1;
	charaParameter.moveDir = { 0, -1 };
	charaParameter.moveSpeed = 120.0f;
	objParameter.position = { (float)(rand() % 1000 - 500), 430.0f };
	Init();
}

void Goblin::Init() {
	BaseEnemy::Init();

	objParameter.tex = RESOURCE.GetTexture("troll2");
	objParameter.scale = { 3.0f, 3.0f };
	objParameter.size = { 64.0f, 64.0f };
	objParameter.visualSize = { 64.0f, 64.0f };
	objParameter.tall = 32.0f;

	isAttack = false;
	isTouchingBarrier_ = false;
	attackInterval_ = 1.0f;
	attackActiveDuration_ = 0.5f; // ゴブリン用に調整

	// 親が作成したコライダーをゴブリンの体格に合わせる
	if (bodyCollider_) bodyCollider_->SetCircle(45.0f);
	if (attackCollider_) {
		attackCollider_->SetCircle(60.0f);
		attackCollider_->SetDamage(1);
	}
}

void Goblin::DrawRequest()
{
	objParameter.rectPosition.x = (int)anim * rectSize;
	BaseEnemy::DrawRequest();
}

void Goblin::OnCollision(Collider* self, const HitResult& hit)
{
	if (state_ == EnemyState::Dead || !hit.other) return;

	// 1. バリア接触判定
	if (hit.other->GetLayer() == CollisionLayer::Barrier) {
		isTouchingBarrier_ = true;

		if (state_ == EnemyState::Walk) {
			state_ = EnemyState::Attack;
			attackTimer_ = attackInterval_;
			objParameter.tex = RESOURCE.GetTexture("troll1");
			anim = 0.0f;
			isAttack = false;
		}
	}

	// 2. 攻撃判定（相手にダメージを与える）
	if (self == attackCollider_) {
		BaseObject* target = hit.other->GetOwner();
		if (target) {
			target->Damage(self->GetDamage());
			attackCollider_->SetEnable(false);
			isHit = false;
			AUDIOM.PlaySe("hit1");
		}
	}
}

void Goblin::WalkUpdate(float dt) {
	BaseEnemy::WalkUpdate(dt);
	anim += dt * 5;
	if (anim >= 8.0f) anim = 0;
}

void Goblin::AttackUpdate(float dt)
{
	// タイマー管理などは親クラスに任せる
	BaseEnemy::AttackUpdate(dt);

	// 攻撃アニメーション処理
	float animSpeed = 8.0f / attackInterval_;
	if (isAttack)
	{
		anim += dt * animSpeed;
		if (anim >= 8.0f)
		{
			anim = 0.0f;
			isAttack = false;
		}
		else if (!isHit && anim >= 2.0f)
		{
			isHit = true;
			attackCollider_->SetEnable(true);
		}
		
	}

	
}