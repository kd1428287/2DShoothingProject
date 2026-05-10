#include "ResultScene.h"
#include "Application/System/RenderManager/ObjectData.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/Scene/SceneManager.h"
#include "Application/System/InputManager/InputManager.h"
#include "Application/System/UIManager/UIManager.h"
#include "Application/Object/ObjectManager.h"



//‹Ù‹}
#include "Application/Object/Characters/Enemy/EnemyManager.h"


void ResultScene::Init()
{
	UIManager::Instance().CreateUI(ScenePaturn::Result);
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
	if (INPUT.IsTriggered(VK_LBUTTON) && UIManager::Instance().IsResultScoreDone())SCENEMANAGER.RequestSceneChange(SceneType::Title);
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
