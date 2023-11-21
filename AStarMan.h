#pragma once
#include "EnemyBase.h"
#include "StateBase.h"

class SphereCollider;
class AStarMan : public EnemyBase
{
    int hModel_;
    XMFLOAT3 targetPos_;

public:
    AStarMan(GameObject* parent);
    ~AStarMan() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

