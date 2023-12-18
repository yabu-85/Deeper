#include "CollisionMap.h"
#include <vector>
#include "Engine/FbxParts.h"
#include "Engine/Model.h"
#include "Cell.h"
#include "Stage.h"

//�f�o�b�O�p
#include "Player.h"

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
    
    //�u�eCELL�̍������v�̍��W��ݒ�
    // �ł��u�������v�̃Z���̍��W��MinX,MinZ,MinY�i��������j
    for (int y = 0; y < numY; y++) {
        for (int z = 0; z < numZ; z++) {
            for (int x = 0; x < numX; x++) {
                cells_[y][z][x].SetPosLeng(XMFLOAT3(minX + boxSize * x, minY + boxSize * y, minZ + boxSize * z), boxSize);
            }
        }
    }

    //Cell�ɒǉ�����\���Triangle�����ׂČv�Z����Creat����
    std::vector<Triangle*> triList;
    std::vector<IntersectData> inteDatas = pStage->GetIntersectDatas();
    for (int i = 0; i < inteDatas.size(); i++) {
        pFbx = Model::GetFbx(inteDatas[i].hModelNum + Stage::MAX);
        std::vector<FbxParts*> pFbxParts = pFbx->GetFbxParts();

        //IntersectData��Collision�p���f���̃p�[�c�����ׂĎ擾���A���̑S�|���S���̍��W���v�Z
        for (int n = 0; n < pFbxParts.size(); n++) {
            std::vector<XMFLOAT3> polygons = pFbxParts[n]->GetAllPositions();

            //���f���f�[�^�̍��W���v�Z
            XMFLOAT3 interPos = inteDatas[i].position;
            XMFLOAT3 interScale = inteDatas[i].scale;
            for (int j = 0; j < polygons.size(); j++)
                polygons[j] = XMFLOAT3(polygons[j].x * interScale.x + interPos.x,
                                       polygons[j].y * interScale.y + interPos.y,
                                       polygons[j].z * interScale.z + interPos.z);

            //Triangle�Ɋ�������|���S�����擾��Creat���ă��X�g�ɒǉ�
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

    //�u�eCELL�v�Ɋ܂܂��O�p�|���S����o�^
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
    //�f�o�b�O�p
    //�v���C���[�̈ʒu���擾���āA���苗�����ɓ����������u���b�N���擾
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

bool CollisionMap::GetCellIndex(XMFLOAT3& pos)
{
    pos.x = (pos.x - minX) / boxSize;
    pos.y = (pos.y - minY) / boxSize;
    pos.z = (pos.z - minZ) / boxSize;
    bool isClamp = true;

    if (pos.x < 0) { pos.x = 0; isClamp = false; }
    else if (pos.x > maxX / boxSize) { pos.x = maxX / boxSize - 1; isClamp = false; }

    if (pos.y < 0) { pos.y = 0; isClamp = false; }
    else if (pos.y > maxY / boxSize) { pos.y = maxY / boxSize - 1; isClamp = false; }

    if (pos.z < 0) { pos.z = 0; isClamp = false; }
    else if (pos.z > maxZ / boxSize) { pos.z = maxZ / boxSize - 1; isClamp = false; }

    return isClamp;
}

Cell* CollisionMap::GetCell(XMFLOAT3 pos)
{
    int x = int((pos.x - minX) / boxSize);
    int y = int((pos.y - minY) / boxSize);
    int z = int((pos.z - minZ) / boxSize);

    //�����ő�𒴂��Ȃ��悤�ɂ��Ă邯�ǁA�����Ȃ��Ă������悤�ɐ݌v�����Ȃ炢��Ȃ�
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

void CollisionMap::RaySelectCellVsSegment(RayCastData& _data, XMFLOAT3 target)
{
    int startX = int((_data.start.x - minX) / boxSize);
    int startY = int((_data.start.y - minY) / boxSize);
    int startZ = int((_data.start.z - minZ) / boxSize);
    int targetX = int((target.x - minX) / boxSize);
    int targetY = int((target.y - minY) / boxSize);
    int targetZ = int((target.z - minZ) / boxSize);

    // ���W�͈̔͂𐧌�
    startX = (int)max(0, min(startX, maxX / boxSize - 1));
    startY = (int)max(0, min(startY, maxY / boxSize - 1));
    startZ = (int)max(0, min(startZ, maxZ / boxSize - 1));
    targetX = (int)max(0, min(targetX, maxX / boxSize - 1));
    targetY = (int)max(0, min(targetY, maxY / boxSize - 1));
    targetZ = (int)max(0, min(targetZ, maxZ / boxSize - 1));

    int stepX = (targetX >= startX) ? 1 : -1;
    int stepY = (targetY >= startY) ? 1 : -1;
    int stepZ = (targetZ >= startZ) ? 1 : -1;

    // ���W�̏��� : ���StartPosition���烋�[�v�񂵂���������W�ɂ���đ��������߂�
    for (int x = startX; x != targetX + stepX; x += stepX) {
        for (int y = startY; y != targetY + stepY; y += stepY) {
            for (int z = startZ; z != targetZ + stepZ; z += stepZ) {

                // line �x�N�g���ɐ����ȃx�N�g�����v�Z
                XMVECTOR upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // �������̃x�N�g�����K�v�Ȃ�ύX
                XMVECTOR lineNormal = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&_data.dir), upVector));

                const XMFLOAT3 floatBoxPos[4] = {
                    { 0.0f, 0.0f, 0.0f},
                    { boxSize, 0.0f, 0.0f },
                    { boxSize, 0.0f, boxSize },
                    { 0.0f, 0.0f, boxSize }
                };

                // �l�p�`�̎l���̓_�����߂� / ���̓_����start�܂ł̋������v�Z / ���ςɂ����̕������v�Z
                XMVECTOR lineBase = XMLoadFloat3(&_data.start);
                XMVECTOR c[4];
                float dp[4] = {};
                for (int i = 0; i < 4; i++) {
                    // �l�p�`�̎l���̓_�̍��W�����߂�
                    XMFLOAT3 boxPos = XMFLOAT3(x * boxSize + floatBoxPos[i].x, y * boxSize + floatBoxPos[i].y, z * boxSize + floatBoxPos[i].z);
                    c[i] = XMLoadFloat3(&boxPos);
                    
                    // ���̓_����start�܂ł̋������v�Z
                    c[i] -= lineBase;
                
                    // ���ςɂ����̕��������߂�
                    dp[i] = XMVectorGetY(XMVector3Dot(lineNormal, c[i]));
                }

                
                // �S�����������ɂ���΁A���Ǝl�p�`���������Ă��邱�Ƃ͂Ȃ�
                if ((dp[0] * dp[1] <= 0) || (dp[1] * dp[2] <= 0) || (dp[2] * dp[3] <= 0)) {
                    Cell* cell = &cells_[y][z][x];
                    float minDist = 0;
                    if (!cell) continue;

                    // Ray���ɂ��������炻��Cell�œ����蔻�������
                    _data.start.y += 0.001f;
                    if (cell->SegmentVsTriangle(&_data, minDist)) {
                        return;
                    }
                }
            }
        }
    }
}

