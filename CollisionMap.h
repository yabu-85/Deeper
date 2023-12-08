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

    //コリジョンマップとの接触判定：めり込んでたら追い出す
    void MapDataVsBox(BoxCollider* collider);
    
    //コリジョンマップとの接触判定：めり込んでたら追い出す
    void MapDataVsSphere(SphereCollider* collider, XMFLOAT3 prePos);

};

