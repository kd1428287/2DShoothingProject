#pragma once

enum class SceneState
{
	onEnter,
	endEnter,
	onExit,
	endExit
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

	virtual void onEnter() = 0;
	virtual void onExit() = 0;

	void ChangeState(SceneState next) { state = next; };
	SceneState GetState() { return state; };

protected:

	SceneState state = SceneState::onEnter;


private:

};