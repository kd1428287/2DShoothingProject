#include "GameScene.h"
#include "Application/System/RenderManager/ObjectData.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/Scene/SceneManager.h"
#include "Application/System/InputManager/InputManager.h"


void GameScene::Init()
{
}

void GameScene::PreUpdate(float dt)
{
}

void GameScene::Update(float dt)
{
	if (INPUT.IsTriggered('Z'))SCENEMANAGER.RequestSceneChange(SceneType::Title);
}

void GameScene::RequestDraw()
{
	ObjectData data;
	data.tex = RESOURCE.GetTexture("dot");
	data.size = { 1,1 };
	data.scale = { 64,64 };
	data.position = { 300,0 };
	data.color = { 1.0f,1.0f,1.0f,1.0f };
	data.target = DrawTarget::front;
	RENDERM.Submit(data);
}

void GameScene::onEnter()
{
}

void GameScene::onExit()
{
}
