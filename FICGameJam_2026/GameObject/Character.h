#pragma once
#include"GameObject.h"
#include"../Vector2.h"
#include"../Collider/RectCollider.h"
#include <memory>

class RectCollider;
class Character :public GameObject
{
public:
	Character(Vector2 pos, Vector2 vel, float dir,float width,float height);
	virtual ~Character();

	virtual void Init()abstract;
	virtual void Update()abstract;
	virtual void Draw()abstract;

	/// <summary>
	/// 重力の取得
	/// </summary>
	/// <returns>重力を返す</returns>
	float GetGravity() const { return gravity_; }

	/// <summary>
	/// 矩形の当たり判定の取得
	/// </summary>
	/// <returns>コライダーのポインタを返す</returns>
	RectCollider& GetCollider() { return collider_; }

	/// <summary>
	/// 衝突判定処理
	/// </summary>
	/// <param name="other">当たったキャラクター</param>
	virtual void OnCollision(Character& other);

protected:
	int hp_;
	float gravity_ = 0.0f;		//重力の強さ
	RectCollider collider_;		//矩形のコライダー
};

