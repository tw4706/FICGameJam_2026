#pragma once
#include "GameObject.h"
#include"../Animation.h"

class Effect :public GameObject
{
public:
    Effect(const Vector2& pos, int drawHandle, Vector2 frameSize,
        int maxAnimNum, int oneAnimFrame, float scale = 1.0f);
    ~Effect();

    void Init() override {};
    void Update() override;
    void Draw() override;

protected:
    Animation animation_;
};