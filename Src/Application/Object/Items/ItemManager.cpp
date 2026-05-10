#include "ItemManager.h"


#include "Throwables/Stone/Stone.h"
#include "Throwables/Bomb/Bomb.h"

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