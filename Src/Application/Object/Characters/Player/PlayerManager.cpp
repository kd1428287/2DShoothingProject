#include "PlayerManager.h"

#include "Player.h"

void PlayerManager::AddPlayer(std::vector<std::unique_ptr<BaseObject>> vector)
{
	if (!player)
	{
		auto p = std::make_unique<Player>();
		p->Init();
		player = p.get();
		vector.push_back(std::move(p));
	}
};