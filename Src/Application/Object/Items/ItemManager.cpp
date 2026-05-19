#include "ItemManager.h"

#include "Throwables/Stone/Stone.h"
#include "Throwables/Bomb/Bomb.h"
#include "Barrier/Fence.h"

#include "Application/Object/BaseObject.h"
#include "Application/Object/ObjectManager.h"



std::unique_ptr<Throwables> ItemManager::CreateItem(ItemType type)
{
	std::unique_ptr<Throwables> item = nullptr;
	switch (type)
	{
	case ItemType::Stone:
		item = std::make_unique<Stone>();
		break;
	case ItemType::Bomb:
		item = std::make_unique<Bomb>();
		break;
	default:
		break;
	}

	item->Init();

	return item;
}

void ItemManager::CreateBarrier()
{
	std::unique_ptr<Fence> fence = nullptr;
	std::unique_ptr<BaseObject> result = nullptr;
	float x = -400.0f;

	for (int i = 0; i < 4; i++)
	{
		fence = std::make_unique<Fence>();
		fence->Init();
		fence->SetPosition(Math::Vector2(x,-210.0f));

		result = std::move(fence);

		ObjectManager::Instance().Add(std::move(result));
		x += 220.0f;
		if (i == 1)x = 180.0f;
	}
}
