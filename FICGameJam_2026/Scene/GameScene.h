#pragma once
#include"Scene.h"

class Player;
class GameScene:public Scene
{
public:
	GameScene(SceneManager& sceneManager);
	~GameScene();

	void Init()override;
	void Update();
	void Draw();

	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpdate();
	using UpdateFunc_t = void (GameScene::*)();
	UpdateFunc_t update_;

	void FadeDraw();
	void NormalDraw();
	using DrawFunc_t = void (GameScene::*)();
	DrawFunc_t draw_;

private:
	int frameCount_ = 0;

	std::shared_ptr<Player>pPlayer_;	//ÉvÉåÉCÉÑÅ[
};

