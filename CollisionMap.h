#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "Engine/Fbx.h"

class Triangle;
class Cell;
class BoxCollider;
class SphereCollider;

class CollisionMap : public GameObject
{
    Cell*** cells_ = 0;

    //�͈͓��Ȃ�true / �n���������ɒl������
    bool GetCellIndex(XMFLOAT3& pos);

    //�w�肵��Cell�̃|�C���^���擾 / �͈͊O�Ȃ�nullptr
    Cell* GetCell(XMFLOAT3 pos);

public:
    CollisionMap(GameObject* parent);
    ~CollisionMap();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    bool GetRayCastMinDist(XMFLOAT3 camPos, XMFLOAT3 plaPos, RayCastData* _data, float& minDist);

    //�R���W�����}�b�v�Ƃ̐ڐG����F�߂荞��ł���ǂ��o��
    void MapDataVsBox(BoxCollider* collider);
    
    //�R���W�����}�b�v�Ƃ̐ڐG����F�߂荞��ł���ǂ��o��
    void MapDataVsSphere(SphereCollider* collider, XMFLOAT3 prePos);

    //�������Cell��Triangle��Ray�̔��蓖�������瑦�I��
    void RaySelectCellVsSegment(RayCastData& _data, XMFLOAT3 target);

    void CalcMapWall(XMFLOAT3& _pos, float speed);

    //CollisionMap�Ƃ̔��莞�ԓ�����̂��
    bool IsWall(int x, int z);

};

