#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "Engine/Fbx.h"

class Triangle;
class Cell;

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
    
    //�R���W�����}�b�v�Ɨ����́iAABB/OBB�j�Ƃ̂����蔻��
    //�R���W�����}�b�v�Ɖ~�Ƃ̂����蔻�������������

};

