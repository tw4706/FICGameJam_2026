#pragma once
#include<string>

class Button
{
public:
	Button(int centerX, int centerY, int hitWidth, int hitHeight, int handle);

	void Update();
	void Draw() const;

	bool IsHovered() const { return isHover_; }

	//クリックされた瞬間か
	bool IsClicked() const;

	//ボタンに表示する文字列の設定
	void SetText(const std::wstring& text, int fontHandle, unsigned int color);

private:
	int centerX_ = 0;
	int centerY_ = 0;
	int hitWidth_ = 0;
	int hitHeight_ = 0;
	int handle_ = -1;
	int fontHandle_ = -1;
	bool isHover_ = false;
	float scale_ = 1.0f;
	unsigned int textColor_ = 0xffffff;

	std::wstring text_;
};

