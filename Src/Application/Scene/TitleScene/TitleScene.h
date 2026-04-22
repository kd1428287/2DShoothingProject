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

	void onEnter(float dt)override;
	void onExit(float dt)override;

protected:


private:

};