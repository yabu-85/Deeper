#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Player.h"
#include <vector>
#include "Engine/FbxParts.h"

namespace {
    Player* pPlayer;
    bool drawCell = true;

}

Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage")
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
        hModel_[i] = Model::Load("Model/" + fileName[i] + ".fbx");
        assert(hModel_[i] >= 0);
    }
    
    const int rt1Count = 10;
    for (int i = 0; i < rt1Count; i++) {
        intersectDatas_.push_back({ hModel_[MAX + RT1], XMFLOAT3(40.0f * i + 20.0f, 0.0f, 0.0f) });
        intersectDatas_.push_back({ hModel_[MAX + RT1], XMFLOAT3(-40.0f * i + -20.0f, 0.0f, 0.0f) });
    }
    const int rt2Count = 50;
    for (int i = 0; i < rt2Count; i++) {
        intersectDatas_.push_back({ hModel_[MAX + RT2], XMFLOAT3(5.0f + 8.0f * i, 0.0f, 15.0f) });
        intersectDatas_.push_back({ hModel_[MAX + RT2], XMFLOAT3(5.0f + 8.0f * i, 0.0f, -15.0f) });
    }

    const int rt2Countm = 50;
    for (int i = 0; i < rt2Countm; i++) {
        intersectDatas_.push_back({ hModel_[MAX + RT2], XMFLOAT3(-5.0f + -8.0f * i, 0.0f, 15.0f) });
        intersectDatas_.push_back({ hModel_[MAX + RT2], XMFLOAT3(-5.0f + -8.0f * i, 0.0f, -15.0f) });
    }
    
    pPlayer = (Player*)FindObject("Player");
}

void Stage::Update()
{
    if (Input::IsKeyDown(DIK_E)) drawCell = !drawCell;

}

void Stage::Draw()
{
    for (int i = 0; i < intersectDatas_.size(); i++) {
        int handle = intersectDatas_.at(i).hModelNum - MAX;
        Transform trans;
        trans.position_ = intersectDatas_.at(i).position;
        Model::SetTransform(handle, trans);
        Model::Draw(handle, 0);
    }

    //Ray用のワイヤー表示
    if (drawCell) {
        for (int i = 0; i < intersectDatas_.size(); i++) {
            int handle = intersectDatas_.at(i).hModelNum;
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