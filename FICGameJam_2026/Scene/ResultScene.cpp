#include "ResultScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "../Game.h"
#include "../Input.h"
#include<Dxlib.h>
#include<memory>
#include<cmath>
#include<algorithm>
#include<cassert>

namespace
{
	//フェードの間隔
	constexpr int kFadeInterval = 60;
}

ResultScene::ResultScene(SceneManager& sceneManager) :
	Scene(sceneManager),
	update_(&ResultScene::FadeInUpdate),
	draw_(&ResultScene::FadeDraw),
	frameCount_(kFadeInterval)
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Init()
{
	SetMouseDispFlag(true);
}

void ResultScene::Update()
{
	(this->*update_)();
}

void ResultScene::Draw()
{
	(this->*draw_)();
}

void ResultScene::FadeInUpdate()
{
	frameCount_--;

	if (frameCount_ <= 0)
	{
		update_ = &ResultScene::NormalUpdate;
		draw_ = &ResultScene::NormalDraw;
		return;
	}
}

void ResultScene::NormalUpdate()
{
	if (Input::GetInstance().IsPressed("next"))
	{
		update_ = &ResultScene::FadeOutUpdate;
		draw_ = &ResultScene::FadeDraw;
		frameCount_ = kFadeInterval;
	}
}

void ResultScene::FadeOutUpdate()
{
	frameCount_--;

	if (frameCount_ < 0)
	{
		sceneManager_.ChangeScene(std::make_shared<TitleScene>(sceneManager_));
		return;
	}
}

void ResultScene::FadeDraw()
{
	float rate;

	if (update_ == &ResultScene::FadeInUpdate)
	{
		//フェードイン
		rate = 1.0f - (float)frameCount_ / kFadeInterval;
	}
	else
	{
		//フェードアウト
		rate = (float)frameCount_ / kFadeInterval;
	}

	rate = std::clamp(rate, 0.0f, 1.0f);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * rate));
	NormalDraw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ResultScene::NormalDraw()
{
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x00ced1, true);
#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, L"リザルトシーン");
#endif
}
