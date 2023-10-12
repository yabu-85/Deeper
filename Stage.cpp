#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

//Rayの分割用のてすとようでーた
#include "Player.h"
#include "Engine/Text.h"

namespace {
    const float playerRadius = 1.0f;
    const float boxSize = 10.0f;
    const int maxCount = 8;

    Text *pText = new Text;

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

    pText->Initialize();

}

void Stage::Update()
{
    //なんかここで四角の中のポリゴン取得したりしたい
    if (!Input::IsKeyDown(DIK_SPACE)) return;

    Player* pPlayer = (Player*)FindObject("Player");
    XMFLOAT3 plaPos = pPlayer->GetPosition();

    //プレイヤーの位置を取得して、判定距離内に入った分割ブロックを取得
    float fBox[3] = { plaPos.x / boxSize, plaPos.y / boxSize, plaPos.z / boxSize };
    int iBox[3] = { fBox[0], fBox[1], fBox[2] };
    for (int i = 0; i < 3; i++) if (fBox[i] < 0) iBox[i] -= 1;
    for(int i = 0;i < 3;i++) iBox[i] *= 10;

    std::string strNumber[3];
    for(int i = 0;i < 3;i++) strNumber[i] = std::to_string(iBox[i]);

    OutputDebugStringA(strNumber[0].c_str());
    OutputDebugString(", ");
    OutputDebugStringA(strNumber[1].c_str());
    OutputDebugString(", ");
    OutputDebugStringA(strNumber[2].c_str());
    OutputDebugString("\n");

    //Model::GetFbx(intersectDatas_[i].hModelNum);

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

    Player* pPlayer = (Player*)FindObject("Player");
    XMFLOAT3 plaPos = pPlayer->GetPosition();

    pText->Draw(30, 30, plaPos.x);
    pText->Draw(30, 70, plaPos.y);
    pText->Draw(30, 110, plaPos.z);

}

void Stage::Release()
{
}