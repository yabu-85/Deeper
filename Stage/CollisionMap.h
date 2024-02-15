#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Fbx.h"
#include <vector>

class Triangle;
class Cell;

class CollisionMap : public GameObject
{
    //�q�[�v�|�C���^�̃G���[�o�邩��d���Ȃ������ɂ�
    std::vector<Triangle*> triangles_;
   
    Cell*** cells_ = 0;

    //�w�肵��Cell�̃|�C���^���擾 / �͈͊O�Ȃ�nullptr
    Cell* GetCell(XMFLOAT3 pos);

public:
    CollisionMap(GameObject* parent);
    ~CollisionMap();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void CreatIntersectDataTriangle();
    void ResetCellTriangle();

    bool GetRayCastMinDist(XMFLOAT3 camPos, XMFLOAT3 plaPos, RayCastData* _data, float& minDist);

    //�������Cell��Triangle��Ray�̔��蓖�������瑦�I��
    void RaySelectCellVsSegment(RayCastData& _data, XMFLOAT3 target);

    void CalcMapWall(XMFLOAT3& _pos, float speed);

    //CollisionMap�Ƃ̔��莞�ԓ�����̂��
    bool IsWall(int x, int z);

};

