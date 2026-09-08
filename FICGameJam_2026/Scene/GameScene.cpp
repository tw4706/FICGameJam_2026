#include "ResultScene.h"
#include "GameScene.h"
#include "../Game.h"
#include "../Input.h"
#include "SceneManager.h"
#include "../StageLoader.h"
#include "../GameObject/Goal.h"
#include "../GameObject/Enemy.h"
#include "../GameObject/Player.h"
#include "../Collider/RectCollider.h"
#include<Dxlib.h>
#include<memory>
#include<algorithm>
#include<set>

namespace
{
	//フェードの間隔
	constexpr int kFadeInterval = 60;

	//明るい場所の半径
	constexpr int kLightRadius = 150;

	//キャラクターの当たり判定サイズ
	constexpr float kPlayerColSize = 30.0f;
	constexpr float kEnemyColSize = 30.0f;
	constexpr float kGoalColSize = 30.0f;

	//各キャラクターの初期位置
	const Vector2 kPlayerStartPos = { 100.0f, 100.0f };
	const Vector2 kEnemyStartPos = { 200.0f, 400.0f };
	const Vector2 kGoalStartPos = { 900.0f, 100.0f };

	//ステージロード用
	const std::string kStageCsvPath = "data/CSV/stage1.csv";
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

	//経路探索を再計算する間隔
	constexpr float kRePathInterval = 15.0f;
}

GameScene::GameScene(SceneManager& sceneManager) :
	Scene(sceneManager),
	frameCount_(kFadeInterval),
	update_(&GameScene::FadeInUpdate),
	draw_(&GameScene::FadeDraw)
{
	loader_ = std::make_unique<StageLoader>();

	pPlayer_ = std::make_shared<Player>(kPlayerStartPos, Vector2(0.0f, 0.0f), 0.0f, kPlayerColSize, kPlayerColSize);
	pEnemy_ = std::make_shared<Enemy>(kEnemyStartPos, Vector2(0.0f, 0.0f), 0.0f, kEnemyColSize, kEnemyColSize);
	pEnemy_->SetPlayer(pPlayer_);

	pGoal_ = std::make_shared<Goal>(kGoalStartPos, Vector2(0.0f, 0.0f), 0.0f, kGoalColSize, kGoalColSize);

	//ゲームオブジェクトの配列に追加
	gameobjects_.push_back(pPlayer_);
	gameobjects_.push_back(pEnemy_);
	gameobjects_.push_back(pGoal_);
}

GameScene::~GameScene()
{
	
}

void GameScene::Init()
{
	pPlayer_->Init();

	pEnemy_->Init();

	pGoal_->Init();

	//壁タイルの設定
	loader_->SetWallTileId(kWallTileIds);
	//マップデータのロード
	loader_->Load(kStageCsvPath);
	//タイル画像のロード
	loader_->LoadTileset(kTilesetPath, kTileSize, kTilesetColumns, kTotalTiles);
	//壁の矩形コライダーを生成
	int scaledTileSize = static_cast<int>(kTileSize * kStageScale);
	wallColliders_ = loader_->CreateColliders(scaledTileSize, kWallTileIds);

	CreateLightGraph();
	darkMaskHandle_ = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, false);
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

	//一定間隔で敵の経路を再計算する
	rePathTimer_--;
	if (rePathTimer_ <= 0 && pEnemy_&&!pEnemy_->IsDead())
	{
		float scaledTileSize = kTileSize * kStageScale;

		//敵とプレイヤーのグリッド座標を計算
		PathFinder::Vector2Int startGrid{
			static_cast<int>(pEnemy_->GetPos().x / scaledTileSize),
			static_cast<int>(pEnemy_->GetPos().y / scaledTileSize) };
		PathFinder::Vector2Int goalGrid{
			static_cast<int>(pPlayer_->GetPos().x / scaledTileSize),
			static_cast<int>(pPlayer_->GetPos().y / scaledTileSize) };

		//ここで経路探索を行う
		auto gridPath = pathFinder_.FindPath(startGrid, goalGrid, *loader_);

		//グリッド座標→ワールド座標に変換
		std::vector<Vector2> worldPath;
		worldPath.reserve(gridPath.size());
		for (const auto& g : gridPath)
		{
			worldPath.emplace_back(
				g.x * scaledTileSize + scaledTileSize / 2.0f,
				g.y * scaledTileSize + scaledTileSize / 2.0f);
		}
		pEnemy_->SetPath(worldPath);

		rePathTimer_ = kRePathInterval;
	}

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

	//当たり判定(敵が死亡していないとき)
	if (pEnemy_ && !pEnemy_->IsDead() && collisionManager_.IsHitCollisionRect(pPlayer_->GetCollider(), pEnemy_->GetCollider()))
	{
		//衝突処理の実行
		pPlayer_->OnCollision(*pEnemy_);
		pEnemy_->OnCollision(*pPlayer_);
	}

	//ゴールに触れたらリザルトシーンに遷移する
	if (pGoal_ && collisionManager_.IsHitCollisionRect(pPlayer_->GetCollider(), pGoal_->GetCollider()))
	{
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeDraw;
		frameCount_ = kFadeInterval;
		return;
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

	DrawLightMask();

#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, L"ゲームシーン");

	DrawFormatString(0, 16, GetColor(255, 255, 255), L"FRAME:%d", frameCount_);
#endif
}

void GameScene::CreateLightGraph()
{
	int size = kLightRadius * 2;
	lightHandle_ = MakeScreen(size, size, FALSE);
	SetDrawScreen(lightHandle_);

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			//中心に近いほど白,外側に行くほど黒くする
			float dx = static_cast<float>(x - kLightRadius);
			float dy = static_cast<float>(y - kLightRadius);
			float dist = sqrtf(dx * dx + dy * dy);
			float rate = 1.0f - std::clamp(dist / kLightRadius, 0.0f, 1.0f);
			int val = static_cast<int>(rate * 255);
			DrawPixel(x, y, GetColor(val, val, val));
		}
	}

	SetDrawScreen(DX_SCREEN_BACK);
}

void GameScene::DrawLightMask()
{
	int mx, my;
	GetMousePoint(&mx, &my);

	int size = kLightRadius * 2;

	//黒背景の上にグラデーション画像を貼ったマスクを作る
	SetDrawScreen(darkMaskHandle_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), TRUE);
	DrawGraph(mx - kLightRadius, my - kLightRadius, lightHandle_, FALSE);
	SetDrawScreen(DX_SCREEN_BACK);

	//全体を黒い描画にする
	//黒い背景と白い背景をゲーム画面に重ねることで
	//マウスカーソルの周りだけ見えるようになる
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255);
	DrawGraph(0, 0, darkMaskHandle_, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
