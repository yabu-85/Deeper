#include "CollisionMap.h"
#include <vector>
#include "Engine/FbxParts.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Cell.h"
#include "Player.h"
#include "Stage.h"

namespace {
    const float playerRadius = 8.0f;
    const float boxSize = 10.0f;
    const int maxCount = 8;
    const int polySize = 3;

    Player* pPlayer;
    Stage* pStage;
    Quad* pQuad = new Quad;

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

    Cell* cell = new Cell;
    cells_.push_back(cell);

}

void CollisionMap::Update()
{
    XMFLOAT3 plaPos = pPlayer->GetPosition();

    //ƒvƒŒƒCƒ„[‚ÌˆÊ’u‚ğæ“¾‚µ‚ÄA”»’è‹——£“à‚É“ü‚Á‚½•ªŠ„ƒuƒƒbƒN‚ğæ“¾
    float fBox[polySize] = { plaPos.x / boxSize, plaPos.y / boxSize, plaPos.z / boxSize };
    int iBox[polySize] = { fBox[0], fBox[1], fBox[2] };
    for (int i = 0; i < polySize; i++) if (fBox[i] < 0) iBox[i] -= 1;
    for (int i = 0; i < polySize; i++) iBox[i] *= 10;
    XMFLOAT3 cellPos = XMFLOAT3((float)iBox[0], (float)iBox[1], (float)iBox[2]);
    XMFLOAT3 currentCellPos = cells_.front()->GetPosision();

    if (cellPos.x != currentCellPos.x || cellPos.y != currentCellPos.y || cellPos.z != currentCellPos.z)
    {

        //‚±‚±‚Å•¡”‘I‘ğ‚Å‚«‚é‚æ‚¤‚É‚µ‚½‚¢

        //‚Ü‚¸‚Í‰º‚Ì”»’è ³í‚¶‚á‚È‚¢ƒ||||||||||||||||
        if (plaPos.y - playerRadius < currentCellPos.y) {
            XMFLOAT3 cellPosTest = XMFLOAT3(cellPos.x, cellPos.y - boxSize, cellPos.z);
            Cell* cellTest = new Cell;
            cellTest->SetPosLeng(cellPosTest, boxSize);
            cellTest->ResetTriangles();
            cells_.push_back(cellTest);

            OutputDebugString(" down !! , ");
        }


        pQuad->SetPosition(cellPos);
        cells_.front()->SetPosLeng(cellPos, boxSize);
        cells_.front()->ResetTriangles();

        std::vector<IntersectData> inteDatas = pStage->GetIntersectDatas();

        //Block‚Ì”ÍˆÍ“à‚Ìƒ|ƒŠƒSƒ“‚ğæ“¾‚µ‚½‚¢
        for (int i = 0; i < inteDatas.size(); i++) {
            Fbx* pFbx = Model::GetFbx(inteDatas[i].hModelNum);
            std::vector<FbxParts*> pFbxParts = pFbx->GetFbxParts();

            for (int n = 0; n < pFbxParts.size(); n++) {
                std::vector<XMFLOAT3> polygons = pFbxParts[n]->GetAllPositions();

                //Intersect‚²‚Æ‚ÌÀ•W‚ğŒvZ
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
                    cells_.front()->SetTriangle(tri);
                }
            }
        }

        triList = cells_.front()->GetTriangles();
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