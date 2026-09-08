#include "StageLoader.h"
#include <fstream>
#include <sstream>
#include <Dxlib.h>

StageLoader::~StageLoader()
{
    // 読み込んだ画像ハンドルの解放処理
    for (int handle : tilehandle_)
    {
        if (handle != -1)
        {
            DeleteGraph(handle);
        }
    }
}

bool StageLoader::Load(const std::string& filePath)
{
    std::ifstream ifs(filePath);
    if (!ifs.is_open())
    {
        return false;
    }

    stage_.clear();
    std::string line;

    // CSVファイルを1行ずつ読み込む
    while (std::getline(ifs, line))
    {
        // 末尾の改行コード \r を除去 (Windows環境対策)
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }
        if (line.empty())
        {
            continue;
        }

        std::vector<int> row;
        std::stringstream ss(line);
        std::string cell;

        // カンマ区切りで数値に変換して追加
        while (std::getline(ss, cell, ','))
        {
            row.push_back(std::stoi(cell));
        }
        stage_.push_back(row);
    }

    if (stage_.empty())
    {
        return false;
    }

    // マップの縦横サイズを保持
    height_ = static_cast<int>(stage_.size());
    width_ = static_cast<int>(stage_[0].size());

    return true;
}

bool StageLoader::LoadTileset(const std::string& filePath, int tileSize, int columns, int totalTiles)
{
    tileSize_ = tileSize;
    tilehandle_.assign(totalTiles, -1);

    int rows = (totalTiles + columns - 1) / columns;

	//マルチバイト文字列をワイド文字列に変換する
    wchar_t wFilePath[1024];
    MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wFilePath, 1024);

    //変換したwFilePathをLoadDivGraphに渡す
    return LoadDivGraph(
        wFilePath,
        totalTiles,
        columns,
        rows,
        tileSize,
        tileSize,
        tilehandle_.data()) == 0;
}

void StageLoader::Draw(int offsetX, int offsetY, float scale) const
{
    //拡大後のタイルサイズ
    float scaledTileSize = tileSize_ * scale;

    for (int y = 0; y < height_; ++y)
    {
        for (int x = 0; x < width_; ++x)
        {
            int id = stage_[y][x];

            if (id < 0 || id >= static_cast<int>(tilehandle_.size())) continue;
            int handle = tilehandle_[id];
            if (handle == -1) continue;

            // 描画位置をスケールに合わせて計算
            int drawX1 = static_cast<int>(offsetX + x * scaledTileSize);
            int drawY1 = static_cast<int>(offsetY + y * scaledTileSize);
            int drawX2 = static_cast<int>(drawX1 + scaledTileSize);
            int drawY2 = static_cast<int>(drawY1 + scaledTileSize);

            //拡大して描画
            DrawExtendGraph(drawX1, drawY1, drawX2, drawY2, handle, TRUE);
        }
    }
}

int StageLoader::GetTile(int x, int y) const
{
    // 範囲外チェック
    if (x < 0 || y < 0 || x >= width_ || y >= height_)
    {
        return -1;
    }
    return stage_[y][x];
}

std::vector<RectCollider> StageLoader::CreateColliders(int tileSize, const std::set<int>& wallTileIds) const
{
    std::vector<RectCollider> colliders;

    for (int y = 0; y < height_; ++y)
    {
        for (int x = 0; x < width_; ++x)
        {
            //壁タイルIDに含まれていない場合はスキップ
            if (wallTileIds.count(stage_[y][x]) == 0)
            {
                continue;
            }

            //タイルの中心座標を計算して矩形コライダーを作成
            Vector2 center(x * static_cast<float>(tileSize) + tileSize / 2.0f,y * static_cast<float>(tileSize) + tileSize / 2.0f);

			//矩形コライダーを追加
            colliders.emplace_back(center, static_cast<float>(tileSize), static_cast<float>(tileSize));
        }
    }

    return colliders;
}

bool StageLoader::IsWall(int gridX, int gridY) const
{
    int id = GetTile(gridX, gridY);
    if (id == -1) return true;          //範囲外は壁扱いにする
    return wallTileIds_.count(id) > 0;
}
