#include "EffectManager.h"
#include "Effects/BaseEffect.h"
#include "Effects/Effect1.h"
#include "Application/Object/ObjectManager.h"

void EffectManager::CreateEffect(EffectPaturn paturn, const Math::Vector2& pos, const Math::Vector2& size)
{
	std::unique_ptr<BaseEffect> effect = nullptr;
	switch (paturn)
	{
	case EffectPaturn::impact1:
		effect = std::make_unique<Effect1>();
		break;
	default:
		break;
	}
	effect->Init();
	effect->SetParameter(pos, size);
	ObjectManager::Instance().Add(std::move(effect));
}
