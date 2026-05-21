#include "Stone.h"
#include "Application/System/InputManager/InputManager.h"
#include "Application/System/CollisionManager/Collider.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/System/CollisionManager/CollisionHitResult.h"
#include "Application/System/EffectManager/EffectManager.h"
#include "Application/System/AudioManager/AudioManager.h"
#include <cmath>

void Stone::Init()
{
	objParameter.tex = RESOURCE.GetTexture("stone");
	objParameter.scale = { 2.0f, 2.0f };
	objParameter.size = { 64.0f, 64.0f };
	objParameter.visualSize = { 64.0f * 0.8f,64.0f * 0.8f };
	objParameter.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	objParameter.priority = 1.0f;

	// ★追加：足元位置のオフセットと浮遊パラメータを初期化
	objParameter.tall = 30.0f;
	objParameter.floating = 0.0f;

	auto col = std::make_unique<Collider>(this, CollisionLayer::PlayerProjectile);
	col->SetCircle(16.0f);
	col->SetDamage(1);
	col->SetEnable(false);

	AddCollider(std::move(col));

	thParameter.state = ThrowState::None;

	isShadow = false;
}

bool Stone::Charge(float dt)
{
	currentMouseDir = INPUT.GetMouseDir(objParameter.position);

	if (INPUT.IsTriggered(VK_LBUTTON))
	{
		chargeRatio = 1.0f;
	}
	else if (INPUT.IsPressed(VK_LBUTTON))
	{
		chargeRatio += chargeMax * dt;
		if (chargeRatio >= chargeMax) chargeRatio = chargeMax;
	}
	else if (INPUT.IsReleased(VK_LBUTTON))
	{
		ThrowStart(objParameter.position, currentMouseDir, chargeRatio);
		chargeRatio = 1.0f;
		return true;
	}

	return false;
}

void Stone::ThrowStart(Math::Vector2 startPos_, float dirAngle, float power)
{
	startPos = startPos_;
	SetPosition(startPos);
	objParameter.scale = { 0.8f,0.8f };

	flightDir = Math::Vector2(std::cos(dirAngle + DirectX::XMConvertToRadians(90)), std::sin(dirAngle + DirectX::XMConvertToRadians(90)));
	targetPos = startPos + flightDir * power;

	flightDistance = (targetPos - startPos).Length();
	currentMoved = 0.0f;

	objParameter.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	objParameter.floating = 0.0f; // 投げ始めの高さをリセット
	objParameter.priority = 0.0f;

	if (!colliders_.empty()) {
		colliders_[0]->SetPosition(startPos);
		colliders_[0]->SetEnable(false);
	}

	thParameter.state = ThrowState::Throw;

	rotateSpeed = 0.1f + rand() % 50 * 0.1f;

	auto dir = flightDir;
	dir.Normalize();
	if (dir.x > 0) rotateSpeed *= -1;

	isShadow = true;
}

void Stone::OnCollision(Collider* self, const HitResult& hit)
{
	if (hit.other->GetLayer() == CollisionLayer::EnemyBody)
	{
		if (!isEnemyHit) {
			Throwables::OnCollision(self, hit);
			isEnemyHit = true;
		}
	}
}

