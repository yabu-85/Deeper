#include "CollisionMap.h"
#include <vector>
#include "Engine/FbxParts.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Cell.h"
#include "Player.h"
#include "Stage.h"

namespace {
    const float playerRadius = 1.0f;
    const float boxSize = 10.0f;
    const int maxCount = 8;

    Player* pPlayer;
    Stage* pStage;
    Quad* pQuad = new Quad;
    Cell* pCell = new Cell;

    bool drawQuad = false;
    std::vector<Triangle*> triList;
}

CollisionMap::CollisionMap(GameObject* parent)
    : GameObject(parent, "CollisionMap")
{
}

CollisionMap::~CollisionMap()
{
}

void CollisionMap::Initialize()
{
    pPlayer = (Player*)FindObject("Player");
    pStage = (Stage*)FindObject("Stage");
    pQuad->Initialize();

}

void CollisionMap::Update()
{
    XMFLOAT3 plaPos = pPlayer->GetPosition();

    //プレイヤーの位置を取得して、判定距離内に入った分割ブロックを取得
    //Blockの追加はできたけど、半径・複数はまだやってない
    float fBox[3] = { plaPos.x / boxSize, plaPos.y / boxSize, plaPos.z / boxSize };
    int iBox[3] = { fBox[0], fBox[1], fBox[2] };
    for (int i = 0; i < 3; i++) if (fBox[i] < 0) iBox[i] -= 1;
    for (int i = 0; i < 3; i++) iBox[i] *= 10;
    XMFLOAT3 cellPos = XMFLOAT3((float)iBox[0], (float)iBox[1], (float)iBox[2]);
    XMFLOAT3 currentCellPos = pCell->GetPosision();

    if (cellPos.x != currentCellPos.x || cellPos.y != currentCellPos.y || cellPos.z != currentCellPos.z)
    {
        pQuad->SetPosition(cellPos);
        pCell->SetPosLeng(cellPos, boxSize);
        pCell->ResetTriangles();

        const int polySize = 3;
        std::vector<IntersectData> inteDatas = pStage->GetIntersectDatas();

        //Blockの範囲内のポリゴンを取得したい
        for (int i = 0; i < inteDatas.size(); i++) {
            Fbx* pFbx = Model::GetFbx(inteDatas[i].hModelNum);
            std::vector<FbxParts*> pFbxParts = pFbx->GetFbxParts();

            for (int n = 0; n < pFbxParts.size(); n++) {
                std::vector<XMFLOAT3> polygons = pFbxParts[n]->GetAllPositions();

                //Intersectごとの座標を計算
                XMFLOAT3 interPos = inteDatas[i].position;
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

        triList = pCell->GetTriangles();
        std::string strNumber;
        strNumber = std::to_string(triList.size());
        OutputDebugStringA(strNumber.c_str());
        OutputDebugString("\n");
    }

    if (Input::IsKeyDown(DIK_R)) drawQuad = !drawQuad;

}

void CollisionMap::Draw()
{
    if (drawQuad) pQuad->Draw();

}

void CollisionMap::Release()
{
}

std::vector<Triangle*>& CollisionMap::GetCellInTriangle()
{
    return triList;
}