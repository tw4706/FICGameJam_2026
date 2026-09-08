#pragma once
#include "../Vector2.h"

class Animation
{
public:
	Animation();
	~Animation();

	void Init(int drawHandle,int animIndex, Vector2 frameSize,int maxAnimNum,int oneAnimFrame,float scale,bool isRepeat = true);	//普通のアニメーション初期化
	void Init(int drawHandle, int animIndexY, int animIndexX, Vector2 frameSize, float scale, bool isRepeat = true);				//一枚絵アニメーション初期化
	void Update();
	void Draw(Vector2 pos,bool isTurn);
	void Draw(int drawHandle,Vector2 pos, bool isTurn);

	void SetRotate(float rotate) { rotate_ = rotate; }
	void SetOffset(Vector2 offset) { drawOffset_ = offset; }

	//アニメーションを最初から再生する
	void SetFirst() { nowAnimNum_ = 0; frameCount_ = 0; }

	//アニメーションを最後のコマにする
	void SetEnd() { nowAnimNum_ = maxAnimNum_; }

	bool GetIsEnd();

	bool operator!=(const Animation& other) const;
private:
	bool isLoop_;		// 繰り返し再生するかどうか
	int drawHandle_;	// 画像ハンドル
	int animIndex_;		// アニメーションの種類番号
	Vector2 frameSize_;	// 画像の1コマサイズ
	int maxAnimNum_;	// アニメーションの最大コマ数
	int nowAnimNum_;	// 現在のアニメーション番号
	int oneAnimFrame_;	// 1コマあたりのフレーム数
	int frameCount_;	// アニメーション制御用
	float scale_;		// 拡大率
	float rotate_;		// 回転角
	Vector2 drawOffset_; // 描画位置オフセット
};