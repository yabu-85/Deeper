#include "CollisionMap.h"
#include <vector>
#include "Engine/FbxParts.h"
#include "Engine/Model.h"
#include "Cell.h"
#include "Player.h"
#include "Stage.h"

namespace {
    const float boxSize = 25.0f;
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

    //Cellに追加する予定のTriangleをすべて計算してCreatする
    std::vector<Triangle*> triList;
    std::vector<IntersectData> inteDatas = pStage->GetIntersectDatas();
    for (int i = 0; i < inteDatas.size(); i++) {
        pFbx = Model::GetFbx(inteDatas[i].hModelNum + Stage::MAX);
        std::vector<FbxParts*> pFbxParts = pFbx->GetFbxParts();

        //IntersectDataのCollision用モデルのパーツをすべて取得し、その全ポリゴンの座標を計算
        for (int n = 0; n < pFbxParts.size(); n++) {
            std::vector<XMFLOAT3> polygons = pFbxParts[n]->GetAllPositions();

            //モデルデータの座標を計算
            XMFLOAT3 interPos = inteDatas[i].position;
            XMFLOAT3 interScale = inteDatas[i].scale;
            for (int j = 0; j < polygons.size(); j++)
                polygons[j] = XMFLOAT3(polygons[j].x * interScale.x + interPos.x,
                                       polygons[j].y * interScale.y + interPos.y,
                                       polygons[j].z * interScale.z + interPos.z);

            //Triangleに割当たるポリゴンを取得しCreatしてリストに追加
            int polygonsSize = (int)polygons.size() / polySize;
            for (int h = 0; h < polygonsSize; h++) {
                XMVECTOR vpoly[polySize];
                for (int t = 0; t < polySize; t++)
                    vpoly[t] = XMLoadFloat3(&polygons[h * polySize + t]);

                Triangle *tri = new Triangle();
                tri->CreatTriangle(vpoly[0], vpoly[1], vpoly[2]);
                triList.push_back(tri);
            }
        }
    }

    OutputDebugString("polygon : ");
    OutputDebugStringA(std::to_string(triList.size()).c_str());
    OutputDebugString("\n");

    //「各CELL」に含まれる三角ポリゴンを登録
    for (int y = 0; y < numY; y++) {
        for (int z = 0; z < numZ; z++) {
            for (int x = 0; x < numX; x++) {
                for (int i = 0; i < (int)triList.size(); i++) {
                    cells_[y][z][x].SetTriangle(*triList[i]);
                }
            }
        }
    }

    pBox = Instantiate<CellBox>(this);
}

void CollisionMap::Update()
{
    //デバッグ用
    //プレイヤーの位置を取得して、判定距離内に入った分割ブロックを取得
    XMFLOAT3 plaPos = pPlayer->GetPosition();
    Cell* cell = GetCell(plaPos);
    if (cell) {
        pBox->SetPosition(cell->GetPosision());
        pBox->SetScale(XMFLOAT3(boxSize, boxSize, boxSize));
    }
}

void CollisionMap::Draw()
{
}

void CollisionMap::Release()
{
}

Cell* CollisionMap::GetCell(XMFLOAT3 pos)
{
    int x = int((pos.x - minX) / boxSize);
    int y = int((pos.y - minY) / boxSize);
    int z = int((pos.z - minZ) / boxSize);

    //デバッグ用
    //ここ最大を超えないようにしてるけど、将来なくてもいいように設計したならいらない
    if (x < 0 || y < 0 || z < 0 || x > maxX / boxSize || y > maxY / boxSize || z > maxZ / boxSize) {
        return nullptr;
    }

    return &cells_[y][z][x];
}

bool CollisionMap::GetRayCastMinDist(XMFLOAT3 camPos, XMFLOAT3 plaPos, RayCastData* _data, float& minDist)
{
    bool hitC = false;
    float distC = FBXSDK_FLOAT_MAX;
    minDist = distC;

    Cell* cell = GetCell(camPos);
    if (!cell) return false;
    hitC = cell->SegmentVsTriangle(_data, distC);

    cell = GetCell(plaPos);
    if (!cell) return false;
    float distP = FBXSDK_FLOAT_MAX;
    bool hitP = cell->SegmentVsTriangle(_data, distP);
    
    if (distC > distP) minDist = distC;
    else minDist = distP;

    return hitC || hitP;
}

void CollisionMap::MapDataVsBox(BoxCollider* collider)
{

}

void CollisionMap::MapDataVsSphere(SphereCollider* collider, XMFLOAT3 prePos)
{
    XMFLOAT3 pos = collider->GetGameObject()->GetPosition();
    Cell* cell = GetCell(pos);
    if (!cell) return;

    cell->MapDataVsSphere(collider, prePos);
}