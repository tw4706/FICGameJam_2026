#include "Character.h"

Character::Character(Vector2 pos, Vector2 vel, float dir):
	GameObject(pos, vel, dir),
	hp_(0),
	gravity_(0.7f)
{
}

Character::~Character()
{
}
