#pragma once
#include"Scene.h"
#include"../GameObject/GameObject.h"
#include "../Collider/CollisionManager.h"
#include "../PathFinder.h"
#include "../GameObject/Chest.h"
#include "../GameObject/Key.h"
#include<vector>

class Goal;
class Player;
class Enemy;
class StageLoader;
class RectCollider;
class GameScene:public Scene
{
public:

	enum class StageType
	{
		Tutorial,
		Stage1
	};

	GameScene(SceneManager& sceneManager,StageType type);
	~GameScene();

	void Init()override;
	void Update();
	void Draw();

	void FadeInUpdate();
	void TutorialMessegeUpdate();
	void NormalUpdate();
	void FadeOutUpdate();
	using UpdateFunc_t = void (GameScene::*)();
	UpdateFunc_t update_;

	void FadeDraw();
	void TutorialMessegeDraw();
	void NormalDraw();
	using DrawFunc_t = void (GameScene::*)();
	DrawFunc_t draw_;

private:

	//グラデーション画像を生成
	void CreateLightGraph();

	//マスクを合成する
	void DrawLightMask(); 

	//宝箱の更新
	void UpdateChests();

private:
	int frameCount_ = 0;
	int rePathTimer_ = 0;									//経路探索を再計算するタイマー
	int lightHandle_ = -1;									//マウス中心の画像
	int darkMaskHandle_ = -1;								//毎フレーム描き直す黒マスク用スクリーン
	bool isKey_ = false;									//鍵を持っているかどうか
	int tutorialMesseageIndex_ = 0;							//表示中の番号
	int tutorialFontHandle_ = -1;

	CollisionManager collisionManager_;						//当たり判定マネージャー
	PathFinder pathFinder_;									//経路探索
	StageType type_;										//ステージタイプ
	std::shared_ptr<Player>pPlayer_;						//プレイヤー
	std::shared_ptr<Goal>pGoal_;							//ゴール
	std::shared_ptr<Key> pKey_;								//鍵
	std::unique_ptr<StageLoader> loader_;					//ステージローダー
	std::vector<std::shared_ptr<Chest>> pChests_;			//宝箱
	std::vector<std::shared_ptr<Enemy>> pEnemies_;			//敵
	std::vector<RectCollider> wallColliders_;				//壁の当たり判定の配列
	std::vector<std::wstring>tutorialMesseage_;				//チュートリアルの説明文
	std::vector<std::shared_ptr<GameObject>>gameobjects_;	//ゲームオブジェクトの配列
};