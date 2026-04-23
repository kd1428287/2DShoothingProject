#pragma once

//class BaseScene;
#include "BaseScene.h"
#include "../System/RenderManager/RenderManager.h"

class SceneManager
{
public:
	~SceneManager() {};

	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	void Init() { if (nowScene)nowScene->Init(); }
	void PreUpdate(float dt) { if (nowScene)nowScene->PreUpdate(dt); };
	void Update(float dt) 
	{ 
		if (nowScene)nowScene->Update(dt);
		if (nextScene) 
		{
			switch (nowScene->NowState())
			{
			case SceneChangeState::Now:nowScene->ChangeState(SceneChangeState::onExit);
				break;
			case SceneChangeState::EndEnter:nowScene = std::move(nextScene);
				Init();
				nowScene->ChangeState(SceneChangeState::onEnter);
				break;
			default:
				break;
			}
		}
	};

	void RequestDraw() { if (nowScene)nowScene->RequestDraw(); };

	void RequestSceneChange(std::unique_ptr<BaseScene> queue) { nextScene = std::move(queue); }
	void SetScene(std::unique_ptr<BaseScene> queue) { nowScene = std::move(queue); Init(); }

private:
	SceneManager() {};

	std::unique_ptr<BaseScene> nowScene = nullptr;
	std::unique_ptr<BaseScene> nextScene = nullptr;

};

#define SCENEMANAGER SceneManager::Instance()