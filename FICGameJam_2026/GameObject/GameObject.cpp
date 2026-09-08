#include "GameObject.h"
#include<Dxlib.h>


GameObject::GameObject(Vector2 pos, Vector2 vel,float dir,int width,int height) :
	pos_(pos),
	vel_(vel),
	dir_(dir),
	collider_(pos, width, height),
	isDead_(false)
{
}

GameObject::~GameObject(){}


void GameObject::OnCollision(GameObject& other)
{
#ifdef _DEBUG
	//衝突したらデバッグ表示
	printfDx(L"当たった\n");
#endif
}
