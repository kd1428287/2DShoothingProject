#pragma once

#include "Application/Object/BaseObject.h"

class BaseEffect : public BaseObject
{
public:
	BaseEffect() {};
	~BaseEffect() {};

	virtual void SetParameter(const Math::Vector2& pos, const Math::Vector2& size);

protected:
	float animCount{};
	float MaxCount{};
	int animRect{};
};