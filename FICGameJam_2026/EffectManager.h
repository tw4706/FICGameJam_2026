#pragma once
#include"../Vector2.h"
#include"../GameObject/Effect.h"
#include <string>
#include <vector>
#include <memory>
#include <map>

class EffectManager
{
public:
	static EffectManager& GetInstance();

	void Update();

	void Draw();

	/// <summary>
	/// エフェクトのロード
	/// </summary>
	/// <param name="name">エフェクトの文字列</param>
	/// <param name="path">エフェクトのファイルパス</param>
	/// <param name="frameSize">フレームサイズ</param>
	/// <param name="maxAnimNum">最大アニメーション数</param>
	/// <param name="oneAnimFrame">1つのコマ数</param>
	/// <param name="scale">拡大率</param>
	void Load(const std::wstring& name, const std::wstring& path,
		const Vector2& frameSize, int maxAnimNum, int oneAnimFrame, float scale = 1.0f);

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	/// <param name="name">エフェクトの名前</param>
	/// <param name="pos">エフェクトを生成する座標</param>
	/// <returns>ハンドルを返す</returns>
	int Play(const std::wstring& name, const Vector2& pos);

	/// <summary>
	/// エフェクトが再生中かどうか
	/// </summary>
	/// <param name="handle">ハンドル</param>
	/// <returns>再生中ならtrue,そうでない場合はfalse</returns>
	bool IsPlaying(int handle)const;

	/// <summary>
	/// エフェクトの停止
	/// </summary>
	/// <param name="handle">ハンドル</param>
	void Stop(int handle);

	/// <summary>
	/// すべてのエフェクトの停止
	/// </summary>
	void StopAll();

	/// <summary>
	/// 使い終わったエフェクトの解放を行う
	/// </summary>
	void Clear();

private:
	EffectManager() = default;
	~EffectManager();
	EffectManager(const EffectManager&) = delete;
	EffectManager& operator=(const EffectManager&) = delete;

	//ロード済みエフェクトの情報
	struct EffectData
	{
		int graphHandle = -1;
		Vector2 frameSize;
		int maxAnimNum = 0;
		int oneAnimFrame = 0;
		float scale = 1.0f;
	};
private:
	int nextEffectHandle_ = -1;

	//エフェクトハンドルを管理するマップ
	std::map<std::wstring, EffectData>effects_;

	//エフェクトハンドルを管理する配列
	std::map<int,std::unique_ptr<Effect>>handles_;
};