void CollisionMap::CalcMapWall(XMFLOAT3& _pos, float speed)
{
    const float radDef = 0.15f;
    const float rad = 0.15f * (1 + speed);
    const float dia = (0.15f * 2.0f) * (1 + speed);

    int checkX1, checkX2;
    int checkZ1, checkZ2;

    checkX1 = (int)(_pos.x + rad); //�O
    checkZ1 = (int)(_pos.z + dia);
    checkX2 = (int)(_pos.x - rad);
    checkZ2 = (int)(_pos.z + dia);
    if (IsWall(checkX1, checkZ1) == 1 || IsWall(checkX2, checkZ2) == 1) {
        _pos.z = _pos.z - radDef;
    }

    checkX1 = (int)(_pos.x + rad); //���
    checkZ1 = (int)(_pos.z - dia);
    checkX2 = (int)(_pos.x - rad);
    checkZ2 = (int)(_pos.z - dia);
    if (IsWall(checkX1, checkZ1) == 1 || IsWall(checkX2, checkZ2) == 1) {
        _pos.z = _pos.z + radDef;
    }

    checkX1 = (int)(_pos.x + dia); //�E
    checkZ1 = (int)(_pos.z + rad);
    checkX2 = (int)(_pos.x + dia);
    checkZ2 = (int)(_pos.z - rad);
    if (IsWall(checkX1, checkZ1) == 1 || IsWall(checkX2, checkZ2) == 1) {
        _pos.x = _pos.x - radDef;
    }

    checkX1 = (int)(_pos.x - dia); //��
    checkZ1 = (int)(_pos.z + rad);
    checkX2 = (int)(_pos.x - dia);
    checkZ2 = (int)(_pos.z - rad);
    if (IsWall(checkX1, checkZ1) == 1 || IsWall(checkX2, checkZ2) == 1) {
        _pos.x = _pos.x + radDef;
    }

}

bool CollisionMap::IsWall(int x, int z)
{
    int pos[2];
    pos[0] = static_cast<int>((float)x / floarSize);
    pos[1] = static_cast<int>((float)z / floarSize);
    std::vector<std::vector<int>> data = pStage->GetMapData();
    
    if (pos[0] < 0 || pos[1] < 0 || pos[0] > data[0].size() || pos[1] > data.size())
        return false;
    
    int wall = data[pos[1]][pos[0]];
    bool flag = wall == 1;

    return flag;
}
