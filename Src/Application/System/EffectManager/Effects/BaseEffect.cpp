#include "BaseEffect.h"

void BaseEffect::SetParameter(const Math::Vector2& pos, const Math::Vector2& size)
{
	objParameter.position = pos;
	objParameter.scale = size / objParameter.size;
}
