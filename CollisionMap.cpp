#include "CollisionMap.h"
#include <vector>
#include "Engine/FbxParts.h"
#include "Engine/Model.h"
#include "Cell.h"
#include "Player.h"
#include "Stage.h"

namespace {
    const float boxSize = 10.0f;
    const int polySize = 3;

    Player* pPlayer;
    Stage* pStage;
    Fbx* pFbx;

    float minX = 0;
    float maxX = 130;
    float minY = -10;
    float maxY = 50; 
    float minZ = 0;
    float maxZ = 130;

    int numX = 0;
    int numY = 0;
    int numZ = 0;

    CellBox* pBox;
}

CollisionMap::CollisionMap(GameObject* parent)
    : GameObject(parent, "CollisionMap")
{
}

CollisionMap::~CollisionMap()
{
    for (int y = 0; y < numY; y++) {
        for (int z = 0; z < numZ; z++) {
            delete[] cells_[y][z];
        }
        delete[] cells_[y];
    }
    delete[] cells_;

}

void CollisionMap::Initialize()
{
    pPlayer = (Player*)FindObject("Player");
    pStage = (Stage*)FindObject("Stage");

    numX = int((abs(maxX) + abs(minX)) / boxSize) + 1;
    numY = int((abs(maxY) + abs(minY)) / boxSize) + 1;
    numZ = int((abs(maxZ) + abs(minZ)) / boxSize) + 1;

    cells_ = new Cell** [numY];
    for (int y = 0; y < numY; y++) {
        cells_[y] = new Cell* [numZ];
        for (int z = 0; z < numZ; z++) {
            cells_[y][z] = new Cell[numX];
        }
    }
    
    //「各CELLの左奥下」の座標を設定
    // 最も「左奥下」のセルの座標はMinX,MinZ,MinY（ここが基準）
    for (int y = 0; y < numY; y++) {
        for (int z = 0; z < numZ; z++) {
            for (int x = 0; x < numX; x++) {
                cells_[y][z][x].SetPosLeng(XMFLOAT3(minX + boxSize * x, minY + boxSize * y, minZ + boxSize * z), boxSize);
            }
        }
    }

    std::vector<Triangle> triList;
    std::vector<IntersectData> inteDatas = pStage->GetIntersectDatas();
    for (int i = 0; i < inteDatas.size(); i++) {
        pFbx = Model::GetFbx(inteDatas[i].hModelNum + StageNum::MAX);
        std::vector<FbxParts*> pFbxParts = pFbx->GetFbxParts();

        for (int n = 0; n < pFbxParts.size(); n++) {
            std::vector<XMFLOAT3> polygons = pFbxParts[n]->GetAllPositions();

            //Intersectごとの座標を計算
            XMFLOAT3 interPos = inteDatas[i].position;
            XMFLOAT3 interScale = inteDatas[i].scale;
            for (int j = 0; j < polygons.size(); j++)
                polygons[j] = XMFLOAT3(polygons[j].x * interScale.x + interPos.x,
                                       polygons[j].y * interScale.y + interPos.y,
                                       polygons[j].z * interScale.z + interPos.z);

            int polygonsSize = (int)polygons.size() / polySize;
            for (int h = 0; h < polygonsSize; h++) {
                XMVECTOR vpoly[polySize];
                for (int t = 0; t < polySize; t++)
                    vpoly[t] = XMLoadFloat3(&polygons[h * polySize + t]);

                Triangle tri;
                tri.CreatTriangle(vpoly[0], vpoly[1], vpoly[2]);
                triList.push_back(tri);
            }
        }
    }

    std::string strNumber = std::to_string(triList.size());
    OutputDebugString("polygon : ");
    OutputDebugStringA(strNumber.c_str());
    OutputDebugString("\n");

    //「各CELL」に含まれる三角ポリゴンを登録
    for (int y = 0; y < numY; y++) {
        for (int z = 0; z < numZ; z++) {
            for (int x = 0; x < numX; x++) {
                for (int i = 0; i < (int)triList.size(); i++) {
                    cells_[y][z][x].SetTriangle(triList[i]);
                }
            }
        }
    }

    pBox = Instantiate<CellBox>(this);
}

void CollisionMap::Update()
{
    //プレイヤーの位置を取得して、判定距離内に入った分割ブロックを取得
    XMFLOAT3 plaPos = pPlayer->GetPosition();
    float fBox[polySize] = { plaPos.x / boxSize, plaPos.y / boxSize, plaPos.z / boxSize };
    int iBox[polySize] = { (int)fBox[0], (int)fBox[1], (int)fBox[2] };
    for (int i = 0; i < polySize; i++) if (fBox[i] < 0) iBox[i] -= 1;
    for (int i = 0; i < polySize; i++) iBox[i] *= boxSize;
    XMFLOAT3 cellPos = XMFLOAT3((float)iBox[0], (float)iBox[1], (float)iBox[2]);
    pBox->SetPosition(cellPos);
    pBox->SetScale(XMFLOAT3(boxSize, boxSize, boxSize));

}

void CollisionMap::Draw()
{
}

void CollisionMap::Release()
{
}

float CollisionMap::GetRayCastMinDist(XMFLOAT3 pos, RayCastData* _data)
{
    RayCastData data;
    const float minRangeMax = FBXSDK_FLOAT_MAX;
    float minRange = minRangeMax;
    
    int x = int((pos.x - minX) / boxSize);
    int y = int((pos.y - minY) / boxSize);
    int z = int((pos.z - minZ) / boxSize);

    //ここ最大を超えないようにしてるけど、将来なくてもいいように設計したならいらないデバッグ用
    if (x < 0 || y < 0 || z < 0 || x > maxX / boxSize || y > maxY / boxSize || z > maxZ / boxSize) return minRange;

    std::vector<Triangle*> triList;
    triList = cells_[y][z][x].GetTriangles();

    for (int i = 0; i < (int)triList.size(); i++) {
        data.start = _data->start;
        data.dir = _data->dir;

        Triangle tri = *triList.at(i);
        tri.RayCast(&data, tri);

        //レイ当たった・判定距離内だったら
        if (data.hit && minRange > data.dist )
        {
            minRange = data.dist;
        }
    }

    return minRange;
}