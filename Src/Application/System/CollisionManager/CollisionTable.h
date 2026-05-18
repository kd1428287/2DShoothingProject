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
    /*                  None    PBody   EBody   PProj   Break   Barri   EAtta*/
    /* None       */ {  false,  false,  false,  false,  false,  false,  false},
    /* PlayerBody */ {  false,  false,  true,   false,  false,  false,  false},
    /* EnemyBody  */ {  false,  true,   false,  true,   false,  true,   false},
    /* PProjectile*/ {  false,  false,  true,   false,  true,   false,  false},
    /* Breakable  */ {  false,  false,  true,   true,   false,  true,   false},
    /* Barrier    */ {  false,  false,  true,   false,  true,   true,   true},
    /* EAttack    */ {  false,  false,  false,  false,  false,  true,   false},
   
};

inline bool CanCollide(CollisionLayer a, CollisionLayer b)
{
    return CollisionTable[(int)a][(int)b];
}
