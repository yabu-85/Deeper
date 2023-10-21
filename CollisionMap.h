#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "Engine/Fbx.h"

class Triangle;
class Cell;

class CollisionMap : public GameObject
{
    std::vector<Cell*> cells_;

public:
    CollisionMap(GameObject* parent);
    ~CollisionMap();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    float GetRayCastMinDist(RayCastData* _data); //当たらなかった場合minMaxRang(10000000)が返ってくる
};

