#include "Player.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/System/CollisionManager/Collider.h"
#include "Application/System/InputManager/InputManager.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/Scene/SceneManager.h"
#include "Application/Object/BaseObject.h"
#include "Application/Object/ObjectManager.h"
#include "Application/System/AudioManager/AudioManager.h"


// アイテム関連
#include "../../Items/ItemManager.h"
#include "../../Items/Throwables/Throwables.h"
#include "../../Items/Throwables/Stone/Stone.h"

Player::~Player()
{
    Characters::~Characters();
}

void Player::Init()
{
    objParameter.tex = RESOURCE.GetTexture("player");

    // 本体コライダーの設定
    auto body = std::make_unique<Collider>(this, CollisionLayer::PlayerBody);
    body->SetCircle(32.0f);
    AddCollider(std::move(body));

    objParameter.position = { 0.0f,-350.0f };
    objParameter.scale = { 1.3f, 1.3f };
    objParameter.size = { 52.0f, 64.0f };
    objParameter.priority = 1.0f;

    HP = 0;

    nowItem = ItemType::Stone;
    SelectItem();
}

void Player::PreUpdate(float dt)
{
   
}

void Player::Update(float dt)
{
    // ★変更：入力検知から投擲までの全責務をアイテムへ委譲
    if (hadItem)
    {
        // アイテム側が基準座標を得られるよう位置を追従させる
        //hadItem->SetPosition(objParameter.position);

        // アイテム自身のチャージ処理を実行し、投げられた瞬間ならマネージャーへ切り離す
        if (hadItem->Charge(dt))
        {
            ObjectManager::Instance().Add(std::move(hadItem));
            hadItem = nullptr;
            SelectItem();
        }
    }


    if (INPUT.IsTriggered(VK_RBUTTON))
    {
        SelectItem();
    }

    if (INPUT.IsTriggered('S'))
    {
        Speach = !Speach;
    }
    
}
void Player::DrawRequest()
{
    //BaseObject::DrawRequest();

    // ★変更：手持ちアイテムの描画（チャージ中の予測円もアイテム自らが描画します）
    if (hadItem)
    {
        hadItem->DrawRequest();
    }
}
void Player::Damage()
{
    RENDERM.StartDamageReaction();
    
    if (Speach)return;
    
    HP--;
    AUDIOM.PlaySe("damage1");

    if (HP <= 0)
    {
        HP = 0;
        SceneManager::Instance().RequestSceneChange(SceneType::Result);
    }
}

void Player::OnCollision(Collider* self, const HitResult& hit)
{
    // 被弾処理などをここに記述
}

void Player::SelectItem()
{
    // 石を生成して初期化
    if (!hadItem)
    {
        hadItem = ItemManager::Instance().CreateItem(nowItem);
    }

    if (INPUT.IsTriggered(VK_RBUTTON))
    {
        nowItem = static_cast<ItemType>(static_cast<int>(nowItem) + 1);
        if (nowItem == ItemType::Max)nowItem = ItemType::Stone;
    }
   
    hadItem = ItemManager::Instance().CreateItem(nowItem);
    Math::Vector2 pos = objParameter.position;// +Math::Vector2(30.0f, 0.0f);
    hadItem->SetPosition(pos);
}
