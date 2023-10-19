#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include <vector>

namespace {
    bool drawCell = true;
    const float floarSize = 5.0f;

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
    
    mapSizeX_ = 30;
    mapSizeZ_ = 50;

    startPos_ = XMFLOAT3( (float)(rand() % mapSizeX_), 0.0f, (float)(mapSizeZ_ - 1));

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
        if(handle == T1) trans.scale_ = XMFLOAT3(5.0f / 10.0f, 1.0f, 5.0f / 10.0f);
        Model::SetTransform(handle, trans);
        Model::Draw(handle, 0);
    }

    //Ray用のワイヤー表示
    if (drawCell) {
        for (int i = 0; i < intersectDatas_.size(); i++) {
            int handle = intersectDatas_.at(i).hModelNum;
            Transform trans;
            trans.position_ = intersectDatas_.at(i).position;
            if (handle == RT1) trans.scale_ = XMFLOAT3(5.0f / 10.0f, 1.0f, 5.0f / 10.0f);
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

    //マップ
    //まずゴールまでのルートにパス引く
    //ゴールまでのパスを導くんじゃなくて、とりあえずパスをある程度の場所まで引いて最終部屋の入口が自動的にgoalPosになる
    
    //マップのサイズ分の２次元配列を用意して、それをもとにパスをつなげていく
    //まずｚが一番下か上今回は下で行くそこにスタートポジションを置いて
    
    //マスを配置してく
    //１出入口の方向を計算し、空いている容量を計算して、ボックスのサイズを計算し配置・その分を２次元配列に入れてく
    //何とかして、出口を配置するマスを計算

    mapData_.resize(mapSizeZ_);
    for (int i = 0; i < mapSizeZ_; i++)
    {
        mapData_[i].resize(mapSizeX_, NONE);
    }

    int StartsizeX = 5;
    int StartsizeZ = 3;
    XMFLOAT3 staPos = XMFLOAT3(startPos_.x, 0.0f, startPos_.z - StartsizeZ);
    CreatSquare(staPos, StartsizeX, StartsizeZ);
    //スタートの出口は固定でいいけど、このあとのマス目はなんかして出口マスを計算してだす
    //
    

    //空白の場所は柱で埋めてみる
    for (int z = 0; z < mapSizeZ_ -1; z++) {
        for (int x = 0; x < mapSizeX_ -1; x++) {
            if (mapData_[z][x] == NONE) {
                intersectDatas_.push_back({ hModel_[MAX + RT2], XMFLOAT3(5.0f * x, 0.0f, 5.0f * z) });
            }
        }
    }

}

XMFLOAT3 Stage::GetPlayerStartPos()
{
    return startPos_;
}

void Stage::CreatSquare(XMFLOAT3 start, int _x, int _z)
{
    for (int x = start.x; x < start.x + _x; x++) {
        for (int z = start.z; z < start.z + _z; z++) {
            intersectDatas_.push_back({ hModel_[MAX + RT1], XMFLOAT3(floarSize * (float)x , start.y, floarSize * (float)z) });
            mapData_[z][x] = FLOAR;
        }
    }
}
