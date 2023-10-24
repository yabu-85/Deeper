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

    float GetRayCastMinDist(RayCastData* _data); //“–‚½‚ç‚È‚©‚Á‚½ê‡minMaxRang(100000000)‚ª•Ô‚Á‚Ä‚­‚é
};

