#pragma once

enum class DrawTarget {
	back = 0,
	middle,
	front,
	UI
};

struct ObjectData
{
	KdTexture* tex{};
	Math::Vector2 position{};
	Math::Vector2 size{};
	Math::Vector2 scale = Math::Vector2(1,1);
	Math::Vector2 rectPosition{};
	float angle{};
	float priority{};
	float footPosition{};
	Math::Matrix mat{};
	Math::Color color = { 1.0f,1.0f,1.0f,1.0f };
	float flashValue{};

	DrawTarget target = DrawTarget::middle;
	bool addEffect{};
};