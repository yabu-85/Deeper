#pragma once
#include "EnemyBase.h"
#include "StateBase.h"

class SphereCollider;

class ActionRotate;
class ActionMove;
class ActionAttack;

class Feet : public EnemyBase
{
    int hModel_;
    SphereCollider* pHandCollider_;

    ActionRotate* pActionRotate_;
    ActionMove* pActionMove_;
    ActionAttack* pActionAttack_;

public:
    Feet(GameObject* parent);
    ~Feet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    ActionRotate* GetActionRotate() { return pActionRotate_; }
    ActionMove* GetActionMove() { return pActionMove_; }
    ActionAttack* GetActionAttack() { return pActionAttack_; }

};
