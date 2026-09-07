#include "GameObject.h"


GameObject::GameObject(Vector2 pos, Vector2 vel,float dir) :
	pos_(pos),
	vel_(vel),
	dir_(dir),
	isDead_(false)
{
}

GameObject::~GameObject(){}

void GameObject::AddPosition()
{
	pos_ += vel_;
}
