#pragma once
#include "Engine/GameObject.h"
#include <vector>

class Triangle;

class CollisionMap : public GameObject
{

public:
    CollisionMap(GameObject* parent);
    ~CollisionMap();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //const Triangle* GetCellInTriangle(int& size);
    std::vector<Triangle*>& GetCellInTriangle();

};

