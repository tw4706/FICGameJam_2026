#pragma once
#include "Effect.h"

class DeathEffect :public Effect
{
public:
	~DeathEffect();

	void Load();
private:
	int handle_;
};

