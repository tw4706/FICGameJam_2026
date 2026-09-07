#pragma once
#include"Scene.h"
#include"../GameObject/GameObject.h"
#include "../Collider/CollisionManager.h"
#include<vector>

class Player;
class Enemy;
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
	int frameCount_ = 0;

	CollisionManager collisionManager_;						//当たり判定マネージャー
	std::shared_ptr<Player>pPlayer_;						//プレイヤー
	std::shared_ptr<Enemy>pEnemy_;							//敵
	std::vector<std::shared_ptr<GameObject>>gameobjects_;	//ゲームオブジェクトの配列
};

