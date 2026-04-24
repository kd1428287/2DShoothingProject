#include "CollisionManager.h"
#include "Application/Object/BaseObject.h"

CollisionManager& CollisionManager::Instance()
{
    static CollisionManager instance;
    return instance;
}

void CollisionManager::Register(Collider* collider)
{
    if (!collider) return;
    m_colliders.push_back(collider);
}

void CollisionManager::Unregister(Collider* col)
{
    auto it = std::find(m_colliders.begin(), m_colliders.end(), col);
    if (it != m_colliders.end())
    {
        m_colliders.erase(it);
    }
}

void CollisionManager::Update()
{
    CheckAllCollisions();
}

CollisionManager::CollisionManager() = default;
CollisionManager::~CollisionManager()  = default;

void CollisionManager::CheckAllCollisions()
{
    const size_t count = m_colliders.size();

    for (size_t i = 0; i < m_colliders.size(); ++i)
    {
        Collider* a = m_colliders[i];
        if (!a || !a->IsEnable()) continue;

        for (size_t j = i + 1; j < m_colliders.size(); ++j)
        {
            Collider* b = m_colliders[j];
   
            if (!b || !b->IsEnable()) continue;

            // 同一オブジェクトは無視
            if (a->GetOwner() == b->GetOwner()) continue;

            // レイヤー判定
            if (!CanCollide(a->GetLayer(), b->GetLayer())) continue;

            // 衝突詳細情報を計算
            HitResult hitA, hitB;
            if (CalculateHitDetail(a, b, hitA, hitB)) {
                if (a->GetOwner()) a->GetOwner()->OnCollision(a, hitA);
                if (b->GetOwner()) b->GetOwner()->OnCollision(b, hitB);

                //// 形状判定
                //if (!a->Intersects(*b)) continue;

                //// 衝突通知
                //if (a->GetOwner())
                //    a->GetOwner()->OnCollision(a, b);

                //if (b->GetOwner())
                //    b->GetOwner()->OnCollision(b, a);
            }
        }
    }
}

//衝突判定に加え、衝突した方向とめり込んだ距離まで計算して返す
//衝突判定自身の詳細はCollider.cppのIntersects関数群に注釈をつけてます
bool CollisionManager::CalculateHitDetail(Collider* a, Collider* b, HitResult& outA, HitResult& outB) {
    outA.other = b;
    outB.other = a;

    // AABB vs AABB
    if (a->GetShapeType() == Collider::ShapeType::AABB && b->GetShapeType() == Collider::ShapeType::AABB) {
        float dx = a->GetPos().x - b->GetPos().x;
        float dy = a->GetPos().y - b->GetPos().y;

        // Collider.cpp の IntersectsAABBAABB と同様の計算
        float overlapX = (a->GetSize().x + b->GetSize().x) - std::abs(dx);
        float overlapY = (a->GetSize().y + b->GetSize().y) - std::abs(dy);

        if (overlapX <= 0 || overlapY <= 0) return false;

        if (overlapX < overlapY) {
            outA.overlap = overlapX; outB.overlap = overlapX;
            outA.normal = { (dx > 0) ? 1.0f : -1.0f, 0.0f };
            outB.normal = { -outA.normal.x, 0.0f };
        }
        else {
            outA.overlap = overlapY; outB.overlap = overlapY;
            outA.normal = { 0.0f, (dy > 0) ? 1.0f : -1.0f };
            outB.normal = { 0.0f, -outA.normal.y };
        }
        return true;
    }

    // 円 vs 円
    if (a->GetShapeType() == Collider::ShapeType::Circle && b->GetShapeType() == Collider::ShapeType::Circle) {
        Math::Vector2 diff = a->GetPos() - b->GetPos();
        float dist = diff.Length();
        float rSum = a->GetSize().x + b->GetSize().x;

        if (dist > rSum || dist == 0) return false;

        outA.normal = diff / dist;
        outB.normal = { -outA.normal.x, -outA.normal.y };
        outA.overlap = rSum - dist;
        outB.overlap = outA.overlap;
        return true;
    }

    // --- Circle vs AABB (混合) ---
    Collider* circle = nullptr;
    Collider* aabb = nullptr;
    bool swap = false;

    if (a->GetShapeType() == Collider::ShapeType::Circle && b->GetShapeType() == Collider::ShapeType::AABB) {
        circle = a; aabb = b;
    }
    else if (a->GetShapeType() == Collider::ShapeType::AABB && b->GetShapeType() == Collider::ShapeType::Circle) {
        circle = b; aabb = a; swap = true; // aがAABBだったので、後で結果を入れ替える
    }

    if (circle && aabb) {
        Math::Vector2 cPos = circle->GetPos();
        Math::Vector2 aPos = aabb->GetPos();
        Math::Vector2 half = aabb->GetSize(); // Collider.cppの仕様に合わせる

        // 1. 矩形上の「円の中心に最も近い点」を探す
        float closestX = std::max(aPos.x - half.x, std::min(cPos.x, aPos.x + half.x));
        float closestY = std::max(aPos.y - half.y, std::min(cPos.y, aPos.y + half.y));

        Math::Vector2 closestPoint = { closestX, closestY };
        Math::Vector2 diff = cPos - closestPoint;
        float distanceSq = diff.LengthSquared();
        float radius = circle->GetSize().x; // 円の半径

        // 衝突していない
        if (distanceSq > radius * radius) return false;

        float distance = std::sqrt(distanceSq);
        Math::Vector2 normal;
        float overlap = 0.0f;

        if (distance > 0.0001f) {
            // 円の中心が矩形の外にある場合：最近接点から中心へのベクトルが法線
            normal = diff / distance;
            overlap = radius - distance;
        }
        else {
            // 円の中心が矩形の中に完全に潜り込んでいる場合（特殊ケース）
            // 上下左右の4辺のうち、最も近い辺から押し出す
            float dl = std::abs(cPos.x - (aPos.x - half.x));
            float dr = std::abs(cPos.x - (aPos.x + half.x));
            float dt = std::abs(cPos.y - (aPos.y + half.y));
            float db = std::abs(cPos.y - (aPos.y - half.y));

            float minDist = std::min({ dl, dr, dt, db });
            if (minDist == dl) normal = { -1, 0 };
            else if (minDist == dr) normal = { 1, 0 };
            else if (minDist == dt) normal = { 0, 1 };
            else normal = { 0, -1 };

            overlap = radius + minDist;
        }

        // 結果をセット
        if (!swap) {
            outA.normal = normal; outA.overlap = overlap;
            outB.normal = -normal; outB.overlap = overlap;
        }
        else {
            outA.normal = -normal; outA.overlap = overlap;
            outB.normal = normal; outB.overlap = overlap;
        }
        return true;
    }

    return false;
}