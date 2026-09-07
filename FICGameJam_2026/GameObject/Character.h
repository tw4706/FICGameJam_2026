#pragma once
#include"../Vector2.h"
#include <memory>

class CharacterStateBase;
class Character :  public std::enable_shared_from_this<Character>
{
public:
	Character(Vector2 pos, Vector2 vel, float dir);
	virtual ~Character();

	virtual void Init()abstract;
	virtual void Update()abstract;
	virtual void Draw()abstract;

	/// <summary>
	/// 状態の切り替え
	/// </summary>
	void ChangeState(std::shared_ptr<CharacterStateBase> pNextState);

	/// <summary>
	/// 重力の取得
	/// </summary>
	/// <returns>重力を返す</returns>
	float GetGravity() const { return gravity_; }

protected:
	int hp_;
	float gravity_ = 0.0f;											//重力の強さ
	std::shared_ptr<CharacterStateBase> pCurrentState_ = nullptr;	//現在のステートを管理するポインタ
};

