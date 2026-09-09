#include "EffectManager.h"
#include <Dxlib.h>
#include <cassert>

EffectManager& EffectManager::GetInstance()
{
	static EffectManager instance;
	return instance;
}

EffectManager::~EffectManager()
{
	Clear();
}

void EffectManager::Update()
{
	for (auto it = handles_.begin(); it != handles_.end();)
	{
		it->second->Update();
		if (it->second->IsDead())
		{
			it = handles_.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void EffectManager::Draw()
{
	for (auto& pair : handles_)
	{
		pair.second->Draw();
	}
}

void EffectManager::Load(const std::wstring& name, const std::wstring& path,
	const Vector2& frameSize, int maxAnimNum, int oneAnimFrame, float scale)
{
	//すでにロードされていたら何もしない
	if (effects_.count(name) > 0)
	{
		return;
	}
	int graphHandle = LoadGraph(path.c_str());
	assert(graphHandle >= 0 && "エフェクトのロードに失敗しました");
	effects_.emplace(name, EffectData{ graphHandle, frameSize, maxAnimNum, oneAnimFrame, scale });
}

int EffectManager::Play(const std::wstring& name, const Vector2& pos)
{
	auto it = effects_.find(name);
	if (it == effects_.end())
	{
		return -1;
	}
	const auto& data = it->second;
	int handle = nextEffectHandle_++;
	handles_[handle] = std::make_unique<Effect>(
		pos, data.graphHandle, data.frameSize, data.maxAnimNum, data.oneAnimFrame, data.scale);
	return handle;
}

bool EffectManager::IsPlaying(int handle) const
{
	auto it = handles_.find(handle);
	if (it == handles_.end())
	{
		return false;
	}
	return !it->second->IsDead();
}

void EffectManager::Stop(int handle)
{
	handles_.erase(handle);
}

void EffectManager::StopAll()
{
	handles_.clear();
}

void EffectManager::Clear()
{
	StopAll();
	for (auto& pair : effects_)
	{
		DeleteGraph(pair.second.graphHandle);
	}
	effects_.clear();
}
