#include "Stone.h"
#include "Application/System/InputManager/InputManager.h"
#include "Application/System/CollisionManager/Collider.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include <cmath>

void Stone::Init()
{
	objParameter.tex = RESOURCE.GetTexture("stone");
	objParameter.scale = { 0.8f, 0.8f };
	objParameter.size = { 64.0f, 64.0f };
	objParameter.color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 初期色（完全不透明）

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
	// ★追加：自身の現在位置をもとにマウス方向を取得
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
		// ★追加：指を離したら自ら投擲を開始し、投げたことを通知する
		ThrowStart(objParameter.position, currentMouseDir, chargeRatio);
		chargeRatio = 1.0f;
		return true;
	}

	return false;
}

void Stone::ThrowStart(Math::Vector2 startPos_, float dirAngle, float power)
{
	startPos = startPos_;
	groundPos = startPos;
	SetPosition(startPos);

	// 方向ベクトルを計算して保持（慣性バウンドでも使用するため）
	flightDir = Math::Vector2(std::cos(dirAngle + DirectX::XMConvertToRadians(90)), std::sin(dirAngle + DirectX::XMConvertToRadians(90)));
	targetPos = startPos + flightDir * power;

	flightDistance = (targetPos - startPos).Length();
	currentMoved = 0.0f;

	// 色を完全不透明にリセット（再利用などを考慮）
	objParameter.color = { 1.0f, 1.0f, 1.0f, 1.0f };

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
			groundPos = targetPos; // バウンドの基準位置を着弾点に設定
			SetPosition(groundPos);
			objParameter.scale = { 1.0f, 1.0f };

			// 着弾した瞬間に当たり判定をオン
			if (!colliders_.empty()) {
				colliders_[0]->SetPosition(groundPos);
				colliders_[0]->SetEnable(true);
			}

			if (rotateSpeed > 0.0f) rotateSpeed += 5.0f;
			else rotateSpeed -= 5.0f;

			thParameter.state = ThrowState::Point;
			lifeTimer = MAX_BOUNCE_TIME; // バウンド全体の時間をセット
			isHitDeactivated = false;    // 判定オフ用フラグをリセット
		}
		else
		{
			// 飛翔中の放物線表現（スケール変化）
			float heightBonus = std::sin(t * 3.141592f) * 1.0f;
			float alpha = 1.0f - heightBonus * 0.3f;
			objParameter.color = { 1.0f, 1.0f, 1.0f, alpha };
			objParameter.scale = { 0.8f * (1.0f + heightBonus), 0.8f * (1.0f + heightBonus) };

			Math::Vector2 currentPos = startPos + (targetPos - startPos) * t;
			groundPos = currentPos;
			currentPos.y += heightBonus * 100.0f * flightDistance / chargeMax;
			SetPosition(currentPos);

			if (!colliders_.empty()) {
				colliders_[0]->SetPosition(currentPos);
			}
		}
		break;
	}

	case ThrowState::Point:
	{
		// 1. 着弾直後のフレームで当たり判定をすぐに消す（一瞬だけヒットさせるため）
		if (!isHitDeactivated && hitCount_ >= 0.1f)
		{
			if (!colliders_.empty()) {
				colliders_[0]->SetEnable(false);
			}
			isHitDeactivated = true;
		}
		else if (hitCount_ < 0.1f)
		{
			hitCount_ += dt;
		}

		lifeTimer -= dt;
		if (lifeTimer <= 0.0f)
		{
			objParameter.isDead = true;
			objParameter.color.w = 0.0f; // 完全に透明にする

			if (objParameter.color.w <= 0.0f) Destroy();
		}
		else
		{
			// バウンド進行度（0.0：着弾直後 〜 1.0：消滅直前）
			float bounceRatio = 1.0f - (lifeTimer / MAX_BOUNCE_TIME);

			// 2. 慣性移動：投げた方向へ少しだけ転がす（時間経過で減速させる）
			float currentBounceSpeed = moveSpeed * 0.25f * (1.0f - bounceRatio);
			groundPos += flightDir * currentBounceSpeed * dt;

			// 3. バウンド高さ計算
			// abs(sin) を使って跳ねる動きを作る。角度に 3.0\pi をかけると約2〜3回跳ねる
			float bounceAngle = bounceRatio * 3.141592f * 3.0f;
			// 跳ねる高さの上限も時間経過で低くしていく
			float maxHeight = 24.0f * (1.0f - bounceRatio);
			float bounceHeight = std::abs(std::sin(bounceAngle)) * maxHeight;

			// 描画位置の更新（地面座標 + 上方向へのバウンド高さ）
			// ※画面上方向がYプラスの座標系を想定しています
			SetPosition(groundPos + Math::Vector2(0.0f, bounceHeight));

			// 4. 透明度の更新（フェードアウト）
			// 残り時間に応じて 1.0 (不透明) から 0.0 (透明) へ減衰させる
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

	// ★変更：影生成ロジックを使わずに、個別にObjectDataを作成して予測円を描画
	if (thParameter.state == ThrowState::None && chargeRatio > 1.0f)
	{
		Math::Vector2 predDir(std::cos(currentMouseDir + DirectX::XMConvertToRadians(90)),
			std::sin(currentMouseDir + DirectX::XMConvertToRadians(90)));

		Math::Vector2 predPos = objParameter.position + predDir * chargeRatio;

		ObjectData predData;
		predData.tex = RESOURCE.GetTexture("shadow");
		predData.position = { predPos.x, predPos.y - 30.0f }; // 影と同じ足元オフセット
		predData.size = { 64.0f, 64.0f };
		predData.scale = { 1.0f, 0.35f }; // 見下ろし視点に合わせて楕円形に潰す
		predData.scale *= 2.0f;
		predData.rectPosition = { 0.0f, 0.0f };
		predData.angle = 0.0f;
		predData.priority = objParameter.priority - 0.1f; // 本体の背面に描画
		predData.footPosition = predPos.y;
		predData.color = { 1.0f, 1.0f, 1.0f, 0.5f }; // 半透明で表示
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
		DrawShadow(groundPos, -8.0f);
	}

	BaseObject::DrawRequest();
}