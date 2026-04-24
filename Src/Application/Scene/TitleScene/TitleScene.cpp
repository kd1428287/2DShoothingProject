#include "TitleScene.h"
#include "Application/System/RenderManager/ObjectData.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Application/Scene/SceneManager.h"
#include "Application/System/InputManager/InputManager.h"

void TitleScene::Init()
{
}

void TitleScene::PreUpdate(float dt)
{
}

void TitleScene::Update(float dt)
{
	if(INPUT.IsTriggered('Z'))SCENEMANAGER.RequestSceneChange(SceneType::Game);
}

void TitleScene::RequestDraw()
{
	ObjectData data;
	data.tex = RESOURCE.GetTexture("player");
	data.size = { 64,64 };
	data.target = DrawTarget::middle;
	data.flashValue = 1.0f;
	RENDERM.Submit(data);
}

void TitleScene::onEnter()
{
}

void TitleScene::onExit()
{
}
