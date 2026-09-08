#include "PathFinder.h"
#include<algorithm>

namespace
{
	//4方向移動(上下左右)
	const PathFinder::Vector2Int kDirections[4] =
	{
		{ 1, 0},
		{-1, 0},
		{ 0, 1},
		{ 0,-1},
	};
}

std::vector<PathFinder::Vector2Int> PathFinder::FindPath(Vector2Int start, Vector2Int goal, const StageLoader& stage) const
{
	//開始位置または終了位置が壁の場合は経路探索を行わない
    if (stage.IsWall(start.x, start.y) || stage.IsWall(goal.x, goal.y))
    {
		return{};
    }

	//開始位置と終了位置が同じ場合は経路探索を行わない
    if (start == goal)
    {
		return{};
    }

	std::vector<Node> openList;
	std::vector<Node> closedList;

	Node startNode;
	startNode.pos = start;
	startNode.gCost = 0.0f;
	startNode.hCost = Heuristic(start, goal);
	startNode.parentIndex = -1;
	openList.push_back(startNode);

	while (!openList.empty())
	{
		//openList内でF値が最小のノードを探す
		auto currentIt = std::min_element(openList.begin(), openList.end(),
			[](const Node& a, const Node& b) { return a.FCost() < b.FCost(); });

		Node current = *currentIt;
		openList.erase(currentIt);

		//現在のノードをclosedListへ移動し、経路復元用にインデックスを控えておく
		closedList.push_back(current);
		int currentIndex = static_cast<int>(closedList.size()) - 1;

		//ゴールに到達したら経路を復元する
		if (current.pos == goal)
		{
			std::vector<Vector2Int> path;
			int index = currentIndex;
			while (index != -1)
			{
				path.push_back(closedList[index].pos);
				index = closedList[index].parentIndex;
			}
			std::reverse(path.begin(), path.end());
			//startを含めたくない場合は先頭を削除
			if (!path.empty())
			{
				path.erase(path.begin());
			}
			return path;
		}

		//隣接マスを調べる
		for (const auto& dir : kDirections)
		{
			Vector2Int next{ current.pos.x + dir.x, current.pos.y + dir.y };

			//壁なら対象外
			if (stage.IsWall(next.x, next.y))
			{
				continue;
			}

			//すでにclosedListにあるなら対象外
			//std::any_ofは、指定された範囲内の要素の中で、条件を満たす要素が1つでもあるかどうかを判定するアルゴリズム
			bool isClosed = std::any_of(closedList.begin(), closedList.end(),
				[&](const Node& n) { return n.pos == next; });
			if (isClosed)
			{
				continue;
			}

			//移動コストは常に1
			float newG = current.gCost + 1.0f; 

			//すでにopenListにあるか調べる
			auto openIt = std::find_if(openList.begin(), openList.end(),
				[&](const Node& n) { return n.pos == next; });

			if (openIt == openList.end())
			{
				//未探索のマスなら追加
				Node nextNode;
				nextNode.pos = next;
				nextNode.gCost = newG;
				nextNode.hCost = Heuristic(next, goal);
				nextNode.parentIndex = currentIndex;
				openList.push_back(nextNode);
			}
			else if (newG < openIt->gCost)
			{
				//今回の経路の方が短いなら更新
				openIt->gCost = newG;
				openIt->parentIndex = currentIndex;
			}
		}
	}

	//経路が見つからなかった
	return {};
}

float PathFinder::Heuristic(Vector2Int a, Vector2Int b) const
{
	//マンハッタン距離を計算する
	//マンハッタン距離とは、縦横の移動のみを考慮して距離を求める計算方法
	return static_cast<float>(std::abs(a.x - b.x) + std::abs(a.y - b.y));
}
