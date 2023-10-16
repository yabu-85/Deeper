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
    Fbx* pFbx;

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

        //¡‚ÌcellPos‚Ì”»’è‚¾‚Æ‚Å‚«‚Ä‚È‚¢
        //‚Ü‚¸Player‚ªˆÚ“®‚µ‚½‚ç”ÍˆÍ‚Ì‚b‚…‚Œ‚Œ‚ğŒvZ

        //‚Æ‚É‚©‚­‚È‚ñ‚Æ‚©‚µ‚ÄŒvZ‚·‚é
        //‚±‚±[|||||||||||||||||||||||


        //‰E‚¾‚Á‚½‚ç@flag‚ğ‚O‚Æ‚©‚É‚µ‚Ä‰º‚Ìvector‚Ì•ûŒü‚Æ‰~‚ÌÚG”»’è‚Å”ÍˆÍ‚É“ü‚Á‚Ä‚¢‚é‚©‚ğ’²‚×‚é

        XMFLOAT3 zero = XMFLOAT3(0.0f, 0.0f, 0.0f);
        XMFLOAT3 vector[4] = {
            XMFLOAT3(zero.x, zero.y, zero.z),                       //¶‰º
            XMFLOAT3(zero.x, zero.y, zero.z + boxSize),             //¶ã 
            XMFLOAT3(zero.x + boxSize, zero.y, zero.z),             //‰E‰º
            XMFLOAT3(zero.x + boxSize, zero.y, zero.z + boxSize)    //‰Eã
        };

        int vecFlag = -1;
        if (plaPos.x - playerRadius < currentCellPos.x) vecFlag = 0;            //¶
        if (plaPos.x + playerRadius > currentCellPos.x + boxSize) vecFlag = 2;  //‰E

        //‚±‚±‚Å‰º‚©ã–ì”»’è‚ğ‚µ‚Äƒtƒ‰ƒO‚ğİ’è‚·‚é

        //‚Ä‚·‚Æ( Cell‚ÌƒŠƒXƒg‰Šú‰»
        {
            int cellSize = cells_.size();
            for (int i = 1; i < cellSize; i++) {
                cells_.pop_back();
            }
        }


        //‚Ü‚¸‚Í‰º‚Ì”»’è ³í‚¶‚á‚È‚¢
        if (plaPos.y - playerRadius < currentCellPos.y) {
            XMFLOAT3 cellPosTest = XMFLOAT3(cellPos.x, cellPos.y - boxSize, cellPos.z);
            Cell* cellTest = new Cell;
            cellTest->SetPosLeng(cellPosTest, boxSize);
            cellTest->ResetTriangles();
            cells_.push_back(cellTest);
        }

        //‚Æ‚è‚ ‚¦‚¸ã‚Ì”»’èl‚¦‚Ä‚İ‚é
        if (plaPos.y + playerRadius > currentCellPos.y + boxSize) {
            XMFLOAT3 cellPosTest = XMFLOAT3(cellPos.x, cellPos.y + boxSize, cellPos.z);
            Cell* cellTest = new Cell;
            cellTest->SetPosLeng(cellPosTest, boxSize);
            cellTest->ResetTriangles();
            cells_.push_back(cellTest);

        }

        //Cell‚Ì4‚Â‚Ì’¸“_‚æ‚èŠO‚É‚¢‚½‚ç”»’è‚³‚¹‚é‚±‚Æ‚É‚·‚é

        //

        std::string strNumber = std::to_string(cells_.size());
        OutputDebugStringA(strNumber.c_str());
        OutputDebugString(" : ");


        pQuad->SetPosition(cellPos);
        cells_.front()->SetPosLeng(cellPos, boxSize);
        cells_.front()->ResetTriangles();

        std::vector<IntersectData> inteDatas = pStage->GetIntersectDatas();

        //Block‚Ì”ÍˆÍ“à‚Ìƒ|ƒŠƒSƒ“‚ğæ“¾‚µ‚½‚¢
        for (int i = 0; i < inteDatas.size(); i++) {
            pFbx = Model::GetFbx(inteDatas[i].hModelNum);
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

                    for (Cell* ce : cells_) {
                        if (ce->SetTriangle(tri)) break;
                    }
                }
            }
        }


        triList.clear();

        for (Cell* ce : cells_) {
            std::vector<Triangle*>& triangles = ce->GetTriangles();
            triList.insert(triList.end(), triangles.begin(), triangles.end());
        }

        strNumber;
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