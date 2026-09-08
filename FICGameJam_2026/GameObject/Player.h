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

	void ChangeState(AnimState state)override;

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="other">当たったキャラクター</param>
	void OnCollision(GameObject& other)override;

private:
	int handle_ = -1;			//ハンドル
	int runHandle_ =- 1;			//Run状態のハンドル
	int invisibleTimer_ = 0;	//無敵時間
	bool isInvisible_ = false;	//無敵状態かどうか
};

