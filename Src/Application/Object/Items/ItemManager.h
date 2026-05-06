#pragma once

class Items;

enum class ItemType
{
	Stone,
	Bomb
};

class ItemManager
{
	ItemManager() {};
	~ItemManager() {};

	std::unique_ptr<Items> CreateItem(ItemType type);
	
};