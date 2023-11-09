#pragma once
#include "EnemyBase.h"
#include "StateBase.h"

class Root;

class Feet : public EnemyBase
{
    int hModel_;
    Root* root_;

public:
    Feet(GameObject* parent);
    ~Feet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};
