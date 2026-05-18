#pragma once

#include "ObjectData.h"
#include <functional>
#include <vector>
#include <memory>

class RenderManager
{
public:

	static RenderManager& Instance()
	{
		static RenderManager instance;
		return instance;
	}

	void Update(); // 毎フレームの減衰更新用に追加
	void DrawSprite();

	void Submit(ObjectData queue);
	void Submit(std::shared_ptr<ObjectData> queue);

	void Fadeout(std::function<void()> onComplete);
	void Fadein(std::function<void()> onComplete);

	void SetZoomRatio(float ratio) { tmpScale = ratio; };

	// ダメージリアクション開始処理を追加
	// shakeIntensity: 揺れの強さ, redFlashAlpha: 赤みの濃さ(0.0f ～ 1.0f)
	void StartDamageReaction(float shakeIntensity = 15.0f, float redFlashAlpha = 0.5f);

private:
	RenderManager();
	~RenderManager() {}

	void BackgroundDraw();
	void TreeGeneration();

	void DrawQueue(std::vector<ObjectData>& queue_);

	KdTexture* backTex;
	KdTexture* treeTex;

	KdTexture tmpTex;
	float tmpScale{};
	float scrWidth = 1280;
	float scrHeight = 720;

	std::vector<ObjectData> backQueue;
	std::vector<ObjectData> middleQueue;
	std::vector<ObjectData> frontQueue;
	std::vector<ObjectData> UIQueue;

	Math::Color fadeColor{};
	float fadeAlpha = 1.0f;

	// ダメージリアクション用変数
	float m_shakeIntensity = 0.0f;
	Math::Vector2 m_shakeOffset{};
	float m_damageRedAlpha = 0.0f;

};
#define RENDERM RenderManager::Instance()