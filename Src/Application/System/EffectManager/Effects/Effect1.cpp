#include "Effect1.h"
#include "Application/System/ResourceManager/ResourceManager.h"

void Effect1::Update(float dt)
{
	animCount += dt * 20;

	objParameter.rectPosition.x = (int)animCount * animRect;

	if (animCount >= MaxCount)
	{
		Destroy();
		animCount = 0.0f;
	}
}

void Effect1::Init()
{
	objParameter.tex = RESOURCE.GetTexture("effect1");
	objParameter.size = { 64.0f,64.0f };
	objParameter.scale = { 1.0f,1.0f };
	objParameter.rectPosition = { 0.0f,64.0f * 8.0f };
	objParameter.color.w = 0.6f;
	animRect = 64;
	animCount = 0.0f;
	MaxCount = 7.0f;
}

//void Effect1::SetParameter(const Math::Vector2& pos, const Math::Vector2& size)
//{
//}
