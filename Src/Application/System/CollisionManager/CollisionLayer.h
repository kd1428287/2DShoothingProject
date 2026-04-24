#pragma once

enum class CollisionLayer
{
    None = 0,

    // ===== 本体 =====
    PlayerBody,        // 自機本体
    EnemyBody,         // 敵本体
    Foot,              // 地形判定

    // ===== 自機攻撃 =====
    PlayerMelee,       // 自機近接攻撃
    PlayerProjectile,  // 自機遠距離攻撃

    // ===== 敵攻撃 =====
    EnemyMelee,        // 敵近接攻撃
    EnemyProjectile,   // 敵遠距離攻撃

    // ==== アイテム ====
    Item,              // アイテム
    breakable,// 自機アイテム(経験値?)取得範囲

    // ===== 防御・特殊 =====
    Shield,            // Knightの盾
    Stage,             // 地形
    Mouse,             // マウス座標
    UI,                // UI判定

    Count
};