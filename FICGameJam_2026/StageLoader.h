#pragma once
#include <string>
#include <vector>
#include <set>
#include "Collider/RectCollider.h"

class StageLoader
{
public:
	StageLoader() = default;
	~StageLoader();

	//CSVからマップデータを読み込む
	bool Load(const std::string& filePath);

	//タイル画像を読み込み
	bool LoadTileset(const std::string& filePath, int tileSize, int columns, int totalTiles);

	void Draw(int offsetX = 0, int offsetY = 0, float scale = 1.0f) const;

	//タイルIDを取得する
	int GetTile(int x, int y) const;

	//指定した壁タイルIDから矩形コライダーのリストを生成する
	//std::setとは重複しない要素の集合を表すコンテナで、壁タイルIDの集合を保持するのに適している
	std::vector<RectCollider> CreateColliders(int tileSize, const std::set<int>& wallTileIds) const;

	/// <summary>
	/// 壁のタイルIDを設定する
	/// </summary>
	/// <param name="wallTileIds">壁のタイルID</param>
	void SetWallTileId(const std::set<int>& wallTileIds) { wallTileIds_ = wallTileIds; }

	/// <summary>
	/// 壁かどうかを判定する
	/// </summary>
	/// <param name="gridX">グリッドのX座標</param>
	/// <param name="gridY">グリッドのY座標</param>
	/// <returns>壁であればtrue、そうでなければfalse</returns>
	bool IsWall(int gridX, int gridY)const;

	// ゲッター
	int GetWidth() const { return width_; }
	int GetHeight() const { return height_; }
	int GetTileSize() const { return tileSize_; }

private:
	std::vector<std::vector<int>> stage_;   //マップデータ
	std::vector<int> tilehandle_;           //画像ハンドル配列
	std::set<int> wallTileIds_;				//壁のタイルIDの配列
	int width_ = 0;                         //幅
	int height_ = 0;                        //高さ
	int tileSize_ = 0;                      //タイルのサイズ
};

