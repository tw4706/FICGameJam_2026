#pragma once
#include"../Vector2.h"
#include"../Collider/RectCollider.h"

class Collider;
class Input;
class GameObject
{
public:
	GameObject(Vector2 pos, Vector2 vel, float dir,int width,int height);
	virtual~GameObject();

	virtual void Init()abstract;
	virtual void Update()abstract;
	virtual void Draw()abstract;

	/// <summary>
	/// 死亡フラグ制御
	/// </summary>
	/// <returns>死んでいるかどうか</returns>
	bool IsDead()const { return isDead_; }

	/// <summary>
	/// 削除処理
	/// </summary>
	void Destroy() { isDead_ = true; }

	///ゲッター
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標の参照</returns>
	const Vector2& GetPos() const { return pos_; }

	/// <summary>
	/// 位置の設定
	/// </summary>
	/// <param name="pos">設定する位置</param>
	void SetPos(const Vector2& pos) { pos_ = pos; }

	/// <summary>
	/// 速度の取得
	/// </summary>
	/// <returns>速度</returns>
	Vector2 GetVelocity() const { return vel_; }

	///セッター
	/// <summary>
	/// 速度のセット
	/// </summary>
	/// <param name="vel">速度</param>
	void SetVelocity(const Vector2& vel) { vel_ = vel; }

	/// <summary>
	/// 矩形の当たり判定の取得
	/// </summary>
	/// <returns>コライダーのポインタを返す</returns>
	RectCollider& GetCollider() { return collider_; }

	/// <summary>
	/// 衝突判定処理
	/// </summary>
	/// <param name="other">当たったキャラクター</param>
	virtual void OnCollision(GameObject& other);

	/// <summary>
	/// 押し戻し量を受け取り座標を修正
	/// </summary>
	/// <param name="pushVector">押し出しのベクトル量</param>
	void AdjustPosition(const Vector2& pushVector) { pos_ += pushVector; collider_.SetPos(pos_); }

protected:
	Vector2 pos_;	//座標
	Vector2 vel_;	//速度
	float dir_;		//向き
	bool isDead_ = false;	//死亡フラグ

	RectCollider collider_;		//矩形のコライダー
};

