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

    float GetRayCastMinDist(XMFLOAT3 pos, RayCastData* _data); //������Ȃ������ꍇFLOAT�̍ő�l���Ԃ��Ă���
    
    //�R���W�����}�b�v�Ƃ̐ڐG����F�߂荞��ł���ǂ��o��
    void MapDataVsBox(BoxCollider* collid);
    
    //�R���W�����}�b�v�Ƃ̐ڐG����F�߂荞��ł���ǂ��o��
    void MapDataVsSphere(SphereCollider* collid);

};

