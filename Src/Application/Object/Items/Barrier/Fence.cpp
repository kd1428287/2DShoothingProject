#include "Fence.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/System/CollisionManager/Collider.h"
#include "Application/System/CollisionManager/CollisionManager.h"
// ... (他のインクルード)

void Fence::Init()
{
    maxHP = 10;
    HP = maxHP;
    objParameter.tex = RESOURCE.GetTexture("fence");
    objParameter.position = { 0.0f, 0.0f };
    objParameter.scale = { 0.2f, 0.2f };
    objParameter.size = { 1000.0f, 500.0f };
    objParameter.visualSize = { 200.0f,100.0f };
    objParameter.tall = 50.0f;
    objParameter.visualSize = { 200.0f,100.0f };
    //objParameter.priority = 0.5f; // 必要に応じて設定

    // コライダー (敵の攻撃レイヤーと当たる設定)
    auto body = std::make_unique<Collider>(this, CollisionLayer::Barrier);
    body->SetAABB(Math::Vector2(100.0f, 10.0f));
    body->SetOffset(Math::Vector2(0.0f, -50.0f));
    AddCollider(std::move(body));
}

void Fence::Update(float dt)
{
    // --- 被弾時の揺れ減衰処理 ---
    if (hitShakeTimer > 0.0f)
    {
        hitShakeTimer -= dt;
        // 左右にサイン波で揺らす (周波数は適宜調整)
        float wave = std::sin(hitShakeTimer * 60.0f);
        shakeOffset.x = wave * hitShakeIntensity;
    }
    else
    {
        shakeOffset = { 0.0f, 0.0f };
    }
}

void Fence::OnCollision(Collider* self, const HitResult& hit)
{
    // 相手が「敵の攻撃」等のタグやレイヤーだった場合 (判定条件はプロジェクトの仕様に合わせてください)
    // ここでは単純に何かが当たったらダメージとして実装

    if(hit.other->GetLayer() == CollisionLayer::EnemyAttack)
    {
        // 1. HP減少
        HP -= 1;

        // 2. 揺れ演出のセット
        hitShakeTimer = 0.2f;      // 0.2秒間揺れる
        hitShakeIntensity = 10.0f; // 揺れ幅

        // 3. 残りHPに応じた色の変化 (例: 減るほど赤みがかる)
        float hpRatio = (float)HP / (float)maxHP;
        // 緑と青を減らすことで赤色を強調する
        objParameter.color.y = hpRatio;
        objParameter.color.z = hpRatio;

        // 4. HPが0になった時の判定
        if (HP <= 0)
        {
            // TODO: ここに死亡演出（煙エフェクトの生成やSE再生など）を書き込む

            Destroy(); // BaseObjectのフラグを立ててObjectManagerから削除されるようにする
        }
    }
}

void Fence::DrawRequest()
{
    // 描画位置に揺れオフセットを加算
    Math::Vector2 originalPos = objParameter.position;
    objParameter.position += shakeOffset;

    // 影の描画 (影は揺らさない方が自然な場合が多いので、originalPosを基準にする)
    //Math::Vector2 shadowPos = { originalPos.x, objParameter.footPosition };
    
    //DrawShadow(shadowPos, 0.0f);
    DrawShadow(objParameter);

    // 本体の描画
    BaseObject::DrawRequest();

    // 座標を元に戻す
    objParameter.position = originalPos;
}