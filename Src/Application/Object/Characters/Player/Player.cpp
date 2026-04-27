#include "Player.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/System/CollisionManager/Collider.h"

void Player::Init()
{
	objParameter.tex = RESOURCE.GetTexture("player");

    // 本体コライダー
    auto body = std::make_unique<Collider>(
        this, CollisionLayer::PlayerBody);
    //body->SetAABB({ 20.0f,32.0f });
    body->SetCircle(32.0f);
    AddCollider(std::move(body));

}

void Player::PreUpdate(float dt)
{
}

void Player::Update(float dt)
{
}

void Player::OnCollision(Collider* self, const HitResult& hit)
{
}

