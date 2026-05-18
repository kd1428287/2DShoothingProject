#pragma once

enum class EffectPaturn
{
	impact1,

};
class EffectManager
{
public:
	static EffectManager& Instance()
	{
		static EffectManager instance;
		return instance;
	}

	void CreateEffect(EffectPaturn paturn, const Math::Vector2& pos, const Math::Vector2& size);

private:
	EffectManager() {};
	~EffectManager() {};


};