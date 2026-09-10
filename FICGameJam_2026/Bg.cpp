#include "Bg.h"

#include "Game.h"
#include <Dxlib.h>
#include <cmath>

Bg::Bg(const std::wstring& filePath, float scrollSpeedX, float scrollSpeedY) :
	filePath_(filePath),
	scrollSpeedX_(scrollSpeedX),
	scrollSpeedY_(scrollSpeedY)
{
}

Bg::~Bg()
{
	DeleteGraph(handle_);
}

void Bg::Init()
{
	handle_ = LoadGraph(filePath_.c_str());
	GetGraphSize(handle_, &width_, &height_);
}

void Bg::Update()
{
	scrollX_ += scrollSpeedX_;
	scrollY_ += scrollSpeedY_;

	//画像サイズでループさせる
	scrollX_ = fmodf(scrollX_, static_cast<float>(width_));
	scrollY_ = fmodf(scrollY_, static_cast<float>(height_));
}

void Bg::Draw() const
{
	//スクロール分だけ左上の描画開始位置をずらす
	int offsetX = -static_cast<int>(scrollX_);
	int offsetY = -static_cast<int>(scrollY_);

	//画面全体を覆うように必要な枚数を計算
	int tilesX = Game::kScreenWidth / width_ + 2;
	int tilesY = Game::kScreenHeight / height_ + 2;

	for (int y = 0; y < tilesY; y++)
	{
		for (int x = 0; x < tilesX; x++)
		{
			int drawX = offsetX + x * width_;
			int drawY = offsetY + y * height_;
			DrawGraph(drawX, drawY, handle_, FALSE);
		}
	}
}