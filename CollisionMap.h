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

public:
    CollisionMap(GameObject* parent);
    ~CollisionMap();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    Cell* GetCell(XMFLOAT3 pos);

    bool GetRayCastMinDist(XMFLOAT3 camPos, XMFLOAT3 plaPos, RayCastData* _data, float& minDist);

    //�R���W�����}�b�v�Ƃ̐ڐG����F�߂荞��ł���ǂ��o��
    void MapDataVsBox(BoxCollider* collider);
    
    //�R���W�����}�b�v�Ƃ̐ڐG����F�߂荞��ł���ǂ��o��
    void MapDataVsSphere(SphereCollider* collider, XMFLOAT3 prePos);

    //�������Cell��Triangle��Ray�̔��蓖�������瑦�I��
    void RaySelectCellVsSegment(RayCastData& _data, XMFLOAT3 target);

};

