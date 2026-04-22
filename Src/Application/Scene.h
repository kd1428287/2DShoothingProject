#pragma once

class Scene
{
private:

	using clock = std::chrono::steady_clock;
	clock::time_point prevTime_ = clock::now();
	float deltaTime_ = 0.0f;

	float timeStopCount = 0.0f;

	float elapsedTime = 0.0f;

public:

	// 初期設定
	void Init();

	// 解放
	void Release();

	// 更新処理
	void Update();

	// 描画処理
	void Draw2D();

	// GUI処理
	void ImGuiUpdate();

	// 更新停止カウント設定
	void SetStopCount(float count) { timeStopCount = count; };

private:

	Scene() {}

public:
	static Scene& GetInstance()
	{
		static Scene instance;
		return instance;
	}
};

#define SCENE Scene::GetInstance()
