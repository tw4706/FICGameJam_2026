#include "Player.h"
#include "../Game.h"
#include "GameObject.h"
#include "../Input.h"
#include"../Collider/RectCollider.h"
#include<Dxlib.h>

namespace
{
	//円の半径
	//マウスカーソルの半径
	constexpr float kCircleRadius = 100.0f;

	//線形補間の割合
	constexpr float kLerpRate = 0.06f;

	//最大体力
	constexpr int kMaxHP = 3;

	//無敵時間
	constexpr int kInvisibleTime = 30;
}

Player::Player(Vector2 pos, Vector2 vel, float dir, float width, float height) :
	Character(pos, vel, dir, width, height),
	handle_(-1)
{
}

Player::~Player()
{
}

void Player::Init()
{
	hp_ = kMaxHP;
}

void Player::Update()
{
	//無敵時間のカウント
	invisibleTimer_--;

	//無敵時間が0以下になったら無敵を解除
	if(invisibleTimer_ <= 0)
	{
		isInvisible_ = false;
	}

	//マウス座標の取得
	int mx, my;
	GetMousePoint(&mx, &my);
	//マウス座標をスクリーン座標に変換
	Vector2 mousePos = { static_cast<float>(mx), static_cast<float>(my) };

	//プレイヤーの座標をスクリーン座標に変換
	Vector2 toMousePos = mousePos - pos_;
	float dist = sqrtf(toMousePos.x * toMousePos.x + toMousePos.y * toMousePos.y);

	//速度の初期化
	Vector2 moveVel = { 0.0f, 0.0f };

	//マウスカーソルの座標が円の半径内にある場合、移動速度をつける
	if (dist <= kCircleRadius && dist > 0.0f)
	{
		pos_.x = pos_.Lerp(pos_.x, mousePos.x, kLerpRate);
		pos_.y = pos_.Lerp(pos_.y, mousePos.y, kLerpRate);
	}

	//コライダー座標の更新
	collider_.SetPos(pos_);
}

void Player::Draw()
{
	//マウス座標の取得
	int mx, my;
	GetMousePoint(&mx, &my);
	//マウスカーソルの描画
	DrawCircle(mx, my, (int)kCircleRadius, 0x00ff00, false);

#ifdef _DEBUG
	//デバッグ用プレイヤー表示
	float halfW = collider_.GetWidth() / 2.0f;
	float halfH = collider_.GetHeight() / 2.0f;
	DrawBox(pos_.x - halfW, pos_.y - halfH, pos_.x + halfW, pos_.y + halfH, 0xff0000, false);
#endif
}

void Player::OnCollision(Character& other)
{
	if (isInvisible_)return;

	//当たったら体力を減らす
	hp_--;

	//無敵
	isInvisible_ = true;
	invisibleTimer_ = kInvisibleTime;

	//hpが0以下になったら削除
	if (hp_ <= 0)
	{
		Destroy();
	}
}
