#include "Player.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/System/CollisionManager/Collider.h"
#include "Application/System/InputManager/InputManager.h"

#include "Application/System/RenderManager/RenderManager.h"
void Player::Init()
{
	objParameter.tex = RESOURCE.GetTexture("player");

    // 本体コライダー
    auto body = std::make_unique<Collider>(
        this, CollisionLayer::PlayerBody);
    //body->SetAABB({ 20.0f,32.0f });
    body->SetCircle(32.0f);
    AddCollider(std::move(body));

    objParameter.scale = { 1.0f,1.0f };
    objParameter.size = { 64.0f,64.0f };

}

void Player::PreUpdate(float dt)
{
    CalculateCursorDirection();
}

void Player::Update(float dt)
{
    Charge(dt);
}

void Player::DrawRequest()
{
    BaseObject::DrawRequest();

    // 矢印(仮置き)
    ObjectData data;
    data.tex = RESOURCE.GetTexture("player");

    // 拡縮計算
    float yScale = chargeRatio / 64.0f;
    //if (yScale > 8.0f)yScale = 8.0f;
    data.scale = { 1.0f,1.0f * yScale };

    // 位置計算
    float dir = mouseDir + DirectX::XMConvertToRadians(90);
    Math::Vector2 radius = { 45.0f,50.0f };
    Math::Vector2 scaleAdjust = { yScale,yScale };
    Math::Vector2 length = radius * scaleAdjust;
    Math::Vector2 vector = Math::Vector2(cos(dir) *  length.x, sin(dir) * length.y);
    
    data.position = objParameter.position + vector;

    data.size = { 64,64 };
    data.angle = mouseDir;
    data.target = DrawTarget::front;
    RENDERM.Submit(data);
}

void Player::OnCollision(Collider* self, const HitResult& hit)
{
}

void Player::Throw(float pow, float dir)
{
}

void Player::Charge(float dt)
{
    if (INPUT.IsTriggered(VK_LBUTTON))
    {
        // チャージ初期化
        chargeRatio = 1.0f;
    }
    else if (INPUT.IsPressed(VK_LBUTTON))
    {
        // チャージ中

        if (chargeRatio >= 600.0f)
        {
            chargeRatio = 600.0f;
            return;
        }

        //縦画面の過半
        chargeRatio += 600.0f * dt;
    }
    else if (INPUT.IsReleased(VK_LBUTTON))
    {
        // チャージ終わり
        Throw(chargeRatio, mouseDir);
        chargeRatio = 1.0f;
    }
}

void Player::CalculateCursorDirection()
{
    POINT mp;
    INPUT.GetMousePos(&mp);

    float dx = mp.x - objParameter.position.x;
    float dy = mp.y - objParameter.position.y;

    mouseDir = std::atan2(dx,dy) * -1;
    
    mouseDis = sqrt(dx * dx + dy * dy);
}

