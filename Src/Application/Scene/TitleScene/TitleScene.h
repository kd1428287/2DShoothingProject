#pragma once
#include "../BaseScene.h"

class TitleScene :public BaseScene
{
public:
	TitleScene() {}
	~TitleScene() {}

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