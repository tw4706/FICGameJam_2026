#pragma once
#include "GameObject.h"
#include "../Vector2.h"
class Goal :public GameObject
{
public:
	Goal(Vector2 pos, Vector2 vel, float dir, int width, int height);
	virtual~Goal();

	void Init()override;
	void Update()override;
	void Draw()override;

private:
	int handle_;	//ƒnƒ“ƒhƒ‹
};

