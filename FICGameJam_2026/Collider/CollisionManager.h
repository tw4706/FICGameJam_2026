#pragma once

class CircleCollider;
class RectCollider;
class CollisionManager
{
public:

	/// <summary>
	/// ‰~‚Æ‰‚Ì“–‚½‚è”»’è
	/// </summary>
	/// <param name="c1">‰~‚Ì“–‚½‚è”»’è1</param>
	/// <param name="c2">‰~‚Ì“–‚½‚è”»’è2</param>
	/// <returns>“–‚½‚Á‚Ä‚¢‚½‚çtrue,“–‚½‚Á‚Ä‚¢‚È‚©‚Á‚½‚çfalse‚ğ•Ô‚·</returns>
	bool IsHitCircleAndCircle(const CircleCollider& c1, const CircleCollider& c2) const;
	bool IsHitCollisionRect(const RectCollider& rectA, const RectCollider& rectB)const;
};
