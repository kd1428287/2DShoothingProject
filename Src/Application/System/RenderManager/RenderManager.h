#pragma once

#include"ObjectData.h";

class RenderManager
{
public:

	static RenderManager& Instance()
	{
		static RenderManager instance;
		return instance;
	}

	void DrawSprite();
	
	void Submit(ObjectData queue);
	void Submit(std::shared_ptr<ObjectData> queue);

	void Fadeout(std::function<void()> onComplete);
	void Fadein(std::function<void()> onComplete);

	void SetZoomRatio(float ratio) { tmpScale = ratio; };


private:
	RenderManager();
	~RenderManager() {}

	void BackgroundDraw();

	void DrawQueue(std::vector<ObjectData>& queue_);

	KdTexture tmpTex;
	float tmpScale{};
	float scrWidth = 1280;
	float scrHeight = 720;

	std::vector<ObjectData> backQueue;
	std::vector<ObjectData> middleQueue;
	std::vector<ObjectData> frontQueue;
	std::vector<ObjectData> UIQueue;

	float fadeAlpha = 1.0f;

};
#define RENDERM RenderManager::Instance()