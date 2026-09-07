#pragma once
#include "Collider.h"
class CircleCollider :public Collider
{
public:
	CircleCollider(const Vector2& pos, float radius);

	/// <summary>
	/// ”¼Œa‚ÌŽæ“¾
	/// </summary>
	/// <returns>”¼Œa‚ð•Ô‚·</returns>
	float GetRadius() const { return radius_; }

private:
	float radius_;
};

