#include "Character.h"
#include<Dxlib.h>

Character::Character(Vector2 pos, Vector2 vel, float dir, float width, float height):
	GameObject(pos, vel, dir),
	hp_(0),
	gravity_(0.7f),
	collider_(pos, width, height)
{
}

Character::~Character()
{
}

void Character::OnCollision(Character& other)
{
#ifdef _DEBUG
	//衝突したらデバッグ表示
	printfDx(L"当たった\n");
#endif
}
