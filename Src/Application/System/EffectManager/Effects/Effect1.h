#pragma once

#include "BaseEffect.h"

class Effect1 : public BaseEffect
{
public:
	Effect1() {};
	~Effect1() {};

	void Update(float dt)override;
	void Init()override;
	//void SetParameter(const Math::Vector2& pos, const Math::Vector2& size)override;

private:

};