#pragma once
#include "Application/Object/BaseObject.h"

class ItemSlotUI : public BaseObject
{
public:
	ItemSlotUI() {}
	~ItemSlotUI() override {}

	

	void Init() override;
	void Update(float dt)override;
	void DrawRequest() override;
	void UIActivate() { displayCount = 1.0f; };

private:
	float displayCount = 0.0f;
	KdTexture* boxTex	= nullptr;
	KdTexture* boxHiTex = nullptr;
	KdTexture* stoneTex = nullptr;
	KdTexture* bombTex	= nullptr;
	KdTexture* MouseTex	= nullptr;
	KdTexture* RTex		= nullptr;
	KdTexture* LTex		= nullptr;
	KdTexture* wallTex	= nullptr;
	KdTexture* viTex	= nullptr;
	KdTexture* hpTex	= nullptr;
	float hpRec = 0.0f;
	
};