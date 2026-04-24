#pragma once

class Collider;
struct HitResult;


class BaseObject
{
public:
	BaseObject() {};
	virtual ~BaseObject() {};

	virtual void PreUpdate(float dt);
	virtual void Update(float dt);

	// ===== Collision =====
	void AddCollider(std::unique_ptr<Collider> collider);
	const std::vector<std::unique_ptr<Collider>>& GetColliders() const;
	virtual void OnCollision(Collider* self, const HitResult& hit) {}

protected:

private:

};