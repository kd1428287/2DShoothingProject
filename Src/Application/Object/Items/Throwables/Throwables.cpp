#include "Throwables.h"
#include "Application/System/CollisionManager/CollisionManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"

void Throwables::Init()
{
	// 共通コライダーベースの作成（デフォルトは自機遠距離攻撃レイヤー）
	auto col = std::make_unique<Collider>(this, CollisionLayer::PlayerProjectile);
	col->SetDamage(thParameter.damage);
	col->SetEnable(false); // 飛んでいる最中は当たり判定を出さない
	AddCollider(std::move(col));
}

void Throwables::ThrowStart(Math::Vector2 startPos, float dirAngle, float power)
{
	startPos_ = startPos;
	SetPosition(startPos_);

	// チャージ量(power)を飛距離として目標座標(着弾点)を算出
	// DirectXのMath::Vector2を想定 (cos, sinで方向ベクトル作成)
	Math::Vector2 dirVec(std::cos(dirAngle), std::sin(dirAngle));
	targetPos_ = startPos_ + dirVec * power;

	thParameter.state = ThrowState::Throw; // 飛翔状態へ切り替え
}

void Throwables::Update(float dt)
{
	if (isDead_) return;

	switch (thParameter.state)
	{
	case ThrowState::None:
		// プレイヤーの手元にある状態。移動はプレイヤーに追従するかCharge処理内で行う
		break;

	case ThrowState::Throw:
	{
		// 目標地点に向かって移動
		Math::Vector2 currentPos = objParameter.position;
		Math::Vector2 vDir = targetPos_ - currentPos;
		float distToTarget = vDir.Length();

		// 1フレームで到達できる距離まできたら着弾とする
		float moveStep = thParameter.moveSpeed * dt;
		if (distToTarget <= moveStep)
		{
			SetPosition(targetPos_);
			thParameter.state = ThrowState::Point; // 着弾状態へ切り替え
			lifeTimer_ = 0.1f; // 判定を残す時間（例: 0.1秒）

			// コライダーの位置を着弾点に合わせる
			if (!colliders_.empty()) {
				colliders_[0]->SetPosition(targetPos_);
			}

			// 個別の着弾処理（効果の発動）を呼び出す
			OnLanding();
		}
		else
		{
			// 目標へ向かって等速移動
			vDir.Normalize();
			SetPosition(currentPos + vDir * moveStep);
		}
		break;
	}

	case ThrowState::Point:
	{
		// 着弾後の処理（一定時間当たり判定を出した後に消滅させる）
		lifeTimer_ -= dt;
		if (lifeTimer_ <= 0.0f)
		{
			// 当たり判定をオフにしてオブジェクトの寿命を終える
			if (!colliders_.empty()) {
				colliders_[0]->SetEnable(false);
			}
			isDead_ = true;
		}
		break;
	}

	default:
		break;
	}
}