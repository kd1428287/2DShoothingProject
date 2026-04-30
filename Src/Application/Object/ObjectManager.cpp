#include "ObjectManager.h"
#include "BaseObject.h"

#include "Application/Object/Characters/Player/PlayerManager.h"
#include "Application/Object/Characters/Player/Player.h"

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
		break;
	case ObjectType::Prop:
		break;
	default:
		break;
	}
}

void ObjectManager::PreUpdate(float dt)
{
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
	}
}

void ObjectManager::DrawRequest()
{
	for (auto& obj : objects_)
	{
		obj->DrawRequest();
	}
}
