#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

//Rayの分割用のてすとようでーた
#include "Player.h"

namespace {
    const float playerRadius = 1.0f;
    const float boxSize = 10.0f;
    const int maxCount = 8;
    
    //多分８個以上はつかわないかな
    int boxCount[maxCount] = { 0,0,0,0,0,0,0,0 };


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

    intersectDatas_.push_back( { hModel_[MAX + RT1], XMFLOAT3(25.0f, 0.0f, 0.0f)} );
    intersectDatas_.push_back( { hModel_[MAX + RT1], XMFLOAT3(-25.0f, 0.0f, 0.0f)} );
    intersectDatas_.push_back( { hModel_[MAX + RT2], XMFLOAT3(10.0f, 0.0f, 0.0f)} );
    intersectDatas_.push_back( { hModel_[MAX + RT2], XMFLOAT3(-10.0f, 0.0f, 0.0f)} );

}

void Stage::Update()
{
    //なんかここで四角の中のポリゴン取得したりしたい
    if (!Input::IsKeyDown(DIK_SPACE)) return;


    Player* pPlayer = (Player*)FindObject("Player");
    XMFLOAT3 plaPos = pPlayer->GetPosition();

    for (int i = 0; i < maxCount; i++) {

    }



    for (int i = 0; i < intersectDatas_.size(); i++) {
        Model::GetFbx(intersectDatas_[i].hModelNum);
    }

    //プレイヤーの位置を取得して、判定距離内に入った分割ブロックを取得

    //とったFbxデータの指定ポリゴンだけにRayCastする関数とか作って判定をしたい



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