#include "Button.h"
#include "Input.h"
#include <Dxlib.h>

namespace
{
	constexpr float kHoverScale = 1.2f;
	constexpr float kNormalScale = 1.0f;
}

Button::Button(int centerX, int centerY, int hitWidth, int hitHeight, int imageHandle) :
	centerX_(centerX), centerY_(centerY),
	hitWidth_(hitWidth), hitHeight_(hitHeight),
	imageHandle_(imageHandle)
{
}

void Button::Update()
{
	auto& input = Input::GetInstance();
	int mouseX = input.GetMouseX();
	int mouseY = input.GetMouseY();

	isHover_ =
		mouseX >= centerX_ - hitWidth_ / 2 &&
		mouseX <= centerX_ + hitWidth_ / 2 &&
		mouseY >= centerY_ - hitHeight_ / 2 &&
		mouseY <= centerY_ + hitHeight_ / 2;

	scale_ = isHover_ ? kHoverScale : kNormalScale;
}

void Button::Draw() const
{
	DrawRotaGraph(centerX_, centerY_, scale_, 0.0, imageHandle_, true);
}

bool Button::IsClicked() const
{
	return isHover_ && Input::GetInstance().IsTriggered("click");
}