void Stone::Update(float dt)
{
	if (objParameter.isDead) return;

	objParameter.angle += rotateSpeed * dt;

	switch (thParameter.state)
	{
	case ThrowState::None:
		break;

	case ThrowState::Throw:
	{
		currentMoved += moveSpeed * dt;

		float t = 0.0f;
		if (flightDistance > 0.0f) {
			t = currentMoved / flightDistance;
		}

		// 着弾判定
		if (t >= 1.0f)
		{
			SetPosition(targetPos); // バウンドの基準位置を着弾点に設定
			objParameter.scale = { 1.0f, 1.0f };
			objParameter.floating = 0.0f; // 着地

			AUDIOM.PlaySe("hit2");

			// 着弾した瞬間に当たり判定をオン
			if (!colliders_.empty()) {
				colliders_[0]->SetPosition(objParameter.position);
				colliders_[0]->SetEnable(true);
			}

			if (rotateSpeed > 0.0f) rotateSpeed += 5.0f;
			else rotateSpeed -= 5.0f;

			thParameter.state = ThrowState::Point;
			EffectManager::Instance().CreateEffect(EffectPaturn::impact1, objParameter.position, objParameter.visualSize * 2.0f);
			lifeTimer = MAX_BOUNCE_TIME;
			isHitDeactivated = false;
		}
		else
		{
			// 飛翔中の放物線表現
			float heightBonus = std::sin(t * 3.141592f) * 2.0f;
			float alpha = 1.0f - heightBonus * 0.3f;
			objParameter.color = { 1.0f, 1.0f, 1.0f, alpha };
			objParameter.scale = { 0.8f * (1.0f + heightBonus), 0.8f * (1.0f + heightBonus) };

			// ★変更：平面の移動位置を position にセットし、高さを floating に割り当てる
			Math::Vector2 currentFlatPos = startPos + (targetPos - startPos) * t;
			SetPosition(currentFlatPos);

			objParameter.floating = heightBonus * 100.0f * flightDistance / chargeMax;

			if (!colliders_.empty()) {
				colliders_[0]->SetPosition(currentFlatPos);
			}
		}
		break;
	}

	case ThrowState::Point:
	{
		// 1. 着弾直後のフレームで当たり判定をすぐに消す（一瞬だけヒットさせるため）
		if (!isHitDeactivated)
		{
			if (!colliders_.empty()) {
				colliders_[0]->SetEnable(false);
			}
			isHitDeactivated = true;
		}

		lifeTimer -= dt;
		if (lifeTimer <= 0.0f)
		{
			objParameter.isDead = true;
			objParameter.color.w = 0.0f;

			if (objParameter.color.w <= 0.0f) Destroy();
		}
		else
		{
			float bounceRatio = 1.0f - (lifeTimer / MAX_BOUNCE_TIME);

			// 2. 慣性移動：投げた方向へ少しだけ転がす（地面の座標自体を進める）
			float currentBounceSpeed = moveSpeed * 0.25f * (1.0f - bounceRatio);
			SetPosition(objParameter.position + flightDir * currentBounceSpeed * dt);

			// 3. バウンド高さ計算
			float bounceAngle = bounceRatio * 3.141592f * 3.0f;
			float maxHeight = 24.0f * (1.0f - bounceRatio);

			// ★変更：跳ねる高さを floating パラメータに預ける
			objParameter.floating = std::abs(std::sin(bounceAngle)) * maxHeight;

			// 4. 透明度の更新（フェードアウト）
			objParameter.color.w = lifeTimer / MAX_BOUNCE_TIME;
		}
		break;
	}

	default:
		break;
	}
}

void Stone::DrawRequest()
{
	if (objParameter.isDead) return;

	if (thParameter.state == ThrowState::None && chargeRatio > 1.0f)
	{
		Math::Vector2 predDir(std::cos(currentMouseDir + DirectX::XMConvertToRadians(90)),
			std::sin(currentMouseDir + DirectX::XMConvertToRadians(90)));

		Math::Vector2 predPos = objParameter.position + predDir * chargeRatio;

		ObjectData predData;
		predData.tex = RESOURCE.GetTexture("shadow");
		// ★変更：定数リテラルではなく objParameter.tall を活用
		predData.position = { predPos.x, predPos.y - objParameter.tall };
		predData.size = { 64.0f, 64.0f };
		predData.scale = { 1.0f, 0.35f };
		predData.scale *= 2.0f;
		predData.rectPosition = { 0.0f, 0.0f };
		predData.angle = 0.0f;
		predData.priority = objParameter.priority - 0.1f;
		predData.footPosition = predPos.y;
		predData.color = { 1.0f, 1.0f, 1.0f, 0.5f };
		predData.target = objParameter.target;
		predData.addEffect = false;
		predData.mat = Math::Matrix(1.f, 0, 0, 0,
			0, 1.f, 0, 0,
			0, 0, 1.f, 0,
			0, 0, 0, 1.f);

		RENDERM.Submit(predData);
	}

	// 飛翔・バウンド中の通常の影表示
	if (isShadow && thParameter.state != ThrowState::None)
	{
		// 親クラス側で objParameter.floating に応じた影の縮小・減衰処理が適用されます
		DrawShadow(objParameter);
	}

	BaseObject::DrawRequest();
}