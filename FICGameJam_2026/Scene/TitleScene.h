#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "../Bg.h"

class Bg;
class Button;
class TitlePlayer;
class TitleCamera;
class TitleScene :public Scene
{
public:
	TitleScene(SceneManager& sceneManager);
	~TitleScene();

	void Init()override;
	void Update();
	void Draw();

	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpdate();
	using UpdateFunc_t = void (TitleScene::*)();
	UpdateFunc_t update_;

	void FadeDraw();
	void NormalDraw();
	using DrawFunc_t = void (TitleScene::*)();
	DrawFunc_t draw_;

private:
	int frameCount_ = 0;

	//タイトルロゴハンドル
	int titleLogoHandle_=-1;

	int button1FrameHandle_ = -1;
	int button2FrameHandle_ = -1;

	//ボタンのサイズ
	int buttonWidth_ = 0;
	int buttonHeight_ = 0;

	//スタートボタン、終了ボタン
	std::unique_ptr<Button> startButton_;
	std::unique_ptr<Button> endButton_;

	//背景
	std::unique_ptr<Bg> pBg_;
};