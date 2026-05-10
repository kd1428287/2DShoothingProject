#pragma once

class Items;
class Throwables;

enum class ItemType
{
	Stone,
	Bomb,
	Max,
};

class ItemManager
{
public:
	static ItemManager& Instance()
	{
		static ItemManager instance;
		return instance;
	}
	//std::unique_ptr<Items> CreateItem(ItemType type);
	std::unique_ptr<Throwables> CreateItem(ItemType type);
private:
	ItemManager() {};
	~ItemManager() {};

	
};