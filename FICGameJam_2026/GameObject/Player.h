#pragma once
#include "Character.h"
class Player :public Character
{
public:
	Player(Vector2 pos, Vector2 vel, float dir,float width,float height);
	virtual ~Player();
	virtual void Init()override;
	virtual void Update()override;
	virtual void Draw()override;

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="other">当たったキャラクター</param>
	void OnCollision(Character& other)override;

private:
	int invisibleTimer_ = 0;	//無敵時間
	int handle_ = -1;			//ハンドル
	bool isInvisible_ = false;	//無敵状態かどうか
};

