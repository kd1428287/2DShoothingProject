#pragma once

#include"RendData.h";

class RenderManager
{
public:

	static RenderManager& Instance();

	void DrawSprite();
	
	void Submit(RendData queue);

	void BlackoutRatio(float ratio);	// 0 ~ 1


private:
	RenderManager() {}

	void BackgroundDraw();

	void DrawQueue(std::vector<RendData>queue_);

	std::vector<RendData> backQueue;
	std::vector<RendData> middleQueue;
	std::vector<RendData> frontQueue;
	std::vector<RendData> UIQueue;



};
#define RENDERM RenderManager::Instance()