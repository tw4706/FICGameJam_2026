#pragma once
#include "Character.h"
class Player :public Character
{
public:
	Player(Vector2 pos, Vector2 vel, float dir);
	virtual ~Player();
	virtual void Init()override;
	virtual void Update()override;
	virtual void Draw()override;

private:
	int handle_ = -1;	//ƒnƒ“ƒhƒ‹
};

