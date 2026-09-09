#include "Effect.h"

Effect::Effect(const Vector2& pos, int drawHandle, Vector2 frameSize, int maxAnimNum, int oneAnimFrame, float scale):
	GameObject(pos, Vector2{0.0f,0.0f},0.0f,0.0f,0.0f)
{
	animation_.Init(drawHandle, 0, frameSize, maxAnimNum, oneAnimFrame, scale, false);
}

Effect::~Effect()
{
}

void Effect::Update()
{
	animation_.Update();

	if (animation_.GetIsEnd())
	{
		isDead_ = true;
	}
}

void Effect::Draw()
{
	animation_.Draw(pos_, false);
}
