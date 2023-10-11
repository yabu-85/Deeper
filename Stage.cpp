#include "Stage.h"
#include "Engine/Model.h"

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

    intersectDatas_.push_back( { hModel_[MAX + RT1], XMFLOAT3(25.0f, 0.0f, 0.0f)} );
    intersectDatas_.push_back( { hModel_[MAX + RT1], XMFLOAT3(-25.0f, 0.0f, 0.0f)} );
    intersectDatas_.push_back( { hModel_[MAX + RT2], XMFLOAT3(10.0f, 0.0f, 0.0f)} );
    intersectDatas_.push_back( { hModel_[MAX + RT2], XMFLOAT3(-10.0f, 0.0f, 0.0f)} );

}

void Stage::Update()
{
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
    if (true) {
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