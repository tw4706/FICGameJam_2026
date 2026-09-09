#include "DeathEffect.h"
#include<Dxlib.h>

namespace
{
    //コマ数
    constexpr int kMaxAnimNum = 7;

    //1コマの幅
    constexpr float kFrameWidth = 192.0f;  

    //1コマの高さ
    constexpr float kFrameHeight = 192.0f; 

    //1コマあたりのフレーム数
    constexpr int kOneAnimFrame = 4;       
    constexpr float kScale = 1.0f;

    //画像のパス
    const wchar_t* kDeathEffectPath = L"data/deathEffect.png";
}

DeathEffect::~DeathEffect()
{
    DeleteGraph(handle_);
}

void DeathEffect::Load()
{
    handle_ = LoadGraph(kDeathEffectPath);
}
