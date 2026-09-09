#include "Key.h"
#include<Dxlib.h>

namespace
{
	//画像の枚数
	constexpr int kAnimNum = 4;

	//1コマの表示フレーム数
	constexpr int kAnimFrame = 10;

	//拡大率
	constexpr float kScale = 3.0f;

}

Key::Key(Vector2 pos, Vector2 vel, float dir, float width, float height) :
	GameObject(pos,vel,dir,width,height)
{
}

Key::~Key()
{
	for (int i = 0; i < kAnimNum; i++)
	{
		DeleteGraph(handles_[i]);
	}
}

void Key::Init()
{
	handles_[0] = LoadGraph(L"data/keys_1_1.png");
	handles_[1] = LoadGraph(L"data/keys_1_2.png");
	handles_[2] = LoadGraph(L"data/keys_1_3.png");
	handles_[3] = LoadGraph(L"data/keys_1_4.png");
}

void Key::Update()
{
	frameCount_++;
	if (frameCount_ >= kAnimFrame)
	{
		frameCount_ = 0;
		animIndex_ = (animIndex_ + 1) % kAnimNum;
	}

	collider_.SetPos(pos_);
}

void Key::Draw()
{
	DrawRotaGraph(static_cast<int>(pos_.x), static_cast<int>(pos_.y),
		kScale, 0.0f, handles_[animIndex_], true);

#ifdef _DEBUG
	//デバッグ用表示
	float halfW = collider_.GetWidth() / 2.0f;
	float halfH = collider_.GetHeight() / 2.0f;
	DrawBox(pos_.x - halfW, pos_.y - halfH, pos_.x + halfW, pos_.y + halfH, 0xff0000, false);
#endif
}

void Key::OnCollision(GameObject& other)
{
	Destroy();
}
