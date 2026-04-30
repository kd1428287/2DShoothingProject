#include "GameScene.h"
#include "Application/System/RenderManager/ObjectData.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/Scene/SceneManager.h"
#include "Application/System/InputManager/InputManager.h"
#include "Application/Object/ObjectManager.h"


void GameScene::Init()
{
	Math::Vector2 startPoint = { 0,-300 };
	ObjectManager::Instance().Add(ObjectType::Player,startPoint);
}

void GameScene::PreUpdate(float dt)
{
	ObjectManager::Instance().PreUpdate(dt);
}

void GameScene::Update(float dt)
{
	ObjectManager::Instance().Update(dt);
	if (INPUT.IsTriggered('Z'))SCENEMANAGER.RequestSceneChange(SceneType::Title);
}

void GameScene::RequestDraw()
{
	ObjectManager::Instance().DrawRequest();
}

void GameScene::onEnter()
{
}

void GameScene::onExit()
{
}
