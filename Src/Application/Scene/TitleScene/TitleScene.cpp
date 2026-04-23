#include "TitleScene.h"
#include "Application/System/RenderManager/RendData.h"
#include "Application/System/RenderManager/RenderManager.h"
#include "Application/System/ResourceManager/ResourceManager.h"

void TitleScene::Init()
{
}

void TitleScene::PreUpdate(float dt)
{
}

void TitleScene::Update(float dt)
{

}

void TitleScene::RequestDraw()
{
	RendData data;
	data.tex = RESOURCE.GetTexture("player");
	data.size = { 64,64 };
	data.target = DrawTarget::middle;
	RENDERM.Submit(data);
}

void TitleScene::onEnter(float dt)
{
}

void TitleScene::onExit(float dt)
{
}
