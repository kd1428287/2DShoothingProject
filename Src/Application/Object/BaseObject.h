#pragma once

class Collider;
struct ObjectData;
struct HitResult;
class ObjectManager;
class RenderManager;

class BaseObject
{
public:
	BaseObject();
	virtual ~BaseObject();

	virtual void PreUpdate(float dt);
	virtual void Update(float dt);
	virtual void DrawRequest();

	// ===== Collision =====
	void AddCollider(std::unique_ptr<Collider> collider);
	const std::vector<std::unique_ptr<Collider>>& GetColliders() const;
	virtual void OnCollision(Collider* self, const HitResult& hit) {}

protected:

	std::shared_ptr<ObjectData> data;
	// 複数コライダー対応（本体＋盾など）
	std::vector<std::unique_ptr<Collider>> colliders_;
	ObjectManager* ownerManager = nullptr;

private:

};