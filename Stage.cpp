#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include <vector>
#include "Engine/CsvReader.h"
#include "CollisionMap.h"
#include "Player.h"

namespace {
    //デバッグ用
    bool drawCell = true;
}

Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), startPos_(0,0,0), mapSizeX_(0), mapSizeZ_(0)
{
    for (int i = 0; i < MAX + RMAX; i++) hModel_[i] = -1;
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    std::string fileName[] = { "StageT1", "StageT2", "RayStageT1", "RayStageT2" };
    for (int i = 0; i < MAX + RMAX; i++) {
        hModel_[i] = Model::Load("Model/Stage/" + fileName[i] + ".fbx");
        assert(hModel_[i] >= 0);
    }
    
    CreatStage();
}

void Stage::Update()
{
    //デバッグ用
    if (Input::IsKeyDown(DIK_E)) drawCell = !drawCell;

}

void Stage::Draw()
{
    for (int i = 0; i < intersectDatas_.size(); i++) {
        int handle = intersectDatas_.at(i).hModelNum;
        Transform trans;
        trans.position_ = intersectDatas_.at(i).position;
        trans.scale_ = intersectDatas_.at(i).scale;
        Model::SetTransform(handle, trans);
        Model::Draw(handle, 0);
    }

    //Ray用のワイヤー表示
    if (drawCell) {
        for (int i = 0; i < intersectDatas_.size(); i++) {
            int handle = intersectDatas_.at(i).hModelNum + MAX;
            Transform trans;
            trans.position_ = intersectDatas_.at(i).position;
            trans.scale_ = intersectDatas_.at(i).scale;
            Model::SetTransform(handle, trans);
            Model::Draw(handle, 2);
        }
    }
}

void Stage::Release()
{
}

XMFLOAT3 Stage::GetPlayerStartPos()
{
    return XMFLOAT3(startPos_.x * floarSize + (floarSize / 2), 0.0f, startPos_.z * floarSize + (floarSize / 2));
}

XMFLOAT3 Stage::GetRandomFloarPosition()
{
    struct Cell
    {
        float x, z;
    };
    std::vector<Cell> posList;
    for (int x = 0; x < mapSizeX_; x++) {
        for (int z = 0; z < mapSizeZ_; z++) {
            if (mapData_[z][x] == Stage::MAP::FLOAR) {
                Cell cell;
                cell.x = (float)x;
                cell.z = (float)z;
                posList.push_back(cell);
            }
        }
    }

    int index = rand() % posList.size();
    return XMFLOAT3(posList.at(index).x * floarSize + (floarSize / 2), 0.0f, posList.at(index).z * floarSize + (floarSize / 2));
}

XMFLOAT3 Stage::GetFloarPosition(XMFLOAT3 position, float range)
{
    Player* pPlayer = (Player*)FindObject("Player");
    XMFLOAT3 f = pPlayer->GetPosition();
 
    XMVECTOR pPos = XMLoadFloat3(&f);
    XMVECTOR ePos = XMLoadFloat3(&position);
    XMVECTOR vec = ePos - pPos;
    vec = XMVector3Normalize(vec) * range;
    
    XMVECTOR endPosition = pPos + vec;
    XMStoreFloat3(&f, endPosition);

    int pos[2] = { 0,0 };
    pos[0] = static_cast<int>(f.x / floarSize);
    pos[1] = static_cast<int>(f.z / floarSize);
    int size = 0;
    int max = 5;
    int maxCount = 0;

    struct Cell
    {
        int x, z;
        Cell(int _x, int _z) : x(_x), z(_z) {};
    };
    std::vector<Cell> cellList;

    while (maxCount < max) {
        cellList.clear();
        for (int i = pos[0] - size; i <= pos[0] + size; ++i) {
            for (int j = pos[1] - size; j <= pos[1] + size; ++j) {
                if (i < 0 || i >= (int)mapData_[0].size() || j < 0 || j >= (int)mapData_.size())
                    continue;

                if (mapData_[j][i] == Stage::MAP::FLOAR) {
                    Cell c(i, j);
                    cellList.push_back(c);
                }
            }
        }

        if (!cellList.empty()) {
            int randamIndex = rand() % cellList.size();
            XMFLOAT3 out = XMFLOAT3((float)cellList.at(randamIndex).x, 0.0f, (float)cellList.at(randamIndex).z);
            out = XMFLOAT3(out.x * floarSize + floarSize / 2.0f, out.y, out.z * floarSize + floarSize / 2.0f);
            return out;
        }

        size++;
        maxCount++;
    }

    return position;
}

void Stage::ResetStage()
{
}

//HADESみたいに作ろう：CSVを読み込みで作成
void Stage::CreatStage()
{
    //CSVファイル読み込み
    CsvReader csv;
    csv.Load("Csv/Map3.csv");

    //ステージの幅と高さを設定
    mapSizeX_ = (int)csv.GetWidth();
    mapSizeZ_ = (int)csv.GetHeight();

    //サイズをマップの大きさにする
    mapData_.resize(mapSizeZ_);
    for (int i = 0; i < mapSizeZ_; i++) mapData_[i].resize(mapSizeX_, FLOAR);

    //CSVデータをテーブルに格納
    for (int x = 0; x < mapSizeX_; x++) {
        for (int z = 0; z < mapSizeZ_; z++) {
            int data = csv.GetValue(x, z);
            if (data == 10)
            {
                intersectDatas_.push_back({ hModel_[T1], XMFLOAT3(x * floarSize, 0.0f, z * floarSize), XMFLOAT3(1.0f / smallSize, 1.0f / smallSize, 1.0f / smallSize) });
                startPos_ = XMFLOAT3((float)x, 0.0f, (float)z);
            }
            else if (data == 1)
            {
                intersectDatas_.push_back({ hModel_[T1], XMFLOAT3(x * floarSize, 0.0f, z * floarSize), XMFLOAT3(1.0f / smallSize, 1.0f / smallSize, 1.0f / smallSize) });
            }
            else if (data == 2)
            {
                mapData_[z][x] = WALL;
                intersectDatas_.push_back({ hModel_[T1], XMFLOAT3(x * floarSize, 0.0f, z * floarSize), XMFLOAT3(1.0f / smallSize, 1.0f * 0.1f / smallSize, 1.0f / smallSize) });
                intersectDatas_.push_back({ hModel_[T2], XMFLOAT3(x * floarSize, 0.0f, z * floarSize), XMFLOAT3(1.0f / smallSize, 1.0f * 0.1f / smallSize, 1.0f / smallSize) });
            }
        }
    }
}
