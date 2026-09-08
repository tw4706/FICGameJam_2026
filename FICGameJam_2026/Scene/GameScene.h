#pragma once
#include"Scene.h"
#include"../GameObject/GameObject.h"
#include "../Collider/CollisionManager.h"
#include "../PathFinder.h"
#include<vector>

class Goal;
class Player;
class Enemy;
class StageLoader;
class RectCollider;
class GameScene:public Scene
{
public:
	GameScene(SceneManager& sceneManager);
	~GameScene();

	void Init()override;
	void Update();
	void Draw();

	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpdate();
	using UpdateFunc_t = void (GameScene::*)();
	UpdateFunc_t update_;

	void FadeDraw();
	void NormalDraw();
	using DrawFunc_t = void (GameScene::*)();
	DrawFunc_t draw_;

private:

	//グラデーション画像を生成
	void CreateLightGraph();

	//マスクを合成する
	void DrawLightMask(); 

private:
	int frameCount_ = 0;
	int rePathTimer_ = 0;									//経路探索を再計算するタイマー
	int lightHandle_ = -1;									//マウス中心のグラデーション画像
	int darkMaskHandle_ = -1;								//毎フレーム描き直す黒マスク用スクリーン

	CollisionManager collisionManager_;						//当たり判定マネージャー
	PathFinder pathFinder_;									//経路探索
	std::shared_ptr<Player>pPlayer_;						//プレイヤー
	std::shared_ptr<Enemy>pEnemy_;							//敵
	std::shared_ptr<Goal>pGoal_;							//ゴール
	std::unique_ptr<StageLoader> loader_;					//ステージローダー
	std::vector<RectCollider> wallColliders_;				//壁の当たり判定の配列
	std::vector<std::shared_ptr<GameObject>>gameobjects_;	//ゲームオブジェクトの配列
};

