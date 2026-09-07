#include "RectCollider.h"
#include "../Vector2.h"

RectCollider::RectCollider(const Vector2& pos, float width, float height):
	Collider(pos),
	width_(width),
	height_(height)
{
}

//左座標を取得する
float RectCollider::GetLeft() const
{
	return GetPos().x - width_ / 2.0f;
}

//右座標を取得する
float RectCollider::GetRight() const
{
	return GetPos().x + width_ / 2.0f;
}

//上座標を取得する
float RectCollider::GetTop() const
{
	return GetPos().y - height_ / 2.0f;
}

//下座標を取得する
float RectCollider::GetBottom() const
{
	return GetPos().y + height_ / 2.0f;
}
