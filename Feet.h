#pragma once
#include "EnemyBase.h"
#include "StateBase.h"

class SphereCollider;
class AstarMoveAction;
class RotateAction;

class Feet : public EnemyBase
{
    int hModel_;
    SphereCollider* pHandCollider_;
    
    //Action
    AstarMoveAction* pMoveAction_;
    RotateAction* pRotateAction_;

public:
    Feet(GameObject* parent);
    ~Feet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    SphereCollider* GetSphereCollider() { return pHandCollider_; }

    AstarMoveAction* GetMoveAction() { return pMoveAction_; }
    RotateAction* GetRotateAction() { return pRotateAction_; }

};
