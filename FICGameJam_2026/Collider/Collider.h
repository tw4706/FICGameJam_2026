#pragma once
#include"../Vector2.h"
class Collider
{
public:
	Collider(const Vector2& pos);
	virtual ~Collider() = default;

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(const Vector2& pos) { pos_ = pos; }

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標を返す</returns>
	const Vector2& GetPos() const { return pos_; }
	
private:
	Vector2 pos_;	//中心座標
};

