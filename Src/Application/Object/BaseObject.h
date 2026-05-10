#pragma once
#include "Application/System/RenderManager/ObjectData.h"

class Collider;
struct HitResult;
class ObjectManager;
class RenderManager;

struct ObjectParameter
{
	KdTexture* tex{};
	Math::Vector2 position{};
	Math::Vector2 velocity{};
	Math::Vector2 size{};
	Math::Vector2 scale = Math::Vector2(1, 1);
	Math::Vector2 rectPosition{};
	float angle{};
	float priority{};
	float footPosition = -9999.9f;
	Math::Matrix mat{};
	Math::Color color = { 1.0f,1.0f,1.0f,1.0f };
	float flashValue{};

	DrawTarget target = DrawTarget::middle;
	bool addEffect{};
	bool isDead = false;
};

class BaseObject
{
public:
	BaseObject();
	virtual ~BaseObject();

	virtual void Init();
	virtual void PreUpdate(float dt);
	virtual void Update(float dt);
	virtual void DrawRequest();

	void PositionSync();

	// ===== Collision =====
	void AddCollider(std::unique_ptr<Collider> collider);
	const std::vector<std::unique_ptr<Collider>>& GetColliders() const;
	virtual void OnCollision(Collider* self, const HitResult& hit) {}

	//セッター
	void SetPosition(Math::Vector2 position) { objParameter.position = position; };
	void Destroy() { objParameter.isDead = true; }

	bool IsDead() { return objParameter.isDead; }
	Math::Vector2 GetPosition() { return objParameter.position; }

protected:

	void DrawShadow(const Math::Vector2& groundPos, float shadowHeightOffset = 0.0f);

	ObjectParameter objParameter;
	// 複数コライダー対応（本体＋盾など）
	std::vector<std::unique_ptr<Collider>> colliders_;
	ObjectManager* ownerManager = nullptr;

	bool isShadow = true;

private:

};