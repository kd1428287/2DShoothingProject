#include "TitleScene.h"
#include "Application/System/RenderManager/ObjectData.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/Scene/SceneManager.h"
#include "Application/System/InputManager/InputManager.h"
#include "Application/System/UIManager/UIManager.h"
#include "Application/System/AudioManager/AudioManager.h"
#include "Application/Object/ObjectManager.h"
#include "Application/Object/Characters/Enemy/EnemyManager.h"

void TitleScene::Init()
{
	ObjectManager::Instance().Clear();
	EnemyManager::Instance().Clear();
	UIManager::Instance().CreateUI(ScenePaturn::Title);
	//AUDIOM.PlayBgm("title");
	AUDIOM.FadeOutAndPlayNext("title", 1.0f, 1.0f, true);
}

void TitleScene::PreUpdate(float dt)
{
	ObjectManager::Instance().PreUpdate(dt);
}

void TitleScene::Update(float dt)
{
	EnemyManager::Instance().MakeTitle(dt);
	ObjectManager::Instance().Update(dt);
	UIManager::Instance().Update(dt);
	RenderManager::Instance().Update();
	
	if (INPUT.IsTriggered(VK_LBUTTON))
	{
		SCENEMANAGER.RequestSceneChange(SceneType::Game);
		AUDIOM.PlaySe("click");
	}

	AUDIOM.Update();
}

void TitleScene::RequestDraw()
{
	ObjectManager::Instance().DrawRequest();
	UIManager::Instance().DrawRequest();
}

void TitleScene::onEnter()
{
}

void TitleScene::onExit()
{
}
