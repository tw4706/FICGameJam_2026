#include "TitleScene.h"
#include "GameScene.h"
#include "../Game.h"
#include "../Application.h"
#include "../Input.h"
#include<Dxlib.h>
#include<memory>
#include<cassert>
#include<algorithm>

namespace
{
	//フェードの間隔
	constexpr int kFadeInterval = 60;
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

	//ボタンの左上座標
	button1X_ = Game::kScreenWidth / 2 - buttonWidth_ / 2;
	button1Y_ = Game::kScreenHeight / 2;

	button2X_ = Game::kScreenWidth / 2 - buttonWidth_ / 2;
	button2Y_ = Game::kScreenHeight / 2 + 80;

	mouseLeftWasPressed_ = false;
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
	//マウス座標を取得
	GetMousePoint(&mouseX_, &mouseY_);

	//ボタンの中心座標
	int button1CenterX =
		button1X_ + buttonWidth_ / 2;

	int button1CenterY =
		button1Y_ + buttonHeight_ / 2;

	int button2CenterX =
		button2X_ + buttonWidth_ / 2;

	int button2CenterY =
		button2Y_ + buttonHeight_ / 2;

	//ボタンの当たり判定
	const int hitWidth = 240;
	const int hitHeight = 60;

	//上のボタンにカーソルがあるか
	isButton1Hover_ =
		mouseX_ >= button1CenterX - hitWidth / 2 &&
		mouseX_ <= button1CenterX + hitWidth / 2 &&
		mouseY_ >= button1CenterY - hitHeight / 2 &&
		mouseY_ <= button1CenterY + hitHeight / 2;

	//下のボタンにカーソルがあるか
	isButton2Hover_ =
		mouseX_ >= button2CenterX - hitWidth / 2 &&
		mouseX_ <= button2CenterX + hitWidth / 2 &&
		mouseY_ >= button2CenterY - hitHeight / 2 &&
		mouseY_ <= button2CenterY + hitHeight / 2;


	//マウスオーバーしたら拡大
	button1Scale_ = isButton1Hover_ ? 1.2f : 1.0f;
	button2Scale_ = isButton2Hover_ ? 1.2f : 1.0f;


	//左クリック状態
	int mouseInput = GetMouseInput();

	bool mouseLeftPressed =
		(mouseInput & MOUSE_INPUT_LEFT) != 0;


	//「押した瞬間」だけ判定
	bool mouseLeftTriggered =
		mouseLeftPressed && !mouseLeftWasPressed_;

	if (mouseLeftTriggered && isButton1Hover_)
	{
		update_ = &TitleScene::FadeOutUpdate;
		draw_ = &TitleScene::FadeDraw;
		frameCount_ = kFadeInterval;
	}

	//下ボタンを押されたらゲーム終了
	if (mouseLeftTriggered && isButton2Hover_)
	{
		Application::GetInstance().GameEnd();
	}


	//前フレームのクリック状態を保存
	mouseLeftWasPressed_ = mouseLeftPressed;
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

	//上ボタン
	int button1CenterX = button1X_ + buttonWidth_ / 2;
	int button1CenterY = button1Y_ + buttonHeight_ / 2;

	DrawRotaGraph(button1CenterX,button1CenterY,
		button1Scale_,0.0,button1FrameHandle_,true);

	//下ボタン
	int button2CenterX = button2X_ + buttonWidth_ / 2;
	int button2CenterY = button2Y_ + buttonHeight_ / 2;

	DrawRotaGraph(button2CenterX,button2CenterY,
		button2Scale_,0.0,button2FrameHandle_,true);

#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, L"タイトルシーン");
#endif
}