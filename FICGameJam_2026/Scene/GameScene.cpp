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
#include "../EffectManager.h"
#include "../SoundManager.h"
#include"../SaveData.h"
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

	constexpr float kKeyColSize = 30.0f;

	//各キャラクターの初期位置
	const Vector2 kPlayerStartPos = { 100.0f, 100.0f };
	const Vector2 kEnemyStartPos = { 200.0f, 400.0f };
	const Vector2 kGoalStartPos = { 930.0f, 100.0f };

	//ステージロード用
	const std::string kStageCsvPath = "data/CSV/stage1.csv";
	const std::string kTutorialCsvPath = "data/CSV/tutorial.csv";
	const std::string kTilesetPath = "data/tileset.png";

	//宝箱の設置位置
	const Vector2 kChestKeyPos = { 745.0f, 350.0f };
	const Vector2 kChestEnemyPos1 = { 610.0f, 610.0f };
	const Vector2 kChestEnemyPos2 = { 470.0f, 230.0f };

	//チュートリアルのキャラの位置
	const Vector2 kTutorialPlayerStartPos = { 150.0f, 150.0f };
	const Vector2 kTutorialGoalStartPos = { 900.0f, 300.0f };
	const Vector2 kTutorialChestKeyPos = { 550.0f, 400.0f };

	//鍵の座標
	const Vector2 kKeyPos = { 770.0f, 360.0f };

	//宝箱をプレイヤーが開けられる距離
	constexpr float kChestOpenRange = 60.0f;

	//1タイルのサイズ
	constexpr int kTileSize = 64;

	//ステージの拡大率
	constexpr float kStageScale = 1.01f;

	//タイル画像の横の枚数
	constexpr int kTilesetColumns = 6;

	//タイル画像の総枚数
	constexpr int kTotalTiles = 18;

	//壁として判定するタイルIDの集合
	const std::set<int> kWallTileIds = { 0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 17 };

	//経路探索を再計算する間隔
	constexpr float kRePathInterval = 15.0f;

	//敵の生成にかかる時間
	constexpr int kSpawnEnemyTime = 60;
}

GameScene::GameScene(SceneManager& sceneManager, StageType type) :
	Scene(sceneManager),
	frameCount_(kFadeInterval),
	update_(&GameScene::FadeInUpdate),
	draw_(&GameScene::FadeDraw),
	type_(type)
{
	loader_ = std::make_unique<StageLoader>();
	if (type_ == StageType::Tutorial)
	{
		//チュートリアルの配置
		pPlayer_ = std::make_shared<Player>(kTutorialPlayerStartPos, Vector2(0.0f, 0.0f), 0.0f, kPlayerColSize, kPlayerColSize);
		pGoal_ = std::make_shared<Goal>(kTutorialGoalStartPos, Vector2(0.0f, 0.0f), 0.0f, kGoalColSize, kGoalColSize);

		pChests_.push_back(std::make_shared<Chest>(kTutorialChestKeyPos, Chest::ChestContents::Key));

		tutorialMesseage_ =
		{
			L"今からチュートリアルをはじめます。",
			L"プレイヤーはあかりの中に入っていると移動ができます。",
			L"あかりから出ると停止します。",
			L"逆に敵はあかりに一定時間いると倒れます。",
			L"あかりの外にいると敵はプレイヤーを襲ってきます。",
			L"右クリックで宝箱を開けると鍵が出てきます。",
			L"中には意外なものも...",
			L"では鍵を見つけてゴールを目指しましょう！",
		};
	}
	else
	{
		pPlayer_ = std::make_shared<Player>(kPlayerStartPos, Vector2(0.0f, 0.0f), 0.0f, kPlayerColSize, kPlayerColSize);
		pGoal_ = std::make_shared<Goal>(kGoalStartPos, Vector2(0.0f, 0.0f), 0.0f, kGoalColSize, kGoalColSize);

		pChests_.push_back(std::make_shared<Chest>(kChestKeyPos, Chest::ChestContents::Key));
		pChests_.push_back(std::make_shared<Chest>(kChestEnemyPos1, Chest::ChestContents::Enemy));
		pChests_.push_back(std::make_shared<Chest>(kChestEnemyPos2, Chest::ChestContents::Enemy));
	}

	//ゲームオブジェクトの配列に追加
	gameobjects_.push_back(pGoal_);

	for (auto& chest : pChests_)
	{
		gameobjects_.push_back(chest);
	}
	gameobjects_.push_back(pPlayer_);
}

