#include "ResultScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "../Game.h"
#include "../Input.h"
#include "../Button.h"
#include "../SoundManager.h"
#include<Dxlib.h>
#include<memory>
#include<cmath>
#include<algorithm>
#include<cassert>

namespace
{
	//フェードの間隔
	constexpr int kFadeInterval = 60;

	//ボタンの当たり判定サイズ
	constexpr int kButtonWidth = 240;
	constexpr int kButtonHeight = 60;
}

ResultScene::ResultScene(SceneManager& sceneManager,ResultType result) :
	Scene(sceneManager),
	result_(result),
	update_(&ResultScene::FadeInUpdate),
	draw_(&ResultScene::FadeDraw),
	frameCount_(kFadeInterval)
{
}

ResultScene::~ResultScene()
{
	DeleteGraph(button1FrameHandle_);
	DeleteGraph(button2FrameHandle_);
}

void ResultScene::Init()
{
	frameCount_ = kFadeInterval;

	SetMouseDispFlag(true);

	pBg_ = std::make_unique<Bg>(L"data/Bg.png", 0.5f, 0.3f);
	pBg_->Init();

	button1FrameHandle_ = LoadGraph(L"data/ResultButonFrame.png");
	button2FrameHandle_ = LoadGraph(L"data/ResultButonFrame.png");

	//ボタン画像のサイズを取得
	GetGraphSize(button1FrameHandle_, &buttonWidth_, &buttonHeight_);

	//ボタンの中心座標を計算しButtonを生成
	int centerX = Game::kScreenWidth / 2;
	int button1CenterY = Game::kScreenHeight / 2 + buttonHeight_ / 2;
	int button2CenterY = Game::kScreenHeight / 2 + 80 + buttonHeight_ / 2;

	//ボタンの生成
	retryButton_ = std::make_unique<Button>(
		centerX, button1CenterY,
		kButtonWidth, kButtonHeight,
		button1FrameHandle_);
	retryButton_->SetText(L"リトライ", Game::kFontUIHandle, 0x000000);

	backTitleButton_ = std::make_unique<Button>(
		centerX, button2CenterY,
		kButtonWidth, kButtonHeight,
		button2FrameHandle_);
	backTitleButton_->SetText(L"タイトルに戻る", Game::kFontUIHandle, 0x000000);

	SoundManager::GetInstance().PlayBgm(BGM::Result);
}

void ResultScene::Update()
{
	(this->*update_)();
}

void ResultScene::Draw()
{
	(this->*draw_)();
}

void ResultScene::FadeInUpdate()
{
	frameCount_--;

	if (frameCount_ <= 0)
	{
		update_ = &ResultScene::NormalUpdate;
		draw_ = &ResultScene::NormalDraw;
		return;
	}
}

void ResultScene::NormalUpdate()
{
	pBg_->Update();

	retryButton_->Update();
	backTitleButton_->Update();

	//スタートボタンが押されたらゲームシーンへ遷移
	if (backTitleButton_->IsClicked())
	{
		update_ = &ResultScene::FadeOutUpdate;
		draw_ = &ResultScene::FadeDraw;
		frameCount_ = kFadeInterval;
	}

	//リトライしたらステージ1に遷移
	if(retryButton_->IsClicked())
	{
		sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_,GameScene::StageType::Stage1));
		return;
	}
}

void ResultScene::FadeOutUpdate()
{
	frameCount_--;

	if (frameCount_ < 0)
	{
		sceneManager_.ChangeScene(std::make_shared<TitleScene>(sceneManager_));
		return;
	}
}

void ResultScene::FadeDraw()
{
	float rate;

	if (update_ == &ResultScene::FadeInUpdate)
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

void ResultScene::NormalDraw()
{
	pBg_->Draw();

	//クリア/ゲームオーバーで表示テキストと色を切り替える
	const wchar_t* text = (result_ == ResultType::Clear) ? L"クリア！" : L"ゲームオーバー";
	unsigned int color = (result_ == ResultType::Clear) ? 0xffffff : 0xff0000;

	int textLen = static_cast<int>(wcslen(text));
	int textWidth = GetDrawStringWidthToHandle(text, textLen, Game::kFontUIHandle);
	int drawX = Game::kScreenWidth / 2 - textWidth / 2-40;
	int drawY = (Game::kScreenHeight/2 - 100) - GetFontSizeToHandle(Game::kFontUIHandle) / 2;

	DrawExtendStringToHandle(drawX, drawY, 2.0f,2.0f,text, 0xffffff, Game::kFontUIHandle);
	DrawExtendStringToHandle(drawX+4, drawY+4, 2.0f,2.0f,text, 0x000000, Game::kFontUIHandle);

	retryButton_->Draw();
	backTitleButton_->Draw();
#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, L"リザルトシーン");
#endif
}
