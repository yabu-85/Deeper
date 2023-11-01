#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include <vector>
#include "Engine/CsvReader.h"

namespace {
    bool drawCell = true;
    const float floarSize = 5.0f;
    const int StartsizeX = 5;
    const int StartsizeZ = 3;

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
    std::string fileName[] = { "StageT1", "StageT2", "StageT3", "RayStageT1", "RayStageT2", "RayStageT3" };
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
        Model::SetTransform(handle, trans);
        Model::Draw(handle, 0);
    }

    //Ray用のワイヤー表示
    if (drawCell) {
        for (int i = 0; i < intersectDatas_.size(); i++) {
            int handle = intersectDatas_.at(i).hModelNum + MAX;
            Transform trans;
            trans.position_ = intersectDatas_.at(i).position;
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
    return XMFLOAT3(startPos_.x * floarSize, 0.0f, startPos_.z * floarSize);
}

void Stage::ResetStage()
{
}

//HADESみたいに作ろう：CSVを読み込みで作成
void Stage::CreatStage()
{
    //CSVファイル読み込み
    CsvReader csv;
    csv.Load("Csv/Map1.csv");

    //ステージの幅と高さを設定
    mapSizeX_ = (int)csv.GetWidth();
    mapSizeZ_ = (int)csv.GetHeight();

    //サイズをマップの大きさにする
    mapData_.resize(mapSizeZ_);
    for (int i = 0; i < mapSizeZ_; i++) mapData_[i].resize(mapSizeX_, FLOAR);

    //CSVデータをテーブルに格納
    for (int z = 0; z < mapSizeZ_; z++) {
        for (int x = 0; x < mapSizeX_; x++) {
            int data = csv.GetValue(z, x);
            if (data == 10)
            {
                mapData_[z][x] = 1;
                intersectDatas_.push_back({ hModel_[T1], XMFLOAT3(x * floarSize, 0.0f, z * floarSize) });
                startPos_ = XMFLOAT3((float)x, 0.0f, (float)z);
            }
            else if (data == 1)
            {
                mapData_[z][x] = 1;
                intersectDatas_.push_back({ hModel_[T1], XMFLOAT3(x * floarSize, 0.0f, z * floarSize) });
            }
            else if (data == 21)
            {
                mapData_[z][x] = 1;
                intersectDatas_.push_back({ hModel_[T1], XMFLOAT3(x * floarSize, 0.0f, z * floarSize) });
                intersectDatas_.push_back({ hModel_[T2], XMFLOAT3(x * floarSize, 0.0f, z * floarSize) });
            }
            else if (data == 31)
            {
                mapData_[z][x] = 1;
                intersectDatas_.push_back({ hModel_[T1], XMFLOAT3(x * floarSize, 0.0f, z * floarSize) });
                intersectDatas_.push_back({ hModel_[T3], XMFLOAT3(x * floarSize, 0.0f, z * floarSize) });
            }
            else
            {
                mapData_[z][x] = 0;
            }
        }
    }
}
