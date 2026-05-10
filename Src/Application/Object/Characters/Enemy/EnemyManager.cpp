#include "EnemyManager.h"
#include "Application/Object/Characters/Player/Player.h"
#include "Application/Object/Characters/Player/PlayerManager.h"
#include "Application/Object/ObjectManager.h"

#include "Enemy.h"
#include "Goblin/Goblin.h"

void EnemyManager::AutoMake(float dt)
{
	makeLevel += levelSpeed * dt;
	makeCount += makeSpeed * dt;

	makeSpeed = 0.5f + 0.01f * makeLevel;
	int num = std::min(std::max(1, (int)makeLevel / 5) + rand() % maxMakeNum, maxMakeNum);
	if (makeCount > 1.0f)
	{
		for (int i = 0; i < num; i++)
		{
			ObjectManager::Instance().Add(ObjectType::Enemy);
			makeCount = 0.0f;
		}
	}
}

std::unique_ptr<BaseEnemy> EnemyManager::MakeEnemy()
{
	//auto en = std::make_unique<BaseEnemy>();
	std::unique_ptr<BaseEnemy> en = std::make_unique<Goblin>();
	en->SetParameter();
	enemyList.push_back(en.get());
	return en;
}

void EnemyManager::EnemyRangeCheack()
{
	for (auto& enemy : enemyList)
	{
		if (enemy->GetPosition().y < -420.0f)
		{
			enemy->Destroy();
			PlayerManager::Instance().GetPlayer()->Damage();
		}
	}

	enemyList.erase(
		std::remove_if(enemyList.begin(), enemyList.end(),
			[](BaseEnemy* obj) {
				return obj->IsDead();
			}),
		enemyList.end()
	);
}

void EnemyManager::MakeTitle(float dt)
{
	makeCount += dt;

	if (makeCount > 0.1f)
	{
		ObjectManager::Instance().Add(ObjectType::Enemy);
		makeCount = 0.0f;
	}

}

void EnemyManager::Clear()
{
	enemyList.clear();
	makeLevel = 1.0f;
	maxMakeNum = 1;
	makeSpeed = 0.5f;
	makeCount = 0.0f;
	levelSpeed = 0.5f;
}
