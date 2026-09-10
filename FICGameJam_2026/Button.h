#pragma once
#include<string>

class Button
{
public:
	Button(int centerX, int centerY, int width, int height, int handle);

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
	int width_ = 0;
	int height_ = 0;
	int handle_ = -1;
	int fontHandle_ = -1;
	bool isHover_ = false;
	bool wasHover_ = false;				//保存用の前フレームでホバー状態だったかどうかのフラグ
	float scale_ = 1.0f;
	unsigned int textColor_ = 0xffffff;

	std::wstring text_;
};

