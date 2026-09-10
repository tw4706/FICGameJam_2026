#include "Application.h"
#include "Game.h"
#include "Input.h"
#include "Scene/TitleScene.h"
#include "Scene/GameScene.h"
#include "Scene/SceneManager.h"
#include "../EffectManager.h"
#include "../SoundManager.h"
#include<Dxlib.h>
#include<memory>

Application::Application()
{
}

Application::~Application()
{
}

Application& Application::GetInstance()
{
	static Application instance;
	return instance;
}

bool Application::Init()
{
	//ウィンドウモード設定
	ChangeWindowMode(true);

	//ウィンドウのタイトル変更
	SetMainWindowText(L"リミテッドエスケープ");

	//画面のサイズ変更
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);

	if (DxLib_Init() == -1)		//ＤＸライブラリ初期化処理
	{
		return -1;				//エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	AddFontResourceEx(L"data/Font/Corporate-Logo-Rounded-Bold-ver3.otf", FR_PRIVATE, NULL);
	Game::kFontUIHandle = CreateFontToHandle(L"コーポレート・ロゴ（ラウンド）ver3 Bold", 40, -1);

	//サウンドマネージャーの初期化
	SoundManager::GetInstance().Init();

	return true;
}

void Application::Run()
{
	//シーンの作成
	SceneManager sceneManager;

	//シーンの切り替え
	sceneManager.ChangeScene(std::make_shared<TitleScene>(sceneManager));

#ifdef _DEBUG
	//デバッグ時のFPS表示用変数
	int nowFps = 0;
#endif
	while (ProcessMessage() != -1)
	{
		LONGLONG start = GetNowHiPerformanceCount();

		//前のフレーム描画を削除
		ClearDrawScreen();

		Input::GetInstance().Update();
		//シーンの更新と描画
		sceneManager.Update();
		sceneManager.Draw();

		//サウンドの更新
		SoundManager::GetInstance().Update();

		//エフェクトの更新と描画
		EffectManager::GetInstance().Update();
		EffectManager::GetInstance().Draw();

		//FPSの表示
#ifdef _DEBUG
		DrawFormatString(0, 30, GetColor(255, 255, 0), L"FPS:%d", nowFps);
#endif

		if (isGameEnd_||CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		//画面に反映
		ScreenFlip();

		//ゲーム速度がどのPCでも同じスピードになる
		while (GetNowHiPerformanceCount() - start < 16667)
		{

		}
#ifdef _DEBUG
		//実測フレーム時間からFPSを計算
		LONGLONG frameTime = GetNowHiPerformanceCount() - start;
		if (frameTime > 0)
		{
			nowFps = static_cast<int>(1000000LL / frameTime);
		}
#endif
	}
}

void Application::Terminate()
{
	//メモリの開放
	RemoveFontResourceEx(L"data/Font/Corporate-Logo-Rounded-Bold-ver3.otf", FR_PRIVATE, NULL);
	DxLib_End();				//ＤＸライブラリ使用の終了処理
}
