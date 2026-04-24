#pragma once
#include "CollisionHitResult.h"
#include "Collider.h"
#include "CollisionTable.h"

//class Collider;

//--------------------------------------------
// CollisionManager
//--------------------------------------------
class CollisionManager
{
public:
    static CollisionManager& Instance();

    void Register(Collider* collider);      //ƒRƒ‰ƒCƒ_[(“–‚½‚è”»’è)“o˜^
    void Unregister(Collider* collider);    //“o˜^‰ğœ

    void Update();

    void Clear() { m_colliders.clear(); }
private:
    CollisionManager();
    ~CollisionManager();

    CollisionManager(const CollisionManager&) = delete;
    CollisionManager& operator=(const CollisionManager&) = delete;

private:
    void CheckAllCollisions();

    bool CalculateHitDetail(Collider* a, Collider* b, HitResult& outA, HitResult& outB);

private:
    std::vector<Collider*> m_colliders;
};
