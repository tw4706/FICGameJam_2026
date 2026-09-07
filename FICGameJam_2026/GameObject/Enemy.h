#pragma once
#include "Character.h"

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

private:
	int handle_ = -1;	//ハンドル
	std::weak_ptr<Player>pPlayer_;
};

