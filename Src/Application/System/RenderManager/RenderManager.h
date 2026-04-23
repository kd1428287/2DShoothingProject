#pragma once

#include"RendData.h";

class RenderManager
{
public:

	static RenderManager& Instance()
	{
		static RenderManager instance;
		return instance;
	}

	void DrawSprite();
	
	void Submit(RendData queue);

	void Fadeout(std::function<void()> onComplete);
	void Fadein(std::function<void()> onComplete);


private:
	RenderManager() {}

	void BackgroundDraw();

	void DrawQueue(std::vector<RendData>queue_);

	std::vector<RendData> backQueue;
	std::vector<RendData> middleQueue;
	std::vector<RendData> frontQueue;
	std::vector<RendData> UIQueue;

	float fadeAlpha = 0.0f;

};
#define RENDERM RenderManager::Instance()