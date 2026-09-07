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
	//“–‚½‚ç‚È‚¢ðŒ‚ð‚Í‚¶‚­‚±‚Æ‚ÅA“–‚½‚éðŒ‚ðo‚·
	return rectA.GetLeft() < rectB.GetRight() &&
		rectA.GetRight() > rectB.GetLeft() &&
		rectA.GetTop() < rectB.GetBottom() &&
		rectA.GetBottom() > rectB.GetTop();
}

