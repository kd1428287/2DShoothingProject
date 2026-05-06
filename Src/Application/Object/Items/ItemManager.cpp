#include "ItemManager.h"


#include "Throwables/Stone/Stone.h"
#include "Throwables/Bomb/Bomb.h"

std::unique_ptr<Items> ItemManager::CreateItem(ItemType type)
{
	std::unique_ptr<Items> item;
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
	return item;
}