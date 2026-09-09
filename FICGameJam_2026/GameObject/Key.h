#pragma once
#include "GameObject.h"
#include "../Animation.h"


class Key :public GameObject
{
public:
	Key(Vector2 pos, Vector2 vel, float dir, float width, float height);
	~Key();

	void Init()override;
	void Update()override;
	void Draw()override;

	void OnCollision(GameObject& other)override;

private:
	int animIndex_ = 0;
	int frameCount_ = 0;
	//‰æ‘œƒnƒ“ƒhƒ‹
	int handles_[4] = {-1,-1,-1,-1};
};

