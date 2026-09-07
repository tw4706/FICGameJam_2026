#include "Character.h"
#include "../CharacterStateBase.h"

Character::Character(Vector2 pos, Vector2 vel, float dir):
	hp_(0),
	gravity_(0.7f)
{
}

Character::~Character()
{
}

void Character::ChangeState(std::shared_ptr<CharacterStateBase> pNextState)
{
	if (!pNextState) return;

	//ステートが残っている際はそのステートのExitを呼ぶ
	if (pCurrentState_)
	{
		pCurrentState_->Exit();
	}

	//次のステートを更新
	pCurrentState_ = pNextState;

	//初期化処理を行う
	pCurrentState_->Enter();
}
