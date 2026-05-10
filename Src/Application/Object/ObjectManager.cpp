#include "ObjectManager.h"
#include "BaseObject.h"

#include "Application/Object/Characters/Player/PlayerManager.h"
#include "Application/Object/Characters/Player/Player.h"


#include "Application/Object/Characters/Enemy/EnemyManager.h"
#include "Application/Object/Characters/Enemy/Enemy.h"


ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::Add(std::unique_ptr<BaseObject> obj)
{
	if (obj)
	{
		pendingObjects_.push_back(std::move(obj));
	}
}

void ObjectManager::Add(ObjectType type)
{
	Add(type, { 0,0 });
}

void ObjectManager::Add(ObjectType type, Math::Vector2 pos)
{
	switch (type)
	{
	case ObjectType::Player:
		Add(std::move(PlayerManager::Instance().MakePlayer(pos)));
		break;
	case ObjectType::Enemy:
		Add(std::move(EnemyManager::Instance().MakeEnemy()));
		break;
	case ObjectType::Prop:
		break;
	default:
		break;
	}
}

void ObjectManager::RangeCheck()
{
	Math::Vector2 pos;
	for (auto& obj : objects_)
	{
		pos = obj->GetPosition();
		if (pos.x > 700.0f || pos.x < -700.0f ||
			pos.y > 450.0f || pos.y < -450.0f)
		{
			obj->Destroy();
		}
	}
}

void ObjectManager::PreUpdate(float dt)
{
	// 1. 削除フラグが立っているものを除去
	objects_.erase(
		std::remove_if(objects_.begin(), objects_.end(),
			[](const std::unique_ptr<BaseObject>& obj) {
				return obj->IsDead();
			}),
		objects_.end()
	);

	if (!pendingObjects_.empty())
	{
		// 効率化のためにメモリを予約（任意）
		objects_.reserve(objects_.size() + pendingObjects_.size());

		for (auto& newObj : pendingObjects_)
		{
			objects_.push_back(std::move(newObj));
		}
		// 移動し終わった保留リストは空にする
		pendingObjects_.clear();
	}
}

void ObjectManager::Update(float dt)
{
	for (auto& obj : objects_)
	{
		obj->PreUpdate(dt);
		obj->Update(dt);
		obj->PositionSync();
	}
}

void ObjectManager::DrawRequest()
{
	for (auto& obj : objects_)
	{
		obj->DrawRequest();
	}
}

void ObjectManager::Clear()
{
	objects_.clear();
	pendingObjects_.clear();
}
