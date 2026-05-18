#include "Bomb.h"
#include "Application/System/InputManager/InputManager.h"
#include "Application/System/CollisionManager/Collider.h"
#include "Application/System/CollisionManager/CollisionManager.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include <cmath>

void Bomb::Init()
{
	objParameter.tex = RESOURCE.GetTexture("bomb");
	objParameter.scale = { 2.0f, 2.0f };
	objParameter.size = { 64.0f, 64.0f };
	objParameter.visualSize = { 64.0f,64.0f };
	objParameter.rectPosition = { 0.0f, 0.0f };
	objParameter.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	objParameter.priority = 1.0f;

	// ★追加：親クラスの DrawRequest や DrawShadow で足元位置を正しく算出させるための設定
	objParameter.tall = 30.0f;
	objParameter.floating = 0.0f;

	auto col = std::make_unique<Collider>(this, CollisionLayer::PlayerProjectile);
	col->SetCircle(20.0f);
	col->SetDamage(2);
	col->SetEnable(false);

	AddCollider(std::move(col));

	thParameter.state = ThrowState::None;
	isShadow = false;
	isIgnitedSprite = false;
}

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
		ThrowStart(objParameter.position, currentMouseDir, chargeRatio);
		chargeRatio = 1.0f;
		return true;
	}

	return false;
}

void Bomb::ThrowStart(Math::Vector2 startPos_, float dirAngle, float power)
{
	startPos = startPos_;
	SetPosition(startPos);

	flightDir = Math::Vector2(std::cos(dirAngle + DirectX::XMConvertToRadians(90)), std::sin(dirAngle + DirectX::XMConvertToRadians(90)));
	targetPos = startPos + flightDir * power;

	flightDistance = (targetPos - startPos).Length();
	currentMoved = 0.0f;

	objParameter.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	objParameter.flashValue = 0.0f;
	objParameter.floating = 0.0f; // 投げ始めの浮遊をリセット
	objParameter.priority = 0.0f;

	if (!colliders_.empty()) {
		colliders_[0]->SetPosition(startPos);
		colliders_[0]->SetLayer(CollisionLayer::PlayerProjectile);
		colliders_[0]->SetEnable(false);
	}

	thParameter.state = ThrowState::Throw;

	rotateSpeed = 0.05f + rand() % 30 * 0.01f;
	auto dir = flightDir;
	dir.Normalize();
	if (dir.x > 0) rotateSpeed *= -1;

	isShadow = true;
	isPlanted = false;
	isExploding = false;

	timeToExplode = INITIAL_EXPLODE_TIME;
	animTimer = 0.0f;
	isIgnitedSprite = false;
}

void Bomb::StartExplosion()
{
	if (isExploding) return;

	isExploding = true;
	explosionTimer = MAX_EXPLOSION_TIME;

	KdTexture* expTex = RESOURCE.GetTexture("explosion");
	if (expTex) {
		objParameter.tex = expTex;
		objParameter.size = { 90.0f, 64.0f };
		objParameter.rectPosition = { 0.0f, 0.0f };
	}
	else {
		objParameter.color = { 1.0f, 0.3f, 0.1f, 1.0f };
	}

	objParameter.scale = { 6.5f, 6.5f };
	objParameter.angle = 0.0f;
	objParameter.flashValue = 0.0f;
	objParameter.floating = 0.0f; // 爆発時は地面に接地させる
	rotateSpeed = 0.0f;

	if (!colliders_.empty()) {
		colliders_[0]->SetCircle(192.0f);
		colliders_[0]->SetDamage(5);
		colliders_[0]->SetLayer(CollisionLayer::PlayerProjectile);
		colliders_[0]->SetPosition(objParameter.position);
		colliders_[0]->SetEnable(true);
	}
}

void Bomb::UpdateAnimation(float dt)
{
	if (thParameter.state == ThrowState::None || isExploding)
	{
		objParameter.rectPosition = { 0.0f, 0.0f };
		return;
	}

	float ratio = timeToExplode / INITIAL_EXPLODE_TIME;
	if (ratio < 0.0f) ratio = 0.0f;

	float currentInterval = 0.05f + (0.3f * std::max(0.0f, ratio - 0.3f));

	animTimer += dt;
	if (animTimer >= currentInterval)
	{
		animTimer = 0.0f;
		isIgnitedSprite = !isIgnitedSprite;
	}

	if (isIgnitedSprite) {
		objParameter.rectPosition = { 64.0f, 0.0f };
	}
	else {
		objParameter.rectPosition = { 0.0f, 0.0f };
	}
}

