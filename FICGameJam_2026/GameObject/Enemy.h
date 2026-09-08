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
	std::weak_ptr<Player>pPlayer_;

	std::vector<Vector2>path_;
	size_t pathIndex_ = 0;
};

