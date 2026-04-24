#pragma once

class Collider;

struct HitResult {
    Collider* other = nullptr;      // 当たった相手のコライダー
    Math::Vector2 normal{ 0, 0 };     // 衝突法線（相手から自分へ向かう方向）
    float overlap = 0.0f;           // めり込み量
};
