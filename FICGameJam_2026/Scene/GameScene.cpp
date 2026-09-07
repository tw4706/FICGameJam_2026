#include "ResultScene.h"
#include "GameScene.h"
#include "../GameObject/Player.h"
#include "../GameObject/Enemy.h"
#include "../Game.h"
#include "../Input.h"
#include "SceneManager.h"
#include "../StageLoader.h"
#include "../Collider/RectCollider.h"
#include<Dxlib.h>
#include<memory>
#include<algorithm>
#include<set>

namespace
{
	//フェードの間隔
	constexpr int kFadeInterval = 60;

	//キャラクターの当たり判定サイズ
	constexpr float kPlayerColSize = 30.0f;
	constexpr float kEnemyColSize = 30.0f;

	//各キャラクターの初期位置
	const Vector2 kPlayerStartPos = { 100.0f, 100.0f };
	const Vector2 kEnemyStartPos = { 200.0f, 400.0f };

	//ステージロード用
	const std::string kStageCsvPath = "data/stage1.csv";
	const std::string kTilesetPath = "data/tileset.png";

	//1タイルのサイズ
	constexpr int kTileSize = 64;

	//ステージの拡大率
	constexpr float kStageScale = 1.05f;

	//タイル画像の横の枚数
	constexpr int kTilesetColumns = 6;

	//タイル画像の総枚数
	constexpr int kTotalTiles = 18;

	//壁として判定するタイルIDの集合
	const std::set<int> kWallTileIds = { 0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 17 };
}

GameScene::GameScene(SceneManager& sceneManager) :
	Scene(sceneManager),
	frameCount_(0),
	update_(&GameScene::FadeInUpdate),
	draw_(&GameScene::FadeDraw)
{
	loader_ = std::make_unique<StageLoader>();

	pPlayer_ = std::make_shared<Player>(kPlayerStartPos, Vector2(0.0f, 0.0f), 0.0f, kPlayerColSize, kPlayerColSize);
	pEnemy_ = std::make_shared<Enemy>(kEnemyStartPos, Vector2(0.0f, 0.0f), 0.0f, kEnemyColSize, kEnemyColSize);
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

	//マップデータのロード
	loader_->Load(kStageCsvPath);

	//タイル画像のロード
	loader_->LoadTileset(kTilesetPath, kTileSize, kTilesetColumns, kTotalTiles);

	//壁の矩形コライダーを生成
	int scaledTileSize = static_cast<int>(kTileSize * kStageScale);
	wallColliders_ = loader_->CreateColliders(scaledTileSize, kWallTileIds);
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
	for(auto& gameObject : gameobjects_)
	{
		gameObject->Update();
	}

	//各キャラクターと壁コライダーの当たり判定処理
	for (int i = 0; i < 2; i++)
	{
		for (const auto& wall : wallColliders_)
		{
			//プレイヤーの壁判定
			if (collisionManager_.IsHitCollisionRect(pPlayer_->GetCollider(), wall))
			{
				Vector2 pushVector = collisionManager_.GetOverlapRect(pPlayer_->GetCollider(), wall);
				pPlayer_->AdjustPosition(pushVector);
			}

			//敵の壁判定
			if (pEnemy_ && collisionManager_.IsHitCollisionRect(pEnemy_->GetCollider(), wall))
			{
				Vector2 pushVector = collisionManager_.GetOverlapRect(pEnemy_->GetCollider(), wall);
				pEnemy_->AdjustPosition(pushVector);
			}
		}
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
	//ステージの描画
	if (loader_)
	{
		loader_->Draw(0, 0, kStageScale);
	}

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
