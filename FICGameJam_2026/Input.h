#pragma once
#include<array>
#include<map>
#include<string>
#include<vector>
#include<Dxlib.h>

/// <summary>
/// 周辺機器種別
/// </summary>
enum class PeripheralType
{
	keyboard,
	padXInput,
};

struct InputState
{
	PeripheralType type;	//入力された機器の種別
	int id;					//入力情報が入る(キーボードの場合はインデックス、パッドの場合はビット)
};

class Input
{
public:

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	/// <returns>インスタンス</returns>
	static Input& GetInstance();

private:
	//初期化処理
	Input();

	//コピー禁止
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

public:
	//更新処理
	void Update();
	//ボタンが押されたか
	bool IsPressed(const char* name)const;

	//ボタンが今押されたかどうか
	bool IsTriggered(const char* name)const;

	//ボタンが離されたかどうか
	bool IsReleased(const char* name)const;
private:
	std::map<std::string, std::vector<InputState>>inputTable_;	//イベント名と実際の入力の対応表
	std::map<std::string, bool>inputData_;						//実際に入力されたかどうかのデータ
	std::map<std::string, bool>lastInputData_;					//最後に入力されたかどうかのデータ

	//XInputの取得
	XINPUT_STATE xInputState_={};
	//パッドが接続できているかどうか
	bool isXInputConnected_ =false;
};

