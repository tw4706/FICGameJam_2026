#include "Enemy.h"
#include "Player.h"
#include "../Game.h"
#include<Dxlib.h>

namespace
{
	//移動速度
	constexpr float kSpeed = 1.0f;

	//マウスカーソルの半径
	constexpr float kCircleRadius = 100.0f;

	//経路のウェイポイントに到達したとみなす距離
	constexpr float kWayPointThreshold = 5.0f;
}
Enemy::Enemy(Vector2 pos, Vector2 vel, float dir,float width,float height):
	Character(pos,vel,dir, width, height),
	handle_(-1)
{
	//プレイヤーの初期化
	pPlayer_ = std::weak_ptr<Player>();
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
}

void Enemy::Update()
{
	//プレイヤーの取得
	auto player = pPlayer_.lock();

	//プレイヤーが存在しない場合は何もしない
	if (!player)return;

	//マウス座標の取得
	int mx, my;
	GetMousePoint(&mx, &my);
	Vector2 mousePos = { (float)mx, (float)my };

	//敵とマウスカーソルの距離
	Vector2 toMouse = mousePos - pos_;
	float distToMouse = sqrtf(toMouse.x * toMouse.x + toMouse.y * toMouse.y);

	//マウスカーソルの円の中に入っている場合は停止
	if (distToMouse <= kCircleRadius)
	{
		vel_ = Vector2(0.0f, 0.0f);
		return;
	}

	//経路がない、または経路のインデックスが範囲外の場合は何もしない
	if (path_.empty() || pathIndex_ >= path_.size())
	{
		return;
	}

	//現在向かうべきウェイポイント
	Vector2 targetPos = path_[pathIndex_];
	Vector2 toTarget = targetPos - pos_;
	float distToTarget = toTarget.Length();

	//ウェイポイントに閾値まで近づいたら次のウェイポイントへ進める
	if (distToTarget <= kWayPointThreshold)
	{
		pathIndex_++;
		if (pathIndex_ >= path_.size())
		{
			vel_ = Vector2(0.0f, 0.0f);
			return;
		}
		targetPos = path_[pathIndex_];
		toTarget = targetPos - pos_;
		distToTarget = toTarget.Length();
	}

	//ウェイポイントに向かって移動する
	if (distToTarget > 0.0f)
	{
		toTarget.Normalize();
		vel_ = toTarget * kSpeed;
	}
	else
	{
		vel_ = Vector2(0.0f, 0.0f);
	}
	pos_ += vel_;

	//コライダー座標の更新
	collider_.SetPos(pos_);
}

void Enemy::Draw()
{
#ifdef _DEBUG
	//デバッグ用プレイヤー表示
	float halfW = collider_.GetWidth() / 2.0f;
	float halfH = collider_.GetHeight() / 2.0f;
	DrawBox(pos_.x - halfW, pos_.y - halfH, pos_.x + halfW, pos_.y + halfH, 0xffff00, false);

	//デバッグ用:経路の可視化
	for (size_t i = 0; i + 1 < path_.size(); ++i)
	{
		DrawLine(
			static_cast<int>(path_[i].x), static_cast<int>(path_[i].y),
			static_cast<int>(path_[i + 1].x), static_cast<int>(path_[i + 1].y),
			0x00ff00);
	}
#endif
}
