#include "Button.h"
#include "Input.h"
#include <Dxlib.h>

namespace
{
	constexpr float kHoverScale = 1.2f;
	constexpr float kNormalScale = 1.0f;
}

Button::Button(int centerX, int centerY, int hitWidth, int hitHeight, int handle) :
	centerX_(centerX), centerY_(centerY),
	hitWidth_(hitWidth), hitHeight_(hitHeight),
	handle_(handle)
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
	DrawRotaGraph(centerX_, centerY_, scale_, 0.0, handle_, true);

	//•¶Žš—ñ‚Ì•`‰æ(Šgk‚É‘Î‰ž‚·‚é)
	if (!text_.empty() && fontHandle_ != -1)
	{
		int textWidth = GetDrawStringWidthToHandle(text_.c_str(), static_cast<int>(text_.length()), fontHandle_);
		int scaledTextWidth = static_cast<int>(textWidth * scale_);
		int drawX = centerX_ - scaledTextWidth / 2;
		int drawY = centerY_ - static_cast<int>(GetFontSizeToHandle(fontHandle_) * scale_) / 2;

		DrawExtendStringToHandle(
			drawX, drawY,
			scale_, scale_,
			text_.c_str(), textColor_, fontHandle_);
	}
}

bool Button::IsClicked() const
{
	return isHover_ && Input::GetInstance().IsTriggered("click");
}

void Button::SetText(const std::wstring& text, int fontHandle, unsigned int color)
{
	text_ = text;
	fontHandle_ = fontHandle;
	textColor_ = color;
}
