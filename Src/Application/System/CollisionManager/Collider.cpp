#include "Collider.h"
#include "Application/Object/BaseObject.h"

Collider::Collider(BaseObject* owner, CollisionLayer layer)
    : m_owner(owner)
    , m_layer(layer)
{
}

Collider::~Collider() = default;

void Collider::SetPosition(const Math::Vector2& pos)
{
    m_position = pos;
}

void Collider::SetCircle(float radius)
{
    m_shapeType = ShapeType::Circle;
    m_radius = radius;
}

void Collider::SetAABB(const Math::Vector2& halfSize)
{
    m_shapeType = ShapeType::AABB;
    m_halfSize = halfSize;
}

Math::Vector2 Collider::GetSize()
{
    if (m_shapeType == ShapeType::Circle) {
        return { m_radius,m_radius };
    }
    else if (m_shapeType == ShapeType::AABB) {
        return m_halfSize;
    }
    else {
        return { NULL,NULL };
    }
}

bool Collider::Intersects(const Collider& other) const
{
    if (m_shapeType == ShapeType::Circle &&
        other.m_shapeType == ShapeType::Circle)
    {
        return IntersectsCircleCircle(other);
    }

    if (m_shapeType == ShapeType::AABB &&
        other.m_shapeType == ShapeType::AABB)
    {
        return IntersectsAABBAABB(other);
    }

    if (m_shapeType == ShapeType::Circle &&
        other.m_shapeType == ShapeType::AABB)
    {
        return IntersectsCircleAABB(*this, other);
    }

    if (m_shapeType == ShapeType::AABB &&
        other.m_shapeType == ShapeType::Circle)
    {
        return IntersectsCircleAABB(other, *this);
    }

    return false;
}

//円判定どうしの当たり判定
bool Collider::IntersectsCircleCircle(const Collider& other) const
{
    float dx = m_position.x - other.m_position.x;   //Aの位置座標 - Bの位置座標
    float dy = m_position.y - other.m_position.y;

    float r = m_radius + other.m_radius;            //Aの半径     + Bの半径　
    return (dx * dx + dy * dy) <= (r * r);          //三平方の定理を利用し、dx + dy が r 以下なら true を返す
}

//矩形判定どうしの当たり判定
bool Collider::IntersectsAABBAABB(const Collider& other) const
{
    if (std::fabs(m_position.x - other.m_position.x) >      //Aの位置座標 - Bの位置座標(fabsは引数の小数点対応の絶対値を返す)   より
        (m_halfSize.x + other.m_halfSize.x)) return false;  //Aのx方向のハーフサイズ + Bのx方向のハーフサイズ                   の方が小さいなら
                                                            //(つまりx軸上で重なっているなら処理続行)
    if (std::fabs(m_position.y - other.m_position.y) >      //Aの位置座標 - Bの位置座標(fabsは引数の小数点対応の絶対値を返す)   より
        (m_halfSize.y + other.m_halfSize.y)) return false;  //Aのy方向のハーフサイズ + Bのy方向のハーフサイズ                   の方が小さいなら
                                                            //(つまりy軸上で重なっているなら処理続行)
    return true;                                            //x軸上でもy軸上でも重なっている = 判定が接しているから　　　　　　 trueを返す
}

//円判定と矩形判定の当たり判定
bool Collider::IntersectsCircleAABB(const Collider& circle,
    const Collider& aabb) const
{   //maxは引数2つを比べ大きい方を返す  minは引数2つを比べ小さいほうを返す
    float cx = std::max(
        aabb.m_position.x - aabb.m_halfSize.x,          //矩形判定の左側の位置座標
        std::min(circle.m_position.x,                   //円判定の中心座標
            aabb.m_position.x + aabb.m_halfSize.x));    //矩形判定の右側の位置座標
                                                        //矩形判定のx方向の当たり判定のなかで、円判定の中心座標により近い座標
                                                        //(矩形判定の中にいるなら円判定の中心座標そのものを)を返す
    float cy = std::max(
        aabb.m_position.y - aabb.m_halfSize.y,          //矩形判定の下側の位置座標
        std::min(circle.m_position.y,                   //円判定の中心座標
            aabb.m_position.y + aabb.m_halfSize.y));    //矩形判定の上側の位置座標
                                                        //矩形判定のy方向の当たり判定のなかで、円判定の中心座標により近い座標
                                                        //(矩形判定の中にいるなら円判定の中心座標そのものを)を返す

    float dx = circle.m_position.x - cx;                //円判定の中心座標から矩形判定内の最も近い座標を引いた位置
    float dy = circle.m_position.y - cy;                //つまり矩形判定内にある、最も円座標に近い一点であり、
                                                        //これを三平方の定理を利用した場合求められるものが円判定の中心座標から矩形判定までの距離

    //円判定の中心座標から矩形判定までの距離が円の半径以下なら、つまり接しているならtrueを返す
    return (dx * dx + dy * dy) <= (circle.m_radius * circle.m_radius);  
}
