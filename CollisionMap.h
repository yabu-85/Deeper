#pragma once
#include "Engine/GameObject.h"
#include <vector>

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

    std::vector<Triangle*>& GetCellInTriangle();

};

