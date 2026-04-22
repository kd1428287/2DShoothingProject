#include "main.h"
#include "Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene/TitleScene.h"

void Scene::Draw2D()
{
	
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
	// -

	SCENEMANAGER.PreUpdate(deltaTime_);
	SCENEMANAGER.Update(deltaTime_);
	SCENEMANAGER.RequestDraw();
}

void Scene::Init()
{
	SCENEMANAGER.SetScene(std::make_unique<TitleScene>());
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
