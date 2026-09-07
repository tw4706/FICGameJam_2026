#pragma once
#include "Collider.h"
class RectCollider :public Collider
{
public:
	RectCollider(const Vector2& pos, float width, float height);

	float GetLeft()   const;
	float GetRight()  const;
	float GetTop()    const;
	float GetBottom() const;

	float GetWidth()  const { return width_; }
	float GetHeight() const { return height_; }

private:
	float width_;	//â°ïù
	float height_;	//çÇÇ≥
};