GameScene::~GameScene()
{
	DeleteGraph(lightHandle_);
	DeleteGraph(darkMaskHandle_);
	DeleteFontToHandle(tutorialFontHandle_);
}

void GameScene::Init()
{
	SetMouseDispFlag(false);

	//各オブジェクトの初期化
	pPlayer_->Init();

	//マウスの位置をプレイヤーの位置にする
	SetMousePoint(static_cast<int>(pPlayer_->GetPos().x), static_cast<int>(pPlayer_->GetPos().y));

	pGoal_->Init();

	for (auto& chest : pChests_)
	{
		chest->Init();
	}

	if (type_ == StageType::Tutorial)
	{
		tutorialFontHandle_ = CreateFontToHandle(NULL, 25, 6);
	}

	//死亡エフェクトのロード
	EffectManager::GetInstance().Load(L"death", L"data/deathEffect.png",
		Vector2{ 192.0f, 192.0f }, 7, 4);

	//壁タイルの設定
	loader_->SetWallTileId(kWallTileIds);

	//マップデータのロード
	const std::string& stageCsvPath = (type_ == StageType::Tutorial) ? kTutorialCsvPath : kStageCsvPath;
	loader_->Load(stageCsvPath);

	//タイル画像のロード
	loader_->LoadTileset(kTilesetPath, kTileSize, kTilesetColumns, kTotalTiles);

	//壁の矩形コライダーを生成
	int scaledTileSize = static_cast<int>(kTileSize * kStageScale);
	wallColliders_ = loader_->CreateColliders(scaledTileSize, kWallTileIds);

	CreateLightGraph();
	darkMaskHandle_ = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, false);

	SoundManager::GetInstance().PlayBgm(BGM::Game);
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
		//チュートリアルならメッセージ表示状態へ、それ以外は通常状態へ
		if (type_ == StageType::Tutorial && !tutorialMesseage_.empty())
		{
			update_ = &GameScene::TutorialMessegeUpdate;
			draw_ = &GameScene::TutorialMessegeDraw;
		}
		else
		{
			update_ = &GameScene::NormalUpdate;
			draw_ = &GameScene::NormalDraw;
		}
		return;
	}
}

void GameScene::TutorialMessegeUpdate()
{

	//メッセージ表示中もプレイヤーが動けるようにする
	pPlayer_->Update();

	//プレイヤーと壁の当たり判定
	for (const auto& wall : wallColliders_)
	{
		if (collisionManager_.IsHitCollisionRect(pPlayer_->GetCollider(), wall))
		{
			Vector2 pushVector = collisionManager_.GetOverlapRect(pPlayer_->GetCollider(), wall);
			pPlayer_->AdjustPosition(pushVector);
		}
	}

	if (Input::GetInstance().IsTriggered("next"))
	{
		tutorialMesseageIndex_++;

		SoundManager::GetInstance().PlaySe(SE::CursoleMove);

		//全メッセージを見終わったら通常状態へ移行
		if (tutorialMesseageIndex_ >= static_cast<int>(tutorialMesseage_.size()))
		{
			update_ = &GameScene::NormalUpdate;
			draw_ = &GameScene::NormalDraw;
			return;
		}
	}
}

