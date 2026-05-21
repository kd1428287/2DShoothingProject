#include "main.h"
#include "Scene.h"
#include "Application/System/ResourceManager/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Application/System/AudioManager/AudioManager.h"

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
	srand(timeGetTime());

	SCENEMANAGER.RequestSceneChange(SceneType::Title);

	// 背景
	RESOURCE.LoadTexture("background", "Texture/System/Background/background.png");
	RESOURCE.LoadTexture("tree", "Texture/System/Background/tree.png");

	// オブジェクト
	RESOURCE.LoadTexture("player", "Texture/Object/Player/Player.png");
	RESOURCE.LoadTexture("dot", "Texture/System/dot.png");
	RESOURCE.LoadTexture("troll", "Texture/Object/Enemy/Goblin.png");
	RESOURCE.LoadTexture("troll1", "Texture/Object/Enemy/orc1_attack_full.png");
	RESOURCE.LoadTexture("troll2", "Texture/Object/Enemy/orc1_run_full.png");
	RESOURCE.LoadTexture("troll3", "Texture/Object/Enemy/knock.png");

	RESOURCE.LoadTexture("stone", "Texture/Object/Item/stone.png");
	RESOURCE.LoadTexture("bomb", "Texture/Object/Item/bomb.png");
	RESOURCE.LoadTexture("explosion", "Texture/Object/Item/explosion.png");
	RESOURCE.LoadTexture("effect1", "Texture/Object/Item/effect1.png");

	RESOURCE.LoadTexture("fence", "Texture/Object/fence.png");
	//RESOURCE.LoadTexture("wall", "Texture/System/Game/wall_transparent.png");

	RESOURCE.LoadTexture("shadow", "Texture/Object/shadow.png");
	
	// UI
	RESOURCE.LoadTexture("logo", "Texture/System/Title/title.png");
	RESOURCE.LoadTexture("titleUI", "Texture/System/Title/ClickToStart.png");
	RESOURCE.LoadTexture("resultUI", "Texture/System/Title/resultText.png");
	RESOURCE.LoadTexture("box", "Texture/System/box.png");
	RESOURCE.LoadTexture("box_Hi", "Texture/System/box_.png");
	RESOURCE.LoadTexture("torch", "Texture/System/Title/torch.png");

	RESOURCE.LoadTexture("number", "Texture/System/Game/number.png");
	RESOURCE.LoadTexture("mouse", "Texture/System/Game/mouse.png");
	RESOURCE.LoadTexture("wall", "Texture/System/Game/wooden.png");
	RESOURCE.LoadTexture("L", "Texture/System/Game/tIcom.png");
	RESOURCE.LoadTexture("R", "Texture/System/Game/cIcom.png");
	RESOURCE.LoadTexture("Village", "Texture/System/Game/Village1.png");
	RESOURCE.LoadTexture("HP", "Texture/System/Game/HP.png");

	AUDIOM.Init();

	
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
