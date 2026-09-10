#pragma once
#include <string>

class Bg
{
public:
	Bg(const std::wstring& filePath, float scrollSpeedX, float scrollSpeedY);
	~Bg();

	void Init();
	void Update();
	void Draw() const;

private:
	//画像ハンドル
	int handle_ = -1;

	//ファイルパス
	std::wstring filePath_;

	//サイズ
	int width_ = 0;
	int height_ = 0;

	//現在のスクロール量
	float scrollX_ = 0.0f;
	float scrollY_ = 0.0f;

	//スクロール速度
	float scrollSpeedX_;
	float scrollSpeedY_;
};

