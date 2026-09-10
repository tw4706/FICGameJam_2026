#include "Input.h"
#include "Game.h"
#include<Dxlib.h>
#include<algorithm>

namespace
{	
	//スティックのデッドゾーン
	constexpr float kStickDeadZone = 8000.0f;

	//仮想カーソルの移動速度
	constexpr float kCursorSpeed = 7.0f;

}

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
	inputTable_["open"] = { {PeripheralType::padXInput,XINPUT_BUTTON_A},
						{PeripheralType::mouse,MOUSE_INPUT_RIGHT} };

	inputTable_["click"] = { {PeripheralType::mouse, MOUSE_INPUT_LEFT} };

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

	//マウス座標とマウス入力状態の取得
	//前フレームのマウス座標を代入して更新
	prevMouseX_ = mouseX_;
	prevMouseY_ = mouseY_;
	GetMousePoint(&mouseX_, &mouseY_);
	int mouseInput = GetMouseInput();

	//実マウスが動いていたら仮想座標をマウスに同期
	bool mouseMoved = (mouseX_ != prevMouseX_ || mouseY_ != prevMouseY_);
	if (mouseMoved)
	{
		virtualMouseX_ = static_cast<float>(mouseX_);
		virtualMouseY_ = static_cast<float>(mouseY_);
	}
	//マウスが動いていなければスティック入力で仮想座標を移動
	else if (isXInputConnected_)
	{
		float lx = static_cast<float>(xInputState_.ThumbLX);
		float ly = static_cast<float>(xInputState_.ThumbLY);

		if (fabsf(lx) > kStickDeadZone)
		{
			virtualMouseX_ += (lx / 32768.0f) * kCursorSpeed;
		}
		if (fabsf(ly) > kStickDeadZone)
		{
			virtualMouseY_ -= (ly / 32768.0f) * kCursorSpeed;
		}
	}

	//画面外に出ないよう制限
	virtualMouseX_ = std::clamp(virtualMouseX_, 0.0f, static_cast<float>(Game::kScreenWidth));
	virtualMouseY_ = std::clamp(virtualMouseY_, 0.0f, static_cast<float>(Game::kScreenHeight));

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
				input = isXInputConnected_ && (xInputState_.Buttons[state.id] != 0);
				break;
			case PeripheralType::mouse:
				input = (mouseInput & state.id) != 0;
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
