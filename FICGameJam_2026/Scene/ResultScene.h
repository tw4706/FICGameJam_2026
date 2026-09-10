#pragma once
#include "Scene.h"
#include<memory>

class Button;
class ResultScene :public Scene
{
public:
	ResultScene(SceneManager& sceneManager);
	~ResultScene();

	void Init()override;
	void Update();
	void Draw();

	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpdate();
	using UpdateFunc_t = void (ResultScene::*)();
	UpdateFunc_t update_;

	void FadeDraw();
	void NormalDraw();
	using DrawFunc_t = void (ResultScene::*)();
	DrawFunc_t draw_;

private:
	int frameCount_ = 0;

	int button1FrameHandle_ = -1;
	int button2FrameHandle_ = -1;

	//ボタンのサイズ
	int buttonWidth_ = 0;
	int buttonHeight_ = 0;

	//リトライボタン、タイトルに戻るボタン
	std::unique_ptr<Button> retryButton_;
	std::unique_ptr<Button> backTitleButton_;
};

