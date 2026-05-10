#include "GameScene.h"
#include "Application/System/RenderManager/ObjectData.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/Scene/SceneManager.h"
#include "Application/System/InputManager/InputManager.h"
#include "Application/System/UIManager/UIManager.h"
#include "Application/System/ScoreManager/ScoreManager.h"
#include "Application/Object/ObjectManager.h"



//‹Ù‹}
#include "Application/Object/Characters/Enemy/EnemyManager.h"


void GameScene::Init()
{
	Math::Vector2 startPoint = { 0,-300 };
	ObjectManager::Instance().Clear();
	EnemyManager::Instance().Clear();
	ScoreManager::Instance().Clear();
	ObjectManager::Instance().Add(ObjectType::Player,startPoint);
	ObjectManager::Instance().Add(ObjectType::Enemy);

	UIManager::Instance().CreateUI(ScenePaturn::Game);
}

void GameScene::PreUpdate(float dt)
{
	ObjectManager::Instance().PreUpdate(dt);
}

void GameScene::Update(float dt)
{

	EnemyManager::Instance().AutoMake(dt);

	ObjectManager::Instance().Update(dt);
	UIManager::Instance().Update(dt);
	EnemyManager::Instance().EnemyRangeCheack();
	RenderManager::Instance().Update();
	if (INPUT.IsTriggered('Z'))SCENEMANAGER.RequestSceneChange(SceneType::Title);
	if (INPUT.IsTriggered('X'))SCENEMANAGER.RequestSceneChange(SceneType::Result);
}

void GameScene::RequestDraw()
{
	ObjectManager::Instance().DrawRequest();
	UIManager::Instance().DrawRequest();
}

void GameScene::onEnter()
{
}

void GameScene::onExit()
{
}
