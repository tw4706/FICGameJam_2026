#include "SoundManager.h"
#include<Dxlib.h>

namespace
{
	constexpr int kBaseBgmVolume = 160;
	constexpr int kBaseSeVolume = 220;

	//BGMのフェードスピード
	constexpr float kBGMFadeSpeed = 0.05f;

	//各BGM・SEのパス
	const wchar_t* kTitleBgm = L"data/BGM・SE/TitleBGM.mp3";
	const wchar_t* kGameBgm = L"data/BGM・SE/GameBGM.mp3";
	const wchar_t* kResultBgm = L"data/BGM・SE/ResultBGM.mp3";

	const wchar_t* kDecideSe = L"data/BGM・SE/Decide.mp3";
	const wchar_t* kPlayerHitSe = L"data/BGM・SE/PlayerHit.mp3";
	const wchar_t* kDeathSe = L"data/BGM・SE/Explosion.mp3";
	const wchar_t* kEnemyDeathSe = L"data/BGM・SE/Death.mp3";
	const wchar_t* kCursoleMoveSe = L"data/BGM・SE/CursoleMove.mp3";
	const wchar_t* kGetKeySe = L"data/BGM・SE/Key.mp3";
	const wchar_t* kOpenTreasureSe = L"data/BGM・SE/OpenTakarabako.mp3";
	const wchar_t* kSpawnSe = L"data/BGM・SE/Spawn.mp3";
}

SoundManager& SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}

SoundManager::SoundManager() :
	currentSeHandle_(-1),
	currentBgmHandle_(-1)
{
}

SoundManager::~SoundManager()
{
	//BGMハンドルの削除
	for (auto bgm : bgmHandles_)
	{
		DeleteSoundMem(bgm.second);
	}

	//SEハンドルの削除
	for (auto se : seHandles_)
	{
		DeleteSoundMem(se.second);
	}
}

void SoundManager::Init()
{
	//音量の設定
	seVolume_ = kBaseSeVolume;
	bgmVolume_ = kBaseBgmVolume;

	//各BGM・SEの読み込み
	bgmHandles_[BGM::Title] = LoadSoundMem(kTitleBgm);
	bgmHandles_[BGM::Game] = LoadSoundMem(kGameBgm);
	bgmHandles_[BGM::Result] = LoadSoundMem(kResultBgm);

	seHandles_[SE::Decide] = LoadSoundMem(kDecideSe);
	seHandles_[SE::PlayerHit] = LoadSoundMem(kPlayerHitSe);
	seHandles_[SE::Death] = LoadSoundMem(kDeathSe);
	seHandles_[SE::EnemyDeath] = LoadSoundMem(kEnemyDeathSe);
	seHandles_[SE::CursoleMove] = LoadSoundMem(kCursoleMoveSe);
	seHandles_[SE::GetKey] = LoadSoundMem(kGetKeySe);
	seHandles_[SE::OpenTreasure] = LoadSoundMem(kOpenTreasureSe);
	seHandles_[SE::Spawn] = LoadSoundMem(kSpawnSe);
}

void SoundManager::Update()
{
	if (state_ == BGMState::None) return;

	if (state_ == BGMState::FadeOut)
	{
		fadeBGMVolume_ -= kBGMFadeSpeed;

		if (fadeBGMVolume_ <= 0.0f)
		{
			fadeBGMVolume_ = 0.0f;

			//現在のBGMを止めて次のBGMに切り替え
			StopSoundMem(currentBgmHandle_);
			currentBgmHandle_ = bgmHandles_[nextBgmState_];
			PlaySoundMem(currentBgmHandle_, DX_PLAYTYPE_LOOP);

			state_ = BGMState::FadeIn;
		}
	}
	else if (state_ == BGMState::FadeIn)
	{
		fadeBGMVolume_ += kBGMFadeSpeed;

		if (fadeBGMVolume_ >= 1.0f)
		{
			fadeBGMVolume_ = 1.0f;
			state_ = BGMState::None;
		}
	}

	//実際の音量に反映(bgmVolume_は元々の基準音量)
	ChangeVolumeSoundMem(static_cast<int>(bgmVolume_ * fadeBGMVolume_), currentBgmHandle_);
}

void SoundManager::PlaySe(SE se)
{
	int handle = seHandles_[se];
	currentSeHandle_ = handle;

	//SEの音量
	ChangeVolumeSoundMem(seVolume_, handle);

	//SEの再生	
	PlaySoundMem(handle, DX_PLAYTYPE_BACK, TRUE);
}

void SoundManager::PlayBgm(BGM bgm)
{
	int handle = bgmHandles_[bgm];

	//既に同じBGMが再生中、または既にそのBGMへフェード中なら何もしない
	if (currentBgmHandle_ == handle || (state_ != BGMState::None && nextBgmState_ == bgm))
	{
		return;
	}

	//再生中のBGMが無ければ即フェードインだけでよい
	if (currentBgmHandle_ == -1)
	{
		currentBgmHandle_ = handle;
		fadeBGMVolume_ = 0.0f;
		state_ = BGMState::FadeIn;
		PlaySoundMem(currentBgmHandle_, DX_PLAYTYPE_LOOP);
		return;
	}

	//次に再生するBGMを予約し、フェードアウトから開始
	nextBgmState_ = bgm;
	state_ = BGMState::FadeOut;
}

void SoundManager::StopBgm()
{
	if (currentBgmHandle_ == -1) return;

	//BGMの停止
	StopSoundMem(currentBgmHandle_);
	state_ = BGMState::None;
	currentBgmHandle_ = -1;
}

void SoundManager::StopSe()
{
	if (currentSeHandle_ == -1) return;

	//SEの停止
	StopSoundMem(currentSeHandle_);
}
