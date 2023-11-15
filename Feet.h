#pragma once
#include "EnemyBase.h"
#include "StateBase.h"

class SphereCollider;

class Feet : public EnemyBase
{
    int hModel_;
    SphereCollider* pHandCollider_;

public:
    Feet(GameObject* parent);
    ~Feet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};
