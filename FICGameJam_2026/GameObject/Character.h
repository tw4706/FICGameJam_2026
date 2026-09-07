#pragma once
#include"GameObject.h"
#include"../Vector2.h"
#include <memory>

class CharacterStateBase;
class Character :public GameObject
{
public:
	Character(Vector2 pos, Vector2 vel, float dir);
	virtual ~Character();

	virtual void Init()abstract;
	virtual void Update()abstract;
	virtual void Draw()abstract;

	/// <summary>
	/// d—Í‚Ìæ“¾
	/// </summary>
	/// <returns>d—Í‚ğ•Ô‚·</returns>
	float GetGravity() const { return gravity_; }

protected:
	int hp_;
	float gravity_ = 0.0f;											//d—Í‚Ì‹­‚³
};

