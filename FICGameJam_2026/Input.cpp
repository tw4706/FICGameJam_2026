#include "Input.h"
#include<Dxlib.h>

Input& Input::GetInstance()
{
	static Input instance;
	return instance;
}

Input::Input() :inputData_{}, lastInputData_{}, inputTable_{}
{
	inputTable_["up"] = { {PeripheralType::keyboard,KEY_INPUT_UP},
						{PeripheralType::padXInput,XINPUT_BUTTON_DPAD_DOWN} };
	inputTable_["down"] = { {PeripheralType::keyboard,KEY_INPUT_DOWN},
						{PeripheralType::padXInput,XINPUT_BUTTON_DPAD_UP} };
	inputTable_["left"] = { {PeripheralType::keyboard,KEY_INPUT_LEFT},
						{PeripheralType::padXInput,XINPUT_BUTTON_DPAD_LEFT} };
	inputTable_["right"] = { {PeripheralType::keyboard,KEY_INPUT_RIGHT},
						{PeripheralType::padXInput,XINPUT_BUTTON_DPAD_RIGHT} };
	inputTable_["next"] = { {PeripheralType::keyboard,KEY_INPUT_RETURN},
						{PeripheralType::padXInput,XINPUT_BUTTON_A} };

	//変な値が入らないように枠を開けておく
	for (const auto& input : inputTable_)
	{
		inputData_[input.first] = false;
		lastInputData_[input.first] = false;
	}
}

void Input::Update()
{
	//入力情報の取得
	char keyState[256];
	int padState = GetJoypadInputState(DX_INPUT_PAD1);
	GetHitKeyStateAll(keyState);
	lastInputData_ = inputData_;

	//パッドが接続できているかどうか
	isXInputConnected_ = (GetJoypadXInputState(DX_INPUT_PAD1, &xInputState_) == 0);

	//すべての入力イベントをチェック
	for (const auto& inputInfo : inputTable_)
	{
		auto& input = inputData_[inputInfo.first];
		for (const auto& state : inputInfo.second)
		{
			switch (state.type)
			{
			case PeripheralType::keyboard:
				input = keyState[state.id];
				break;
			case PeripheralType::padXInput:
				if (isXInputConnected_)
				{
					input = (xInputState_.Buttons[state.id] != 0);
				}
				break;
			}
			if (input)
			{
				//必須!
				break;
			}
		}
	}
}

bool Input::IsPressed(const char* name) const
{
	return inputData_.at(name);
}

bool Input::IsTriggered(const char* name) const
{
	return inputData_.at(name) && !lastInputData_.at(name);
}

bool Input::IsReleased(const char* name) const
{
	return !inputData_.at(name) && lastInputData_.at(name);
}
