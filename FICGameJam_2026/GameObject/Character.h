#pragma once
#include"GameObject.h"
#include"../Vector2.h"
#include"../Animation.h"
#include <memory>

class Character :public GameObject
{
public:
	//アニメーションの状態
	enum AnimState
	{
		Idle,
		Run,
		Death,
		Clear
	};

	Character(Vector2 pos, Vector2 vel, float dir,float width,float height);
	virtual ~Character();

	virtual void Init()abstract;
	virtual void Update()abstract;
	virtual void Draw()abstract;

	/// <summary>
	/// 状態の遷移
	/// </summary>
	/// <param name="state">アニメーションのステート</param>
	virtual void ChangeState(AnimState state) {};

protected:
	int hp_;					//HP

	Animation animation_;		//アニメーション
	AnimState state_;			//アニメーションステート
};

