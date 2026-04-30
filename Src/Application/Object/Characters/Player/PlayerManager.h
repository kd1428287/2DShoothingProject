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

	std::unique_ptr<Player> MakePlayer(Math::Vector2 position);
	Player* GetPlayer()const { return player; }

private:
	Player* player = nullptr;

};