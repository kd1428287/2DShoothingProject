#include "Pch.h"
#include "SceneManager.h"

#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"	
#include "ResultScene/ResultScene.h"

void SceneManager::ChangeScene(SceneType _nextScene)
{
	switch (_nextScene)
	{
	case SceneType::Title:
		currentScene = std::make_unique<TitleScene>();
		break;
	case SceneType::Game:
		//currentScene = std::make_unique<GameScene>();
		break;
	case SceneType::Result:
		//currentScene = std::make_unique<ResultScene>();
		break;
	default:
		break;
	}

	Init();

	currentSceneType = nextSceneType;
}