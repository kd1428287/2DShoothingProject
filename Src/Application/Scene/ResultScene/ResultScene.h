#pragma once
#include "../BaseScene.h"

class ResultScene :public BaseScene
{
public:
	ResultScene() {}
	~ResultScene() {}

	static ResultScene& Instance()
	{
		static ResultScene instance;
		return instance;
	}

	void Init()override;
	void PreUpdate(float dt)override;
	void Update(float dt)override;
	void RequestDraw()override;

	void onEnter()override;
	void onExit()override;

protected:

private:

};