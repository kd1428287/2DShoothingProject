#include "Bomb.h"
#include "Application/System/InputManager/InputManager.h"
#include "Application/System/CollisionManager/Collider.h"
#include "Application/System/CollisionManager/CollisionManager.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include <cmath>

void Bomb::Init()
{
	// 画像全体は 128x64 ですが、描画サイズ(1コマ)は 64x64 として設定します
	objParameter.tex = RESOURCE.GetTexture("bomb");
	objParameter.scale = { 1.0f, 1.0f };
	objParameter.size = { 64.0f, 64.0f }; // ★1コマ分のサイズ
	objParameter.rectPosition = { 0.0f, 0.0f }; // 初期状態は左側の未点火状態
	objParameter.color = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 投擲直後（飛翔中）の敵直撃用コライダーとして初期化
	auto col = std::make_unique<Collider>(this, CollisionLayer::PlayerProjectile);
	col->SetCircle(20.0f);
	col->SetDamage(2);
	col->SetEnable(false);

	AddCollider(std::move(col));

	thParameter.state = ThrowState::None;
	isShadow = false;

	// 手に持っているときは未点火状態を維持
	isIgnitedSprite = false;
}

// ★変更：自身の現在位置をもとにマウス方向を取得し、入力検知と投擲呼び出しを完結させる
bool Bomb::Charge(float dt)
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
		// 指を離したら自ら投擲を開始し、投げたことを通知する
		ThrowStart(objParameter.position, currentMouseDir, chargeRatio);
		chargeRatio = 1.0f;
		return true;
	}

	return false;
}

void Bomb::ThrowStart(Math::Vector2 startPos_, float dirAngle, float power)
{
	startPos = startPos_;
	groundPos = startPos;
	SetPosition(startPos);

	flightDir = Math::Vector2(std::cos(dirAngle + DirectX::XMConvertToRadians(90)), std::sin(dirAngle + DirectX::XMConvertToRadians(90)));
	targetPos = startPos + flightDir * power;

	flightDistance = (targetPos - startPos).Length();
	currentMoved = 0.0f;

	objParameter.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	objParameter.flashValue = 0.0f;

	if (!colliders_.empty()) {
		colliders_[0]->SetPosition(startPos);
		colliders_[0]->SetLayer(CollisionLayer::PlayerProjectile);
		colliders_[0]->SetEnable(false); // 飛翔中の直撃判定を有効化
	}

	thParameter.state = ThrowState::Throw;

	rotateSpeed = 0.05f + rand() % 30 * 0.01f;
	auto dir = flightDir;
	dir.Normalize();
	if (dir.x > 0) rotateSpeed *= -1;

	isShadow = true;
	isPlanted = false;
	isExploding = false;

	// 起爆タイマーとアニメーションの初期化
	timeToExplode = INITIAL_EXPLODE_TIME;
	animTimer = 0.0f;
	isIgnitedSprite = false;
}

void Bomb::StartExplosion()
{
	if (isExploding) return;

	isExploding = true;
	explosionTimer = MAX_EXPLOSION_TIME;

	// 爆発時は専用の爆発テクスチャ等に切り替え、切り出し位置もリセットします
	KdTexture* expTex = RESOURCE.GetTexture("explosion");
	if (expTex) {
		objParameter.tex = expTex;
		objParameter.size = { 90.0f,64.0f };
		objParameter.rectPosition = { 0.0f, 0.0f }; // 全体表示へ戻す
	}
	else {
		objParameter.color = { 1.0f, 0.3f, 0.1f, 1.0f };
	}

	objParameter.scale = { 6.5f, 6.5f }; // 爆風エリア全体に拡大
	objParameter.angle = 0.0f;
	objParameter.flashValue = 0.0f;
	rotateSpeed = 0.0f;

	// 当たり判定を「広範囲の爆風攻撃」に切り替え
	if (!colliders_.empty()) {
		colliders_[0]->SetCircle(192.0f);
		colliders_[0]->SetDamage(5);
		colliders_[0]->SetLayer(CollisionLayer::PlayerProjectile);
		colliders_[0]->SetPosition(groundPos);
		colliders_[0]->SetEnable(true);
	}

	SetPosition(groundPos);
}

void Bomb::UpdateAnimation(float dt)
{
	// 手に持っているとき、または既に起爆しているときは処理しない
	if (thParameter.state == ThrowState::None || isExploding)
	{
		objParameter.rectPosition = { 0.0f, 0.0f }; // 手に持っている時は未点火状態
		return;
	}

	// 起爆までの残り時間に応じて、アニメーション切り替え間隔を計算
	float ratio = timeToExplode / INITIAL_EXPLODE_TIME;
	if (ratio < 0.0f) ratio = 0.0f;

	// 切り替え間隔の短縮処理
	float currentInterval = 0.05f + (0.3f * std::max(0.0f,ratio - 0.3f));

	animTimer += dt;
	if (animTimer >= currentInterval)
	{
		animTimer = 0.0f;
		isIgnitedSprite = !isIgnitedSprite; // 状態の反転
	}

	// 決定した状態に基づいて画像の切り出し位置 (UV) を設定
	if (isIgnitedSprite) {
		objParameter.rectPosition = { 64.0f, 0.0f }; // 右側の点火状態
	}
	else {
		objParameter.rectPosition = { 0.0f, 0.0f };  // 左側の未点火状態
	}
}

