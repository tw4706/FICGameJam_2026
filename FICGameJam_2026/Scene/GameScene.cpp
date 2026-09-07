#include "ResultScene.h"
#include "GameScene.h"
#include "../GameObject/Player.h"
#include "../GameObject/Enemy.h"
#include "../Game.h"
#include "../Input.h"
#include "SceneManager.h"
#include<Dxlib.h>
#include<memory>
#include<algorithm>

namespace
{
	//フェードの間隔
	constexpr int kFadeInterval = 60;
}

GameScene::GameScene(SceneManager& sceneManager) :
	Scene(sceneManager),
	frameCount_(0),
	update_(&GameScene::FadeInUpdate),
	draw_(&GameScene::FadeDraw)
{
	pPlayer_ = std::make_shared<Player>(Vector2(100.0f, 100.0f), Vector2(0.0f, 0.0f), 0.0f);
	pEnemy_ = std::make_shared<Enemy>(Vector2(200.0f, 400.0f), Vector2(0.0f, 0.0f), 0.0f);
	pEnemy_->SetPlayer(pPlayer_);
}

GameScene::~GameScene()
{
	
}

void GameScene::Init()
{
	pPlayer_->Init();
}

void GameScene::Update()
{
	(this->*update_)();
}

void GameScene::Draw()
{

	(this->*draw_)();
}

void GameScene::FadeInUpdate()
{
	frameCount_--;

	if (frameCount_ <= 0)
	{
		update_ = &GameScene::NormalUpdate;
		draw_ = &GameScene::NormalDraw;
		return;
	}
}

void GameScene::NormalUpdate()
{
	frameCount_++;

	//プレイヤーの更新
	pPlayer_->Update();

	//敵の更新
	pEnemy_->Update();

	if (Input::GetInstance().IsPressed("next"))
	{
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeDraw;
		frameCount_ = kFadeInterval;
	}
}

void GameScene::FadeOutUpdate()
{
	frameCount_--;

	if (frameCount_ < 0)
	{
		sceneManager_.ChangeScene(std::make_shared<ResultScene>(sceneManager_));
		return;
	}
}

void GameScene::FadeDraw()
{
	float rate;

	if (update_ == &GameScene::FadeInUpdate)
	{
		//フェードイン
		rate = (float)frameCount_ / kFadeInterval;
	}
	else
	{
		//フェードアウト
		rate = 1.0f - (float)frameCount_ / kFadeInterval;
	}

	rate = std::clamp(rate, 0.0f, 1.0f);

	NormalDraw();
}

void GameScene::NormalDraw()
{
	//プレイヤーの描画
	pPlayer_->Draw();

	//敵の描画
	pEnemy_->Draw();
#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, L"ゲームシーン");

	DrawFormatString(0, 16, GetColor(255, 255, 255), L"FRAME:%d", frameCount_);
#endif
}
