#pragma once
#include "CollisionLayer.h"

// 判定テーブル
// true  = 判定する
// false = 判定しない
constexpr bool CollisionTable
[(int)CollisionLayer::Count]
[(int)CollisionLayer::Count] =
{
    //左が自分、上が相手
    /*                  None    PBody   EBody   Foot    PMel    PProj   EMel    EProj   Item    break   Shield  Sage    Mouse   UI  */
    /* None       */ {  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false},
    /* PlayerBody */ {  false,  false,  true,   false,  false,  false,  true,   true,   true,   false,  true,   false,  true,   false},
    /* EnemyBody  */ {  false,  true,   false,  false,  true,   true,   false,  false,  false,  false,  true,   false,  true,   false},
    /* EnemyFoot  */ {  false,  true,   false,  false,  false,  false,  false,  false,  false,  false,  true,   true,   true,   false},
    /* PMelee     */ {  false,  false,  true,   false,  false,  false,  false,  false,  false,  true,  true,   false,  true,   false},
    /* PProjectile*/ {  false,  false,  true,   false,  false,  false,  false,  false,  false,  true,  false,  true,   true,   false},
    /* EMelee     */ {  false,  true,   false,  false,  false,  false,  false,  false,  false,  true,  true,   false,  true,   false},
    /* EProjectile*/ {  false,  true,   false,  false,  false,  false,  false,  false,  false,  true,  true,   true,   true,   false},
    /* Item       */ {  false,  true,   false,  false,  false,  false,  false,  false,  false,  true,   false,  true,   true,   false},
    /* breakable  */ {  false,  false,  false,  false,  true,  true,  true,  true,  false,   false,  false,  false,  false,  false},
    /* Shield     */ {  false,  true,   true,   false,  true,   false,  true,   true,   true,   false,  false,  false,  true,   false},
    /* Stage      */ {  false,  true,   true,   true,   false,  true,   false,  true,   true,   false,  false,  false,  true,   false},
    /* Mouse      */ {  false,  true,   true,   true,   true,   true,   true,   true,   true,   false,  true,   true,   false,  true },
    /* UI         */ {  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  false,  true,   false}
};

inline bool CanCollide(CollisionLayer a, CollisionLayer b)
{
    return CollisionTable[(int)a][(int)b];
}