void Bomb::Update(float dt)
{
	if (objParameter.isDead) return;

	// ----------------------------------------------------
	// 爆発中の処理
	// ----------------------------------------------------
	if (isExploding)
	{
		explosionTimer -= dt;

		float ratio = explosionTimer / MAX_EXPLOSION_TIME;
		objParameter.color.w = ratio; // フェードアウト

		if (explosionTimer < MAX_EXPLOSION_TIME * 0.5f) {
			if (!colliders_.empty()) colliders_[0]->SetEnable(false);
		}

		if (explosionTimer <= 0.0f)
		{
			objParameter.isDead = true;
			Destroy();
		}
		return;
	}

	// 起爆時間のカウントダウンとアニメーション更新
	if (thParameter.state != ThrowState::None)
	{
		timeToExplode -= dt;
		UpdateAnimation(dt); // コマ切り替え処理を実行

		if (timeToExplode <= 0.0f)
		{
			StartExplosion(); // 自動起爆
			return;
		}
	}

	// 回転処理
	objParameter.angle += rotateSpeed * dt;

	switch (thParameter.state)
	{
	case ThrowState::None:
		// 手に持っている状態のアニメーション固定を適用
		UpdateAnimation(dt);
		break;

	case ThrowState::Throw:
	{
		currentMoved += moveSpeed * dt;

		float t = 0.0f;
		if (flightDistance > 0.0f) {
			t = currentMoved / flightDistance;
		}

		// 着地判定
		if (t >= 1.0f)
		{
			groundPos = targetPos;
			SetPosition(groundPos);
			objParameter.scale = { 1.0f, 1.0f };

			thParameter.state = ThrowState::Point; // 設置状態へ移行
			bounceTimer = MAX_BOUNCE_TIME;
			isPlanted = true;

			// 自機の攻撃や爆風を受け止めて誘爆するようにレイヤーを変更
			if (!colliders_.empty()) {
				colliders_[0]->SetPosition(groundPos);
				colliders_[0]->SetLayer(CollisionLayer::breakable);
				colliders_[0]->SetEnable(true);
			}

			rotateSpeed *= 0.3f;
		}
		else
		{
			// 放物線表現
			float heightBonus = std::sin(t * 3.141592f) * 1.0f;
			objParameter.scale = { 1.0f + heightBonus * 0.2f, 1.0f + heightBonus * 0.2f };

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
		// 設置直後の小さく重いバウンド演出
		if (bounceTimer > 0.0f)
		{
			bounceTimer -= dt;
			float bounceRatio = 1.0f - (bounceTimer / MAX_BOUNCE_TIME);

			float currentBounceSpeed = moveSpeed * 0.1f * (1.0f - bounceRatio);
			groundPos += flightDir * currentBounceSpeed * dt;

			float bounceAngle = bounceRatio * 3.141592f * 2.0f;
			float maxHeight = 12.0f * (1.0f - bounceRatio);
			float bounceHeight = std::abs(std::sin(bounceAngle)) * maxHeight;

			SetPosition(groundPos + Math::Vector2(0.0f, bounceHeight));

			if (!colliders_.empty()) {
				colliders_[0]->SetPosition(groundPos);
			}

			if (bounceTimer <= 0.0f) {
				rotateSpeed = 0.0f;
				SetPosition(groundPos);
			}
		}
		break;
	}

	default:
		break;
	}
}

void Bomb::OnCollision(Collider* self, const HitResult& hit)
{
	if (objParameter.isDead || isExploding || !hit.other) return;

	// 1. 投擲直後（飛翔中）に敵へ直撃した場合
	if (thParameter.state == ThrowState::Throw)
	{
		if (hit.other->GetLayer() == CollisionLayer::EnemyBody)
		{
			StartExplosion(); // 即起爆
		}
	}
	// 2. 設置後に誘爆条件を満たした場合
	else if (thParameter.state == ThrowState::Point)
	{
		// プレイヤー側の攻撃判定（Stoneや別のBombの爆風）を受けた場合
		if (hit.other->GetLayer() == CollisionLayer::PlayerProjectile)
		{
			StartExplosion(); // 誘爆
		}
	}
}

void Bomb::DrawRequest()
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

		predData.scale *= 7.0f;

		RENDERM.Submit(predData);
	}

	if (isShadow && !isExploding) {
		DrawShadow(groundPos, -30.0f);
	}

	objParameter.footPosition = groundPos.y;

	BaseObject::DrawRequest();
}