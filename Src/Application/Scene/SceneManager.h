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
	
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	void PreUpdate(float dt);
	
	void Update(float dt);
	

	void RequestDraw() { if (currentScene)currentScene->RequestDraw(); };

	void RequestSceneChange(SceneType _nextScene) 
	{
		if (isTransitioning && currentSceneType == _nextScene)return;
		nextSceneType = _nextScene; 
		if(currentScene)currentScene->ChangeState(SceneState::onExit);
		isTransitioning = true;
	}

private:
	SceneManager() {};
	~SceneManager() {};

	void Init() { if (currentScene)currentScene->Init(); }
	void ChangeScene(SceneType _nextScene);

	std::unique_ptr<BaseScene> currentScene = nullptr;
	SceneType currentSceneType = SceneType::Title;
	SceneType nextSceneType = currentSceneType;

	bool isTransitioning = false;	//シーン遷移フラグ
};

#define SCENEMANAGER SceneManager::Instance()