void GameScene::NormalUpdate()
{
	frameCount_++;

	//チュートリアルのメッセージを閉じてもまた見れるようにする
	if (type_ == StageType::Tutorial && !tutorialMesseage_.empty() &&
		Input::GetInstance().IsTriggered("onemore"))
	{
		tutorialMesseageIndex_ = 0;
		update_ = &GameScene::TutorialMessegeUpdate;
		draw_ = &GameScene::TutorialMessegeDraw;
		return;
	}

	//宝箱の更新
	UpdateChests();

	//一定間隔で敵の経路を再計算する
	rePathTimer_--;
	if (rePathTimer_ <= 0)
	{
		float scaledTileSize = kTileSize * kStageScale;

		for (auto& enemy : pEnemies_)
		{
			if (!enemy || enemy->IsDead())
			{
				continue;
			}

			//敵とプレイヤーのグリッド座標を計算
			PathFinder::Vector2Int startGrid{
				static_cast<int>(enemy->GetPos().x / scaledTileSize),
				static_cast<int>(enemy->GetPos().y / scaledTileSize) };
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
			enemy->SetPath(worldPath);
		}

		rePathTimer_ = kRePathInterval;
	}

	//ゲームオブジェクトの更新
	for (auto& gameObject : gameobjects_)
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
			for (auto& enemy : pEnemies_)
			{
				if (enemy && collisionManager_.IsHitCollisionRect(enemy->GetCollider(), wall))
				{
					Vector2 pushVector = collisionManager_.GetOverlapRect(enemy->GetCollider(), wall);
					enemy->AdjustPosition(pushVector);
				}
			}
		}
	}

	//当たり判定(敵が死亡していないとき)
	for (auto& enemy : pEnemies_)
	{
		if (enemy && !enemy->IsDead() && !enemy->IsSpawn() &&
			collisionManager_.IsHitCollisionRect(pPlayer_->GetCollider(), enemy->GetCollider()))
		{
			//衝突処理の実行
			pPlayer_->OnCollision(*enemy);
			enemy->OnCollision(*pPlayer_);
		}
	}

	//ゴールに触れたらリザルトシーンに遷移する
	if (isKey_)
	{
		if (pGoal_ && collisionManager_.IsHitCollisionRect(pPlayer_->GetCollider(), pGoal_->GetCollider()))
		{
			update_ = &GameScene::FadeOutUpdate;
			draw_ = &GameScene::FadeDraw;
			frameCount_ = kFadeInterval;
			return;
		}
	}

	//鍵との当たり判定
	if (pKey_ && collisionManager_.IsHitCollisionRect(pKey_->GetCollider(), pPlayer_->GetCollider()))
	{
		//当たっていたら削除
		pKey_->Destroy();
		isKey_ = true;
		pKey_ = nullptr;

		SoundManager::GetInstance().PlaySe(SE::GetKey);
	}

	//プレイヤーが死んだときリザルトシーンに遷移
	if (pPlayer_ && pPlayer_->IsDead())
	{
		isGameOver_ = true;
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeDraw;
		frameCount_ = kFadeInterval;
		return;
	}
#ifdef _DEBUG
	if (Input::GetInstance().IsPressed("next"))
	{
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeDraw;
		frameCount_ = kFadeInterval;
	}
#endif

	//死亡したゲームオブジェクトの削除
	gameobjects_.erase(std::remove_if(gameobjects_.begin(), gameobjects_.end(),
		[](const std::shared_ptr<GameObject>& obj) { return obj->IsDead(); }),
		gameobjects_.end());

	pEnemies_.erase(std::remove_if(pEnemies_.begin(), pEnemies_.end(),
		[](const std::shared_ptr<Enemy>& enemy)
		{
			bool isDead = !enemy || enemy->IsDead();

			//敵が死んでいるときに音を再生
			if (isDead && enemy)
			{
				SoundManager::GetInstance().PlaySe(SE::Death);
			}
			return isDead;
		}),
		pEnemies_.end());
}

void GameScene::FadeOutUpdate()
{
	frameCount_--;

	if (frameCount_ < 0)
	{
		if (type_ == StageType::Tutorial)
		{
			sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_, GameScene::StageType::Stage1));
		}
		else
		{
			//死亡していればゲームオーバー、そうでなければクリアとして渡す
			ResultScene::ResultType result = isGameOver_ ? ResultScene::ResultType::GameOver : ResultScene::ResultType::Clear;
			sceneManager_.ChangeScene(std::make_shared<ResultScene>(sceneManager_, result));
		}
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

