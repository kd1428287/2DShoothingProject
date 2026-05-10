#pragma once
#include "../BaseScene.h"

class GameScene :public BaseScene
{
public:
	GameScene() {}
	~GameScene() {}

	static GameScene& Instance()
	{
		static GameScene instance;
		return instance;
	}

	void Init()override;
	void PreUpdate(float dt)override;
	void Update(float dt)override;
	void RequestDraw()override;

	void onEnter()override;
	void onExit()override;

protected:

	//int HP = 3;

private:

};