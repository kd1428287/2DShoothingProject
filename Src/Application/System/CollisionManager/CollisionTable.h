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
    /*                  None    PBody   EBody   PProj   Break*/
    /* None       */ {  false,  false,  false,  false,  false},
    /* PlayerBody */ {  false,  false,  true,   false,  false},
    /* EnemyBody  */ {  false,  true,   false,  true,   false},
    /* PProjectile*/ {  false,  false,  true,   false,  true},
    /* Breakable  */ {  false,  false,  true,   true,   false},
   
};

inline bool CanCollide(CollisionLayer a, CollisionLayer b)
{
    return CollisionTable[(int)a][(int)b];
}
