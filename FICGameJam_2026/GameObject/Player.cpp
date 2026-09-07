#include "Player.h"
#include "../Game.h"
#include "GameObject.h"
#include "../Input.h"
#include<Dxlib.h>

namespace
{
	//移動速度
	constexpr float kSpeed = 2.0f;

	//プレイヤーのサイズのオフセット
	constexpr int kPlayerSizeOffset = 30;	

	//円の半径
	//マウスカーソルの半径
	constexpr float kCircleRadius = 100.0f;
}

Player::Player(Vector2 pos, Vector2 vel, float dir):
	Character(pos,vel,dir)
{
}

Player::~Player()
{
}

void Player::Init()
{
}

void Player::Update()
{
	//マウス座標の取得
	int mx, my;
	GetMousePoint(&mx, &my);
	//マウス座標をスクリーン座標に変換
	Vector2 mousePos = { (float)mx - Game::kScreenWidth / 2, (float)my - Game::kScreenHeight / 2 };

	//プレイヤーの座標をスクリーン座標に変換
	Vector2 toMousePos = mousePos - pos_;
	float dist = sqrtf(toMousePos.x * toMousePos.x + toMousePos.y * toMousePos	.y);

	//速度の初期化
	Vector2 moveVel = { 0.0f, 0.0f };

	//マウスカーソルの座標が円の半径内にある場合、移動速度をつける
	if (dist <= kCircleRadius && dist > 0.0f)
	{
		moveVel = toMousePos;
		moveVel.Normalize();
	}

	//移動速度の加算
	vel_ = moveVel * kSpeed;
	pos_ += vel_;
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
	DrawBox(pos_.x+Game::kScreenWidth/2, pos_.y + Game::kScreenHeight/2,
		pos_.x + Game::kScreenWidth / 2+ kPlayerSizeOffset, pos_.y+Game::kScreenHeight / 2+ kPlayerSizeOffset, 0xff0000, false);
#endif
}
