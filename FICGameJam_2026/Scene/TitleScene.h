#pragma once
#include "Scene.h"
#include "SceneManager.h"

class Bg;
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

	//マウス座標
	int mouseX_ = 0;
	int mouseY_ = 0;

	bool isButton1Hover_ = false;
	bool isButton2Hover_ = false;

	bool mouseLeftWasPressed_ = false;

	//ボタンの拡縮率
	float button1Scale_ = 1.0f;
	float button2Scale_ = 1.0f;

	//ボタンの座標
	int button1X_ = 0;
	int button1Y_ = 0;
	int button2X_ = 0;
	int button2Y_ = 0;

	//ボタンのサイズ
	int buttonWidth_ = 0;
	int buttonHeight_ = 0;
};