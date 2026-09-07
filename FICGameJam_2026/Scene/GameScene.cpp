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

	//各キャラクターの初期位置
	const Vector2 kPlayerStartPos = { 100.0f, 100.0f };
	const Vector2 kEnemyStartPos = { 200.0f, 400.0f };
}

GameScene::GameScene(SceneManager& sceneManager) :
	Scene(sceneManager),
	frameCount_(0),
	update_(&GameScene::FadeInUpdate),
	draw_(&GameScene::FadeDraw)
{
	pPlayer_ = std::make_shared<Player>(kPlayerStartPos, Vector2(0.0f, 0.0f), 0.0f, 30.0f, 30.0f);
	pEnemy_ = std::make_shared<Enemy>(kEnemyStartPos, Vector2(0.0f, 0.0f), 0.0f, 30.0f, 30.0f);
	pEnemy_->SetPlayer(pPlayer_);

	//ゲームオブジェクトの配列に追加
	gameobjects_.push_back(pPlayer_);
	gameobjects_.push_back(pEnemy_);
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

	//ゲームオブジェクトの更新
	for(auto gameObject : gameobjects_)
	{
		gameObject->Update();
	}

	//当たり判定
	if (collisionManager_.IsHitCollisionRect(pPlayer_->GetCollider(), pEnemy_->GetCollider()))
	{
		//衝突処理の実行
		pPlayer_->OnCollision(*pEnemy_);
		pEnemy_->OnCollision(*pPlayer_);
	}

	if (Input::GetInstance().IsPressed("next"))
	{
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeDraw;
		frameCount_ = kFadeInterval;
	}

	//死亡したゲームオブジェクトの削除
	gameobjects_.erase(std::remove_if(gameobjects_.begin(), gameobjects_.end(),
		[](const std::shared_ptr<GameObject>& obj) { return obj->IsDead(); }),
		gameobjects_.end());
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
	//ゲームオブジェクトの描画
	for (auto& gameobject : gameobjects_)
	{
		gameobject->Draw();
	}

#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, L"ゲームシーン");

	DrawFormatString(0, 16, GetColor(255, 255, 255), L"FRAME:%d", frameCount_);
#endif
}
