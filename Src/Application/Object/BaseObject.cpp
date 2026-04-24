#include "BaseObject.h"
#include "Application/System/CollisionManager/CollisionManager.h"


void BaseObject::AddCollider(std::unique_ptr<Collider> collider)
{
    CollisionManager::Instance().Register(collider.get());
    colliders_.push_back(std::move(collider));
}

const std::vector<std::unique_ptr<Collider>>&
BaseObject::GetColliders() const
{
    return colliders_;
}