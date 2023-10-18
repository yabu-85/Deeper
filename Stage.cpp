#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include <vector>

namespace {
    bool drawCell = true;

}

Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), startPos_(0,0,0), goalPos_(0,0,0)
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
    
    startPos_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    goalPos_ = XMFLOAT3( 50.0f * (float)(rand() % 2 - 1) , 0.0f, 50.0f + (float)(rand() % 50));

    CreatStage();
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

void Stage::ResetStage()
{
}

//ウォーフレームのマップ生成を作って見ようかと思う（カービィディスカバリーのマップ生成も作りたいよね：むずすぎだけど記事いっぱいある）
void Stage::CreatStage()
{
    //敵の出現位置のコントロールは以下のように行われます。
    //(1)各パーツにプレイヤーが移動可能な場所が指定され、マップが組み上がった時に、1本のラインでプレイヤーが歩く可能性があるルートラインが描かれる
    //(2)ゲーム中はプレイヤーから一定範囲でラインに沿って影響力マップが生成され、その範囲内で敵が生成されプレイヤーを襲いかかる

    //敵の出現はタイムラインにすると「ローラーコースターのように」制御されていて、最初は1体の敵だったのが、徐々に敵の襲撃が激しくなり
    //いずれピークに達し、それを一掃すると暫くは平和が訪れる……という風になります。
    //1つのステージの終了までに倒すべき敵の数が決められていて、幾つかの並を超えるとプレイヤーはゴールに辿り着く事が出来ます。

    
    //やることー＞まずマップ生成するー＞ルートをつないでー＞その場所の影響力によって敵がスポーンされる
    

    //なんか血迷って作ったスタートからゴールまで柱建てるやつ
    XMVECTOR vFront{ 0,0,1,0 };
    XMVECTOR vStart = XMLoadFloat3(&startPos_);
    XMVECTOR vGoal = XMLoadFloat3(&goalPos_);
    XMVECTOR vDot = XMVector3Dot(vFront, XMVector3Normalize(vStart - vGoal));
    float angle = (float)acos(XMVectorGetX(vDot));
    XMVECTOR vCross = XMVector3Cross(vFront, vStart - vGoal);
    if (XMVectorGetY(vCross) < 0) {
        angle *= -1;
    }

    XMVECTOR vMove = { 0.0f, 0.0f, -XMVectorGetX(XMVector3Length(vStart - vGoal)) / 10, 0.0f };
    XMMATRIX mRotY = XMMatrixRotationY(angle);
    vMove = XMVector3TransformCoord(vMove, mRotY);

    XMVECTOR vRight = { 10, 0, 0, 0 };
    XMVECTOR direction = XMVector3TransformNormal(vRight, mRotY);

    XMFLOAT3 dataPos1 = XMFLOAT3(0.0f, 0.0f, 0.0f);
    XMFLOAT3 dataPos2 = XMFLOAT3(0.0f, 0.0f, 0.0f);
    for (int i = 1; i <= 10; i++) {
        XMStoreFloat3(&dataPos1, vStart + vMove * i + direction);
        XMStoreFloat3(&dataPos2, vStart + vMove * i - direction);
        intersectDatas_.push_back({ hModel_[MAX + RT2], dataPos1 });
        intersectDatas_.push_back({ hModel_[MAX + RT2], dataPos2 });
    }
    intersectDatas_.push_back({ hModel_[MAX + RT1], startPos_ });
    intersectDatas_.push_back({ hModel_[MAX + RT1], goalPos_ });

}
