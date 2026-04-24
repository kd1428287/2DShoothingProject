#include "Pch.h"
#include "SceneManager.h"

#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"	
#include "ResultScene/ResultScene.h"

#include "Application/System/InputManager/InputManager.h"

void SceneManager::PreUpdate(float dt)
{
	if (isTransitioning)ChangeScene(nextSceneType);
}

void SceneManager::Update(float dt)
{
	if (currentScene && !isTransitioning)
	{
		currentScene->PreUpdate(dt);
		currentScene->Update(dt);
	}

	INPUT.Update();
}

void SceneManager::ChangeScene(SceneType _nextScene)
{
	if (currentSceneType != _nextScene || currentScene == nullptr)
	{
		RENDERM.Fadeout([this, _nextScene]() {

			switch (_nextScene)
			{
			case SceneType::Title:
				currentScene = std::make_unique<TitleScene>();
				break;
			case SceneType::Game:
				currentScene = std::make_unique<GameScene>();
				break;
			case SceneType::Result:
				//currentScene = std::make_unique<ResultScene>();
				break;
			default:
				break;
			}

			currentSceneType = nextSceneType;
			Init();

			});
	}
	else
	{
		RENDERM.Fadein([this, _nextScene]() {
			isTransitioning = false;
			});
	}
}