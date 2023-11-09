#pragma once
#include "EnemyBase.h"
#include "StateBase.h"
#include "BehaviourNode.h"

class Feet : public EnemyBase
{
    int hModel_;
    BT::Sequence* seq1_;

public:
    Feet(GameObject* parent);
    ~Feet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void ApplyDamage(int da) override;

};
