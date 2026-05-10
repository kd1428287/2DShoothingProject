#pragma once

class BaseEnemy;

class EnemyManager
{
public:
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}

	void AutoMake(float dt);

	std::unique_ptr<BaseEnemy> MakeEnemy();
	void EnemyRangeCheack();

	void MakeTitle(float dt);

	void Clear();
	//BaseEnemy* GetEnemy()const { return enemy; }

private:
	EnemyManager() {};
	~EnemyManager() {};

	std::vector<BaseEnemy*> enemyList;

	float makeLevel = 1.0f;
	int maxMakeNum = 1;
	float makeSpeed = 2.0f;
	float makeCount = 0.0f;
	float levelSpeed = 0.5f;
};