void Bomb::Update(float dt)
{
	if (objParameter.isDead) return;

	if (isExploding)
	{
		explosionTimer -= dt;
		float ratio = explosionTimer / MAX_EXPLOSION_TIME;
		objParameter.color.w = ratio;

		if (explosionTimer < MAX_EXPLOSION_TIME * 0.5f) {
			if (!colliders_.empty()) colliders_[0]->SetEnable(false);
		}

		if (explosionTimer <= 0.0f) {
			objParameter.isDead = true;
			Destroy();
		}
		return;
	}

	if (thParameter.state != ThrowState::None)
	{
		timeToExplode -= dt;
		UpdateAnimation(dt);

		if (timeToExplode <= 0.0f) {
			StartExplosion();
			return;
		}
	}

	objParameter.angle += rotateSpeed * dt;

	switch (thParameter.state)
	{
	case ThrowState::None:
		UpdateAnimation(dt);
		break;

	case ThrowState::Throw:
	{
		currentMoved += moveSpeed * dt;
		float t = 0.0f;
		if (flightDistance > 0.0f) {
			t = currentMoved / flightDistance;
		}

		if (t >= 1.0f)
		{
			SetPosition(targetPos);
			objParameter.scale = { 1.0f, 1.0f };
			objParameter.floating = 0.0f; // 着地

			thParameter.state = ThrowState::Point;
			bounceTimer = MAX_BOUNCE_TIME;
			isPlanted = true;

			if (!colliders_.empty()) {
				colliders_[0]->SetPosition(objParameter.position);
				colliders_[0]->SetLayer(CollisionLayer::breakable);
				colliders_[0]->SetEnable(true);
			}
			rotateSpeed *= 0.3f;
		}
		else
		{
			// ★変更：論理的な地面位置はそのまま position へセットし、高さ表現を floating に任せる
			float heightBonus = std::sin(t * 3.141592f) * 2.0f;
			float alpha = 1.0f - heightBonus * 0.3f;
			objParameter.color = { 1.0f, 1.0f, 1.0f, alpha };
			//objParameter.scale = { 1.0f + heightBonus * 0.2f, 1.0f + heightBonus * 0.2f };
			objParameter.scale = { 0.8f * (1.0f + heightBonus), 0.8f * (1.0f + heightBonus) };

			Math::Vector2 currentFlatPos = startPos + (targetPos - startPos) * t;
			SetPosition(currentFlatPos);

			// 親の DrawRequest で自動的に上に浮いて描画されます
			// (※もし以前の実装でy座標に加算していた仕様に合わせる場合、符号は適宜調整してください)
			objParameter.floating = heightBonus * 100.0f * flightDistance / chargeMax;

			// コライダー座標の同期は直後に呼ばれる親クラスの PositionSync() が行うため記述不要
		}
		break;
	}

	case ThrowState::Point:
	{
		if (bounceTimer > 0.0f)
		{
			bounceTimer -= dt;
			float bounceRatio = 1.0f - (bounceTimer / MAX_BOUNCE_TIME);

			// ★変更：論理的な地面位置を直接移動させる
			float currentBounceSpeed = moveSpeed * 0.1f * (1.0f - bounceRatio);
			SetPosition(objParameter.position + flightDir * currentBounceSpeed * dt);

			float bounceAngle = bounceRatio * 3.141592f * 2.0f;
			float maxHeight = 12.0f * (1.0f - bounceRatio);

			// バウンドの高さを floating に預ける
			objParameter.floating = std::abs(std::sin(bounceAngle)) * maxHeight;

			if (bounceTimer <= 0.0f) {
				rotateSpeed = 0.0f;
				objParameter.floating = 0.0f; // バウンド完全終了
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
	if (objParameter.isDead|| !hit.other) return;

	if (!isExploding) {
		if (thParameter.state == ThrowState::Throw)
		{
			if (hit.other->GetLayer() == CollisionLayer::EnemyBody) {
				StartExplosion();
			}
		}
		else if (thParameter.state == ThrowState::Point)
		{
			if (hit.other->GetLayer() == CollisionLayer::PlayerProjectile) {
				StartExplosion();
			}
		}
	}
	else {
		Throwables::OnCollision(self, hit);
	}	
}

void Bomb::DrawRequest()
{
	if (objParameter.isDead) return;

	if (thParameter.state == ThrowState::None && chargeRatio > 1.0f)
	{
		Math::Vector2 predDir(std::cos(currentMouseDir + DirectX::XMConvertToRadians(90)),
			std::sin(currentMouseDir + DirectX::XMConvertToRadians(90)));

		Math::Vector2 predPos = objParameter.position + predDir * chargeRatio;

		ObjectData predData;
		predData.tex = RESOURCE.GetTexture("shadow");
		predData.position = { predPos.x, predPos.y - objParameter.tall };
		predData.size = { 64.0f, 64.0f };
		predData.scale = { 1.0f, 0.35f };
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

		predData.scale *= 7.0f;
		RENDERM.Submit(predData);
	}

	if (isShadow && !isExploding) {
		// ★変更：親クラスの機能にそのまま ObjectParameter を渡すだけで完結します
		DrawShadow(objParameter);
	}

	// ★削除：footPosition の手動代入は不要
	// （BaseObject::DrawRequest() 側で自動計算されるため）

	BaseObject::DrawRequest();
}