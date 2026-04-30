#include "PlayerManager.h"

#include "Player.h"

std::unique_ptr<Player> PlayerManager::MakePlayer(Math::Vector2 position)
{
	auto pl = std::make_unique<Player>();
	pl->Init();
	pl->SetPosition(position);
	return pl;
}
