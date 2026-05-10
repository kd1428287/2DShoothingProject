#pragma once

enum class CollisionLayer
{
    None = 0,

    PlayerBody,        // 自機本体
    EnemyBody,         // 敵本体
    PlayerProjectile,  // 自機遠距離攻撃
    breakable,         // 破壊可能

    Count
};