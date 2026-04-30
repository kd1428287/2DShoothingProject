#include "main.h"
#include "Scene.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Scene/SceneManager.h"

void Scene::Draw2D()
{
	RENDERM.DrawSprite();
}

void Scene::Update()
{
	//deltaTime入手 -
	auto now = clock::now();
	auto deltaTime_ =
		std::chrono::duration<float>(now - prevTime_).count();
	prevTime_ = now;

	if (deltaTime_ > 0.1f)
		deltaTime_ = 0.1f;

	//更新停止カウント
	if (timeStopCount > 0.0f) 
	{
		if (timeStopCount > deltaTime_)
		{
			timeStopCount -= deltaTime_;
			deltaTime_ = 0.0f;
		}
		else
		{
			deltaTime_ -= timeStopCount;
			timeStopCount = 0.0f;
		}
	}

	//経過時間保存
	elapsedTime += deltaTime_;

	// -

	SCENEMANAGER.PreUpdate(deltaTime_);
	SCENEMANAGER.Update(deltaTime_);
	SCENEMANAGER.RequestDraw();
}

void Scene::Init()
{
	SCENEMANAGER.RequestSceneChange(SceneType::Title);

	RESOURCE.LoadTexture("player", "Texture/player.png");
	RESOURCE.LoadTexture("dot", "Texture/System/dot.png");

	
}

void Scene::Release()
{

}

void Scene::ImGuiUpdate()
{
	return;

	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Once);

	// デバッグウィンドウ
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Text("FPS : %d", APP.m_fps);
	}
	ImGui::End();
}
