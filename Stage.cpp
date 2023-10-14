#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Player.h"
#include <vector>
#include "Cell.h"
#include "Engine/FbxParts.h"

#include "Engine/Text.h"

namespace {
    const float playerRadius = 1.0f;
    const float boxSize = 10.0f;
    const int maxCount = 8;

    Player* pPlayer;
    Text *pText = new Text;
    Quad* pQuad = new Quad;
    Cell* pCell = new Cell;

    bool drawQuad = true;
    bool drawCell = true;

}

Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage")
{
    for (int i = 0; i < MAX + RMAX; i++) hModel_[i] = -1;
}

Stage::~Stage()
{
    pQuad->Release();
    
}

void Stage::Initialize()
{
    std::string fileName[] = { "StageT1", "StageT2", "RayStageT1", "RayStageT2" };
    for (int i = 0; i < MAX + RMAX; i++) {
        hModel_[i] = Model::Load("Model/" + fileName[i] + ".fbx");
        assert(hModel_[i] >= 0);
    }

    intersectDatas_.push_back( { hModel_[MAX + RT1], XMFLOAT3(20.0f, 0.0f, 10.0f)} );
    intersectDatas_.push_back( { hModel_[MAX + RT1], XMFLOAT3(-20.0f, 0.0f, 10.0f)} );
    
    const int rt2Count = 5;
    for(int i = 0;i < rt2Count;i++)
    intersectDatas_.push_back( { hModel_[MAX + RT2], XMFLOAT3(5.0f + 8.0f * i, 0.0f, 15.0f)} );

    const int rt2Countm = 5;
    for (int i = 0; i < rt2Countm; i++)
    intersectDatas_.push_back( { hModel_[MAX + RT2], XMFLOAT3(-5.0f + -8.0f * i, 0.0f, 15.0f)} );
    
    pPlayer = (Player*)FindObject("Player");
    pText->Initialize();
	pQuad->Initialize();
}

void Stage::Update()
{
    XMFLOAT3 plaPos = pPlayer->GetPosition();

    //プレイヤーの位置を取得して、判定距離内に入った分割ブロックを取得
    //Blockの追加はできたけど、半径・複数はまだやってない

    float fBox[3] = { plaPos.x / boxSize, plaPos.y / boxSize, plaPos.z / boxSize };
    int iBox[3] = { fBox[0], fBox[1], fBox[2] };
    for (int i = 0; i < 3; i++) if (fBox[i] < 0) iBox[i] -= 1;
    for(int i = 0;i < 3;i++) iBox[i] *= 10;
    XMFLOAT3 cellPos = XMFLOAT3((float)iBox[0], (float)iBox[1], (float)iBox[2]);
    XMFLOAT3 currentCellPos = pCell->GetPosision();

    if (cellPos.x != currentCellPos.x || cellPos.y != currentCellPos.y || cellPos.z != currentCellPos.z)
    {
        pQuad->SetPosition(cellPos);
        pCell->SetPosLeng(cellPos, boxSize);
        pCell->ResetTriangles();

        const int polySize = 3;

        //Blockの範囲内のポリゴンを取得したい
        for (int i = 0; i < intersectDatas_.size(); i++) {
            Fbx* pFbx = Model::GetFbx(intersectDatas_[i].hModelNum);
            std::vector<FbxParts*> pFbxParts = pFbx->GetFbxParts();

            for (int n = 0; n < pFbxParts.size(); n++) {
                std::vector<XMFLOAT3> polygons = pFbxParts[n]->GetAllPositions();

                //Intersectごとの座標を計算
                XMFLOAT3 interPos = intersectDatas_[i].position;
                for (int j = 0; j < polygons.size(); j++)
                polygons[j] = XMFLOAT3(polygons[j].x + interPos.x, polygons[j].y + interPos.y, polygons[j].z + interPos.z);

                int polygonsSize = polygons.size() / polySize;
                for (int h = 0; h < polygonsSize; h++) {
                    XMVECTOR vpoly[polySize];
                    for (int t = 0; t < polySize; t++)
                    vpoly[t] = XMLoadFloat3(&polygons[h * polySize + t]);

                    Triangle tri;
                    tri.CreatTriangle(vpoly[0], vpoly[1], vpoly[2]);
                    pCell->SetTriangle(tri);
                }
            }
        }

        std::vector<Triangle*> triList = pCell->GetTriangles();
        std::string strNumber;
        strNumber = std::to_string(triList.size());
        OutputDebugStringA(strNumber.c_str());
        OutputDebugString("\n");
    }


    if (Input::IsKeyDown(DIK_E)) drawCell = !drawCell;
    if (Input::IsKeyDown(DIK_R)) drawQuad = !drawQuad;

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

	if(drawQuad)
	pQuad->Draw();

    Player* pPlayer = (Player*)FindObject("Player");
    XMFLOAT3 plaPos = pPlayer->GetPosition();
    pText->Draw(30, 30, plaPos.x);
    pText->Draw(30, 70, plaPos.y);
    pText->Draw(30, 110, plaPos.z);

}

void Stage::Release()
{
}