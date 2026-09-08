#include "Character.h"
#include<Dxlib.h>

Character::Character(Vector2 pos, Vector2 vel, float dir, float width, float height):
	GameObject(pos, vel, dir,width,height),
	hp_(0),
	state_(AnimState::Idle)
{
}

Character::~Character()
{
}