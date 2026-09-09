#pragma once
#include "GameObject.h"
#include "../Animation.h"

class Chest :public GameObject
{
public:
	enum ChestContents
	{
		Key,
		Enemy
	};
	Chest(const Vector2& pos, ChestContents contents);
	~Chest();

	void Init() override;
	void Update() override;
	void Draw() override;

	//宝箱を開ける
	void Open();

	//開けたかどうか
	bool IsOpened() const { return isOpened_; }

	//中身の取得
	ChestContents GetContent() const { return content_; }

	//開けた瞬間に一度だけtrueを返す
	bool IsOpenEvent();

private:
	ChestContents content_;			//宝箱の中身
	bool isOpened_ = false;			//開けたかどうか
	bool isOpenEvent_ = true;		//寳保子を開けてイベントが起こるかどうか

	int handle_ = -1;
	Animation anim_;
};

