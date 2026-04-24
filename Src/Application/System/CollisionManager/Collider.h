#pragma once
#include "CollisionLayer.h"

class BaseObject;

//--------------------------------------------
// Collider
//--------------------------------------------
class Collider
{
public:
    enum class ShapeType
    {
        //判定タイプ選択
        Circle, //円判定
        AABB,   //矩形判定
    };

public:
    Collider(BaseObject* owner, CollisionLayer layer);
    ~Collider();

    // ===== 基本 =====
    void SetPosition(const Math::Vector2& pos);         //当たり判定位置更新

    void SetEnable(bool enable) { m_enable = enable; }  //当たり判定実施フラグ(非削除)
    bool IsEnable() const { return m_enable; }

    void SetLayer(CollisionLayer layer) { m_layer = layer; }    //当たり判定レイヤー登録
    CollisionLayer GetLayer() const { return m_layer; }

    BaseObject* GetOwner() const { return m_owner; }
    Math::Vector2 GetPos() const { return m_position; }

    void SetOffset(const Math::Vector2& offset) { offset_ = offset; }
    const Math::Vector2& GetOffset() const { return offset_; }

    // ===== 形状 =====
    void SetCircle(float radius);
    void SetAABB(const Math::Vector2& halfSize);

    Math::Vector2 GetSize();

    ShapeType GetShapeType() const { return m_shapeType; }

    // ===== パラメータ =====
    void SetDamage(int damage) { m_damage = damage; }
    int GetDamage()const {return m_damage; }
    void SetKnockback(float kbPow) { m_kbPow = kbPow; }
    float GetKnockback() { return m_kbPow; }

    // ===== 判定 =====
    bool Intersects(const Collider& other) const;

private:
    bool IntersectsCircleCircle(const Collider& other) const;
    bool IntersectsAABBAABB(const Collider& other) const;
    bool IntersectsCircleAABB(const Collider& circle,
        const Collider& aabb) const;

private:
    BaseObject* m_owner = nullptr;
    CollisionLayer m_layer = CollisionLayer::None;

    bool m_enable = true;

    Math::Vector2 m_position{ 0.0f, 0.0f };
    Math::Vector2 offset_ = { 0.0f, 0.0f };

    ShapeType m_shapeType = ShapeType::Circle;

    int m_damage{};
    float m_kbPow = 200.0f;

    // Circle
    float m_radius = 0.0f;

    // AABB
    Math::Vector2 m_halfSize{ 0.0f, 0.0f };
};
