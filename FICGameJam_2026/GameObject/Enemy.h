#pragma once
#include "Character.h"
#include "../PathFinder.h"
#include<vector>
#include<memory>

class Player;
class Enemy :public Character
{
public:
	Enemy(Vector2 pos, Vector2 vel, float dir, float width, float height);
	virtual ~Enemy();
	virtual void Init()override;
	virtual void Update()override;
	virtual void Draw()override;

	void ChangeState(AnimState state)override;

	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	void SetPlayer(std::shared_ptr<Player> player) { pPlayer_ = player; }

	/// <summary>
	/// パスの設定
	/// </summary>
	/// <param name="path">パスの位置</param>
	void SetPath(const std::vector<Vector2>& path) { path_ = path; pathIndex_ = 0; }

private:
	int handle_ = -1;	//ハンドル
	int runHandle_ =-1;	//Run状態のハンドル
	int deathHandle_ =-1;	//Death状態のハンドル
	int inCircleTimer_ = 0;//サークルに入っている時間を図るタイマー
	bool isDying_ = false;	//死亡中かどうか

	std::weak_ptr<Player>pPlayer_;
	std::vector<Vector2>path_;
	size_t pathIndex_ = 0;
};

