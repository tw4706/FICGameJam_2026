#pragma once
#include"../StageLoader.h"
#include<vector>

class PathFinder
{
public:

	//Intがたでの2Dベクトル
	struct Vector2Int
	{
		int x, y;

		//オペレーター==をオーバーロードして、Vector2Int同士の比較行う
		bool operator==(const Vector2Int& other) const
		{
			return x == other.x && y == other.y;
		}
	};

	/// <summary>
	/// 経路探索
	/// </summary>
	/// <param name="start">開始位置</param>
	/// <param name="goal">終了位置</param>
	/// <param name="stage">ステージの参照</param>
	/// <returns>次の経路を返す</returns>
	std::vector<Vector2Int>FindPath(Vector2Int start, Vector2Int goal, const StageLoader& stage)const;
private:

	struct Node
	{
		Vector2Int pos;								//ノードの位置
		int gCost;									//開始ノードからのコスト
		int hCost;									//ゴールノードまでの推定コスト
		int parentIndex;							//親ノードのインデックス
		int FCost() const { return gCost + hCost; } //総コスト
	};

	/// <summary>
	///	ヒューリスティック関数を計算する
	/// </summary>
	/// <param name="a">開始位置</param>
	/// <param name="b">終了位置</param>
	/// <returns>ヒューリスティックの値を返す</returns>
	float Heuristic(Vector2Int a, Vector2Int b)const;
};

