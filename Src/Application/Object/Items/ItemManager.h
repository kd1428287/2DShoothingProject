#pragma once

class Items;
class Throwables;
class BaseObject;

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
	
	std::unique_ptr<Throwables> CreateItem(ItemType type);
	void CreateBarrier();
private:
	ItemManager() {};
	~ItemManager() {};

	
};