void GameScene::TutorialMessegeDraw()
{
	NormalDraw();

	//メッセージウィンドウの座標
	int boxX1 = 100;
	int boxY1 = Game::kScreenHeight - 220;
	int boxX2 = Game::kScreenWidth - 100;
	int boxY2 = Game::kScreenHeight - 60;

	//半透明の黒背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(boxX1, boxY1, boxX2, boxY2, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//枠線
	DrawBox(boxX1, boxY1, boxX2, boxY2, 0xffffff, FALSE);

	//現在のメッセージ本文
	if (tutorialMesseageIndex_ < static_cast<int>(tutorialMesseage_.size()))
	{
		const std::wstring& text = tutorialMesseage_[tutorialMesseageIndex_];
		DrawStringToHandle(boxX1 + 30, boxY1 + 30, text.c_str(), 0xffffff, tutorialFontHandle_);
	}

	//続行案内
	const wchar_t* hintText = L"[Enter]で次へ";
	int hintWidth = GetDrawStringWidthToHandle(hintText, static_cast<int>(wcslen(hintText)), tutorialFontHandle_);
	DrawStringToHandle(boxX2 - hintWidth - 20, boxY2-40, hintText, 0xaaaaaa, tutorialFontHandle_);
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

	//チュートリアルを後から見れるように表示している
	if (type_ == StageType::Tutorial)
	{
		DrawStringToHandle(Game::kScreenWidth-250, Game::kScreenHeight - 40, L"[O]で説明を見る", 0xffffff, tutorialFontHandle_);
	}

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
	int mx = Input::GetInstance().GetMouseX();
	int my = Input::GetInstance().GetMouseY();

	int size = kLightRadius * 2;

	//黒背景の上にグラデーション画像を貼ったマスクを作る
	SetDrawScreen(darkMaskHandle_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), TRUE);
	DrawGraph(mx - kLightRadius, my - kLightRadius, lightHandle_, FALSE);
	SetDrawScreen(DX_SCREEN_BACK);

	//全体を黒い描画にする
	//黒い背景と白い背景を乗算することで
	//マウスカーソルの周りだけ見えるようになる
	SetDrawBlendMode(DX_BLENDMODE_MULA, 233);
	DrawGraph(0, 0, darkMaskHandle_, FALSE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameScene::UpdateChests()
{
	//プレイヤーが近づいた宝箱を開ける
	for (auto& chest : pChests_)
	{
		if (chest->IsOpened())
		{
			continue;
		}

		Vector2 diff = chest->GetPos() - pPlayer_->GetPos();
		float dist = sqrtf(diff.x * diff.x + diff.y * diff.y);

		if (dist <= kChestOpenRange && Input::GetInstance().IsPressed("open"))
		{
			chest->Open();
		}
	}

	//開けた宝箱の処理を行う
	for (auto& chest : pChests_)
	{
		if (!chest->IsOpenEvent())
		{
			continue;
		}

		if (chest->GetContent() == Chest::ChestContents::Key)
		{
			//鍵を宝箱の位置に出現させる
			Vector2 keyPos = chest->GetPos();
			auto newKey = std::make_shared<Key>(keyPos, Vector2{ 0.0f,0.0f }, 0.0f, kKeyColSize, kKeyColSize);
			newKey->Init();

			pKey_ = newKey;
			gameobjects_.push_back(newKey);

			SoundManager::GetInstance().PlaySe(SE::Spawn);
		}
		else if (chest->GetContent() == Chest::ChestContents::Enemy)
		{
			//敵を宝箱の位置に出現させる
			auto newEnemy = std::make_shared<Enemy>(
				chest->GetPos(), Vector2(0.0f, 0.0f), 0.0f, kEnemyColSize, kEnemyColSize);
			newEnemy->Init();
			newEnemy->SetPlayer(pPlayer_);
			newEnemy->SetSpawnTime(kSpawnEnemyTime);

			pEnemies_.push_back(newEnemy);
			gameobjects_.push_back(newEnemy);
			SoundManager::GetInstance().PlaySe(SE::Spawn);
		}
	}
}
