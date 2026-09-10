#include "Chest.h"
#include "../SoundManager.h"
#include<Dxlib.h>

namespace
{
	//宝箱画像の1コマサイズ
	const Vector2 kChestFrameSize = { 48.0f, 48.0f };

	//アニメーションのコマ数
	constexpr int kChestAnimMaxNum = 5;

	//1コマあたりのフレーム数
	constexpr int kChestOneAnimFrame = 6;

	//拡大率
	constexpr float kChestScale = 2.0f;

	//上段の行番号
	constexpr int kChestAnimIndex = 0;
}

Chest::Chest(const Vector2& pos, ChestContents contents):
	GameObject(pos, Vector2{0.0f,0.0f},0.0f,24.0f,24.0f),
	content_(contents)
{
}

Chest::~Chest()
{
	DeleteGraph(handle_);
}

void Chest::Init()
{
	handle_ = LoadGraph(L"data/Chest.png");

	anim_.Init(handle_, kChestAnimIndex, kChestFrameSize,
		kChestAnimMaxNum, kChestOneAnimFrame, kChestScale, false);
	anim_.SetFirst();
}

void Chest::Update()
{
	if (isOpened_ && !anim_.IsLastFrame())
	{
		anim_.Update();
	}
}

void Chest::Draw()
{
	anim_.Draw(pos_, false);

#ifdef _DEBUG
	if (!isOpened_)
	{
		float halfW = collider_.GetWidth() / 2.0f;
		float halfH = collider_.GetHeight() / 2.0f;
		DrawBox(pos_.x - halfW, pos_.y - halfH, pos_.x + halfW, pos_.y + halfH, 0xff0000, false);
	}
#endif
}

void Chest::Open()
{
	if (isOpened_)
	{
		return;
	}

	isOpened_ = true;
	isOpenEvent_ = false;
}

bool Chest::IsOpenEvent()
{
	if (isOpened_ && !isOpenEvent_)
	{
		isOpenEvent_ = true;
		SoundManager::GetInstance().PlaySe(SE::OpenTreasure);
		return true;
	}
	return false;
}
