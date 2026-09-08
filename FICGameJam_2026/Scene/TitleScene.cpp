#include "TitleScene.h"
#include "GameScene.h"
#include "../Game.h"
#include "../Application.h"
#include "../Input.h"
#include "../Button.h"
#include<Dxlib.h>
#include<memory>
#include<cassert>
#include<algorithm>

namespace
{
	//フェードの間隔
	constexpr int kFadeInterval = 60;

	//ボタンの当たり判定サイズ
	constexpr int kButtonHitWidth = 240;
	constexpr int kButtonHitHeight = 60;
}

TitleScene::TitleScene(SceneManager& sceneManager) :
	Scene(sceneManager),
	update_(&TitleScene::FadeInUpdate),
	draw_(&TitleScene::FadeDraw),
	frameCount_(kFadeInterval)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	frameCount_ = kFadeInterval;

	titleLogoHandle_ = LoadGraph(L"data/titleLogo.png");
	button1FrameHandle_ = LoadGraph(L"data/ButtonFrame.png");
	button2FrameHandle_ = LoadGraph(L"data/ButtonFrame.png");

	//ボタン画像のサイズを取得
	GetGraphSize(button1FrameHandle_, &buttonWidth_, &buttonHeight_);

	//ボタンの中心座標を計算しButtonを生成
	int centerX = Game::kScreenWidth / 2;
	int button1CenterY = Game::kScreenHeight / 2 + buttonHeight_ / 2;
	int button2CenterY = Game::kScreenHeight / 2 + 80 + buttonHeight_ / 2;

	//ボタンの生成
	startButton_ = std::make_unique<Button>(
		centerX, button1CenterY,
		kButtonHitWidth, kButtonHitHeight,
		button1FrameHandle_);

	exitButton_ = std::make_unique<Button>(
		centerX, button2CenterY,
		kButtonHitWidth, kButtonHitHeight,
		button2FrameHandle_);
}

void TitleScene::Update()
{
	(this->*update_)();
}

void TitleScene::Draw()
{
	(this->*draw_)();
}

void TitleScene::FadeInUpdate()
{
	frameCount_--;

	if (frameCount_ <= 0)
	{
		update_ = &TitleScene::NormalUpdate;
		draw_ = &TitleScene::NormalDraw;
	}
}

void TitleScene::NormalUpdate()
{
	startButton_->Update();
	exitButton_->Update();

	//スタートボタンが押されたらゲームシーンへ遷移
	if (startButton_->IsClicked())
	{
		update_ = &TitleScene::FadeOutUpdate;
		draw_ = &TitleScene::FadeDraw;
		frameCount_ = kFadeInterval;
	}

	//終了ボタンが押されたらゲーム終了
	if (exitButton_->IsClicked())
	{
		Application::GetInstance().GameEnd();
	}
}

void TitleScene::FadeOutUpdate()
{
	frameCount_--;

	if (frameCount_ <= 0)
	{
		sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_));
		return;
	}
}

void TitleScene::FadeDraw()
{
	float rate;

	if (update_ == &TitleScene::FadeInUpdate)
	{
		//フェードイン
		rate = 1.0f - (float)frameCount_ / kFadeInterval;
	}
	else
	{
		//フェードアウト
		rate = (float)frameCount_ / kFadeInterval;
	}
	rate = std::clamp(rate, 0.0f, 1.0f);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * rate));
	NormalDraw();	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::NormalDraw()
{
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x00ced1, true);

	DrawGraph(Game::kScreenWidth/2-530, Game::kScreenHeight/2-680, titleLogoHandle_, true);

	startButton_->Draw();
	exitButton_->Draw();
#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, L"タイトルシーン");
#endif
}
