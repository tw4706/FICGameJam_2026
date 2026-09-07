#include "Player.h"
#include "../Game.h"
#include<Dxlib.h>

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
}

void Player::Draw()
{
#ifdef _DEBUG
	DrawBox(Game::kScreenWidth/2, Game::kScreenHeight/2, Game::kScreenWidth / 2+50, Game::kScreenHeight / 2+50, 0xff0000, false);
#endif
}
