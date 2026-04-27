#pragma once

class Player;
class BaseObject;

class PlayerManager
{
public:
	static PlayerManager& Instance()
	{
		static PlayerManager instance;
		return instance;
	}

	void AddPlayer(std::vector<std::unique_ptr<BaseObject>> vector);
	Player* GetPlayer()const { return player; }

private:
	Player* player = nullptr;

};