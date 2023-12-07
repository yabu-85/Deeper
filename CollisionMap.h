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

    float GetRayCastMinDist(XMFLOAT3 pos, RayCastData* _data); //当たらなかった場合FLOATの最大値が返ってくる
    
    //コリジョンマップと立方体（AABB/OBB）とのあたり判定
    //コリジョンマップと円とのあたり判定を実装したい

};

