#pragma once

enum class SceneChangeState
{
	None = 0,
	onEnter = 1,
	EndEnter = 2,
	Now = 3,
	onExit = 4,
	EndExit = 5
};

class BaseScene
{
public:
	BaseScene() {}
	~BaseScene() {}

	virtual void Init() = 0;
	virtual void PreUpdate(float dt) = 0;
	virtual void Update(float dt) = 0;
	virtual void RequestDraw() = 0;

	virtual void onEnter(float dt) = 0;
	virtual void onExit(float dt) = 0;

	void ChangeState(SceneChangeState next) { state = next; };
	SceneChangeState NowState() { return state; };

protected:

	SceneChangeState state = SceneChangeState::None;


private:

};