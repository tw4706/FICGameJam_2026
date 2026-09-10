#include "Goal.h"
#include<Dxlib.h>

namespace
{
	constexpr float kDrawScale = 2.0f;
}

Goal::Goal(Vector2 pos, Vector2 vel, float dir, int width, int height):
	GameObject(pos,vel,dir,width,height),
	handle_(-1)
{
}

Goal::~Goal()
{
}

void Goal::Init()
{
	handle_ = LoadGraph(L"data/Door.png");
}

void Goal::Update()
{
	//コライダーの座標の更新
	collider_.SetPos(pos_);
}

void Goal::Draw()
{
	DrawRotaGraph(
		static_cast<int>(pos_.x),
		static_cast<int>(pos_.y),
		kDrawScale,
		0.0,
		handle_,
		true);

#ifdef _DEBUG
	//デバッグ用表示
	float halfW = collider_.GetWidth() / 2.0f;
	float halfH = collider_.GetHeight() / 2.0f;
	DrawBox(pos_.x - halfW, pos_.y - halfH, pos_.x + halfW, pos_.y + halfH, 0xff0000, false);
#endif
}
