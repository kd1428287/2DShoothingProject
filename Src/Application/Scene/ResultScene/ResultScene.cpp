#include "ResultScene.h"
#include "Application/System/RenderManager/ObjectData.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/Scene/SceneManager.h"
#include "Application/System/InputManager/InputManager.h"
#include "Application/System/UIManager/UIManager.h"
#include "Application/System/AudioManager/AudioManager.h"
#include "Application/Object/ObjectManager.h"



//‹Ù‹}
#include "Application/Object/Characters/Enemy/EnemyManager.h"


void ResultScene::Init()
{
	UIManager::Instance().CreateUI(ScenePaturn::Result);
	//AUDIOM.PlayBgm("result");
	AUDIOM.FadeOutAndPlayNext("drum", 0.1f, 0.1f, true);
}

void ResultScene::PreUpdate(float dt)
{
	ObjectManager::Instance().PreUpdate(dt);
}

void ResultScene::Update(float dt)
{
	//ObjectManager::Instance().Update(dt);
	UIManager::Instance().Update(dt);
	RenderManager::Instance().Update();
	//EnemyManager::Instance().EnemyRangeCheack();

	if (INPUT.IsTriggered(VK_LBUTTON) && UIManager::Instance().IsResultScoreDone())
	{
		SCENEMANAGER.RequestSceneChange(SceneType::Title);
		AUDIOM.PlaySe("click");
	}
	AUDIOM.Update();
}

void ResultScene::RequestDraw()
{
	ObjectManager::Instance().DrawRequest();
	UIManager::Instance().DrawRequest();
}

void ResultScene::onEnter()
{
}

void ResultScene::onExit()
{
}
