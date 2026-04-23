#pragma once

//class BaseScene;
#include "BaseScene.h"
#include "../System/RenderManager/RenderManager.h"

enum class SceneType
{
	Title,
	Game,
	Result
};

class SceneManager
{
public:
	~SceneManager() {};

	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	void PreUpdate(float dt) 
	{
		if (currentSceneType != nextSceneType && currentScene->NowState() == SceneState::endExit)
		{
			ChangeScene(nextSceneType);
		}
	};
	void Update(float dt) 
	{ 
		if (currentScene)currentScene->PreUpdate(dt);
		if (currentScene)currentScene->Update(dt);
	};

	void RequestDraw() { if (currentScene)currentScene->RequestDraw(); };

	void RequestSceneChange(SceneType _nextScene) 
	{
		nextSceneType = _nextScene; 
		currentScene->ChangeState(SceneState::onExit);
	}

private:
	SceneManager() {};
	~SceneManager() {};

	void Init() { if (currentScene)currentScene->Init(); }
	void ChangeScene(SceneType _nextScene);

	std::unique_ptr<BaseScene> currentScene = nullptr;
	SceneType currentSceneType = SceneType::Title;
	SceneType nextSceneType = currentSceneType;
};

#define SCENEMANAGER SceneManager::Instance()