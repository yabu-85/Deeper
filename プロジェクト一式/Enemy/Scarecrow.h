#pragma once
#include "EnemyBase.h"

class MoveAction;

class Scarecrow : public EnemyBase
{
    int hModel_;
    MoveAction* pMoveAction_;
    void Dead() override;

public:
    Scarecrow(GameObject* parent);
    ~Scarecrow() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};
