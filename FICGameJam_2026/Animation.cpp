#include "Animation.h"
#include<Dxlib.h>

Animation::Animation():
	isLoop_(true),
	drawHandle_(-1),
	animIndex_(0),
	frameSize_({ 0.0f,0.0f }),
	maxAnimNum_(0),
	nowAnimNum_(0),
	oneAnimFrame_(0),
	frameCount_(0),
	scale_(1.0f),
	rotate_(0.0f),
	drawOffset_({ 0.0f,0.0f })
{
}

Animation::~Animation()
{
}

void Animation::Init(int drawHandle, int animIndex, Vector2 frameSize, int maxAnimNum, int oneAnimFrame, float scale,bool isRepeat)
{
	isLoop_ = isRepeat;
	drawHandle_ = drawHandle;
	animIndex_ = animIndex;
	frameSize_ = frameSize;
	maxAnimNum_ = maxAnimNum;
	oneAnimFrame_ = oneAnimFrame;
	scale_ = scale;
	nowAnimNum_ = 0;
	frameCount_ = 0;
}

void Animation::Init(int drawHandle, int animIndexY, int animIndexX, Vector2 frameSize, float scale, bool isRepeat)
{
	isLoop_ = isRepeat;
	drawHandle_ = drawHandle;
	animIndex_ = animIndexY;
	frameSize_ = frameSize;
	maxAnimNum_ = animIndexX + 1;
	oneAnimFrame_ = 0;
	scale_ = scale;
	nowAnimNum_ = animIndexX;
	frameCount_ = 0;
}

void Animation::Update()
{
	if (oneAnimFrame_ == 0) return;	//1コマあたりのフレーム数が0なら処理を抜ける

	frameCount_++;	//フレームカウントを進める
	
	if (frameCount_ >= oneAnimFrame_)	//1コマ分のフレームが経過したら
	{
		frameCount_ = 0;
		nowAnimNum_++;	//アニメーション番号を進める

		if (nowAnimNum_ >= maxAnimNum_)
		{	
			//最大コマ数を超えたら最初に戻す
			if (isLoop_)
			{	
				//繰り返し再生するなら最初のコマに戻す
				nowAnimNum_ = 0;
			}
			
		}
	}
}

void Animation::Draw(Vector2 pos, bool isTurn)
{
	//現在のコマ数が最大コマ数なら描画しない
	if (nowAnimNum_ != maxAnimNum_)
	{
		DrawRectRotaGraph(static_cast<int>(pos.x + drawOffset_.x), static_cast<int>(pos.y + drawOffset_.y),
			static_cast<int>(frameSize_.x) * nowAnimNum_, animIndex_ * static_cast<int>(frameSize_.y),
			static_cast<int>(frameSize_.x), static_cast<int>(frameSize_.y),
			scale_, rotate_, drawHandle_, true, isTurn);
	}
}

void Animation::Draw(int drawHandle, Vector2 pos, bool isTurn)
{
	if (nowAnimNum_ != maxAnimNum_)
	{
		DrawRectRotaGraph(static_cast<int>(pos.x), static_cast<int>(pos.y),
			static_cast<int>(frameSize_.x) * nowAnimNum_, animIndex_ * static_cast<int>(frameSize_.y),
			static_cast<int>(frameSize_.x), static_cast<int>(frameSize_.y),
			scale_, rotate_, drawHandle, true, isTurn);
	}
}

bool Animation::GetIsEnd()
{
	if (isLoop_) return false;										//繰り返し再生するなら終了しない

	int totalFrames = oneAnimFrame_ * maxAnimNum_;					//アニメーション全体のフレーム数
	int currentFrame = nowAnimNum_ * oneAnimFrame_ + frameCount_;	//現在のフレーム数
	if (currentFrame >= totalFrames) return true;

	return false;
}

bool Animation::operator!=(const Animation& other) const
{
	if (drawHandle_ != other.drawHandle_) return true;
	if (animIndex_ != other.animIndex_) return true;
	if (rotate_ != other.rotate_) return true;
	if (oneAnimFrame_ != other.oneAnimFrame_) return true;
	return false;
}
