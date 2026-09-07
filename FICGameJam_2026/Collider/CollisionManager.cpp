#include "CollisionManager.h"
#include "CircleCollider.h"
#include "RectCollider.h"
#include<algorithm>
#include<cmath>

bool CollisionManager::IsHitCircleAndCircle(const CircleCollider& c1, const CircleCollider& c2) const
{
	float dx = c1.GetPos().x - c2.GetPos().x;
	float dy = c1.GetPos().y - c2.GetPos().y;
	float distSq = dx * dx + dy * dy;
	float radiusSum = c1.GetRadius() + c2.GetRadius();

	return distSq <= radiusSum * radiusSum;
}

bool CollisionManager::IsHitCollisionRect(const RectCollider& rectA, const RectCollider& rectB) const
{
	//当たらない条件をはじくことで、当たる条件を出す
	return rectA.GetLeft() < rectB.GetRight() &&
		rectA.GetRight() > rectB.GetLeft() &&
		rectA.GetTop() < rectB.GetBottom() &&
		rectA.GetBottom() > rectB.GetTop();
}

Vector2 CollisionManager::GetOverlapRect(const RectCollider& rectA, const RectCollider& rectB) const
{
    //重なっている領域の幅と高さを計算
    float overlapX = (rectA.GetWidth() + rectB.GetWidth()) / 2.0f - std::abs(rectA.GetPos().x - rectB.GetPos().x);
    float overlapY = (rectA.GetHeight() + rectB.GetHeight()) / 2.0f - std::abs(rectA.GetPos().y - rectB.GetPos().y);

    //重なっていない場合はゼロベクトルを返す
    if (overlapX <= 0.0f || overlapY <= 0.0f)
    {
        return Vector2(0.0f, 0.0f);
    }

    //めり込み量が少ない方の軸で押し戻す
    if (overlapX < overlapY)
    {
        float sign = (rectA.GetPos().x < rectB.GetPos().x) ? -1.0f : 1.0f;
        return Vector2(overlapX * sign, 0.0f);
    }
    else
    {
        float sign = (rectA.GetPos().y < rectB.GetPos().y) ? -1.0f : 1.0f;
        return Vector2(0.0f, overlapY * sign);
    }
}

