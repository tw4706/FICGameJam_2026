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

	//プレイヤーとの差分ベクトル
	Vector2 toPlayer = player->GetPos()-pos_ ;

	float distance = toPlayer.Length();

	//プレイヤーに向かって移動する
	if(distance > 0.0f)
	{
		//差分ベクトルの正規化
		toPlayer.Normalize();

		//敵の移動速度を設定
		vel_ = toPlayer * kSpeed;
	}
	else
	{
		//プレイヤーと同じ位置にいる場合は停止
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
#endif
}
