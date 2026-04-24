#pragma once
#include "../BaseScene.h"

class GameScene :public BaseScene
{
public:
	GameScene() {}
	~GameScene() {}

	void Init()override;
	void PreUpdate(float dt)override;
	void Update(float dt)override;
	void RequestDraw()override;

	void onEnter()override;
	void onExit()override;

protected:

	KdTexture playerTex;

private:

};