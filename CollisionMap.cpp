#include "CollisionMap.h"
#include <vector>
#include "Engine/FbxParts.h"
#include "Engine/Model.h"
#include "Cell.h"
#include "Player.h"
#include "Stage.h"

namespace {
    const float playerRadius = 8.0f;
    const float boxSize = 30.0f;
    const int polySize = 3;

    std::vector<Triangle*> triList;
    Player* pPlayer;
    Stage* pStage;
    Fbx* pFbx;

    CellBox* pBox;
    std::vector<CPolygon*> polyList;
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

    Cell* cell = new Cell;
    cells_.push_back(cell);
    pBox = Instantiate<CellBox>(this);

}

void CollisionMap::Update()
{
    XMFLOAT3 plaPos = pPlayer->GetPosition();

    //�v���C���[�̈ʒu���擾���āA���苗�����ɓ����������u���b�N���擾
    float fBox[polySize] = { plaPos.x / boxSize, plaPos.y / boxSize, plaPos.z / boxSize };
    int iBox[polySize] = { (int)fBox[0], (int)fBox[1], (int)fBox[2] };
    for (int i = 0; i < polySize; i++) if (fBox[i] < 0) iBox[i] -= 1;
    for (int i = 0; i < polySize; i++) iBox[i] *= boxSize;
    XMFLOAT3 cellPos = XMFLOAT3((float)iBox[0], (float)iBox[1], (float)iBox[2]);
    XMFLOAT3 currentCellPos = cells_.front()->GetPosision();

    if (cellPos.x != currentCellPos.x || cellPos.y != currentCellPos.y || cellPos.z != currentCellPos.z)
    {
        std::string strNumber = std::to_string(cells_.size());
        OutputDebugStringA(strNumber.c_str());
        OutputDebugString(" : ");

        cells_.front()->SetPosLeng(cellPos, boxSize);
        cells_.front()->ResetTriangles();
        pBox->SetPosition(cellPos);
        pBox->SetScale(XMFLOAT3(boxSize, boxSize, boxSize));

        std::vector<IntersectData> inteDatas = pStage->GetIntersectDatas();
        //Cell�ς�邲�ƂɌv�Z����̂��ǂ����Ǝv������A�̂��̂����Ƃ�����

        //Block�͈͓̔��̃|���S�����擾������
        for (int i = 0; i < inteDatas.size(); i++) {
            pFbx = Model::GetFbx(inteDatas[i].hModelNum + StageNum::MAX);
            std::vector<FbxParts*> pFbxParts = pFbx->GetFbxParts();

            for (int n = 0; n < pFbxParts.size(); n++) {
                std::vector<XMFLOAT3> polygons = pFbxParts[n]->GetAllPositions();

                //Intersect���Ƃ̍��W���v�Z
                XMFLOAT3 interPos = inteDatas[i].position;
                for (int j = 0; j < polygons.size(); j++)
                    polygons[j] = XMFLOAT3(polygons[j].x + interPos.x, polygons[j].y + interPos.y, polygons[j].z + interPos.z);

                int polygonsSize = (int)polygons.size() / polySize;
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

        strNumber = std::to_string(triList.size());
        OutputDebugStringA(strNumber.c_str());
        OutputDebugString("\n");

        //Cell�Ȃ��̃|���S���S�폜
        for (auto e : polyList) {
            e->Release();
            delete e;
        }polyList.clear();


        //Triangle�\���F�Ȃ񂩂��܂Ƀo�O��
        return;
        for (Cell* ce : cells_) {
            std::vector<Triangle*>& triangles = ce->GetTriangles();
            for (int i = 0; i < triangles.size(); i++) {
                CPolygon* a = new CPolygon;
                Triangle b = *triangles[i];
                XMFLOAT3 poly[3] = { b.GetPosition()[0], b.GetPosition()[1], b.GetPosition()[2] };

                a->Initialize(poly[0], poly[1], poly[2]);
                polyList.push_back(a);

            }
        }
    }
}

void CollisionMap::Draw()
{
    for (auto e : polyList) {
        e->Draw();
    }

}

void CollisionMap::Release()
{
}

float CollisionMap::GetRayCastMinDist(RayCastData* _data)
{
    RayCastData data;
    const float minRangeMax = 100000000;
    float minRange = minRangeMax;
    for (int i = 0; i < (int)triList.size(); i++) {
        data.start = _data->start;
        data.dir = _data->dir;

        Triangle tri = *triList.at(i);
        tri.RayCast(&data, tri);

        //���C���������E���苗������������
        if (data.hit && minRange > data.dist )
        {
            minRange = data.dist;
        }
    }

    return minRange;
}