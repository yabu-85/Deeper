#include "Stage.h"
#include "Engine/Model.h"

Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage")
{
    for (int i = 0; i < MAX; i++) hModel_[i] = -1;
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    std::string fileName[] = { "StageT1", "StageT2" };
    for (int i = 0; i < MAX; i++) {
        hModel_[i] = Model::Load("Model/" + fileName[i] + ".fbx");
        assert(hModel_[i] >= 0);
    }

    IntersectData data1 = { hModel_[0], XMFLOAT3(25.0f, 0.0f, 0.0f) };
    IntersectData data2 = { hModel_[0], XMFLOAT3(-25.0f, 0.0f, 0.0f) };
    IntersectData data3 = { hModel_[1], XMFLOAT3(-10.0f, 0.0f, 0.0f) };
    intersectDatas_.push_back(data1);
    intersectDatas_.push_back(data2);
    intersectDatas_.push_back(data3);

}

void Stage::Update()
{
}

void Stage::Draw()
{
    for (int i = 0; i < intersectDatas_.size(); i++) {
        int handle = intersectDatas_.at(i).hModelNum;
        Transform trans;
        trans.position_ = intersectDatas_.at(i).position;
        Model::SetTransform(handle, trans);
        Model::Draw(handle);
    }


}

void Stage::Release()
{
}