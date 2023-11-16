#pragma once
#include "EnemyBase.h"
#include "StateBase.h"

class SphereCollider;
class MoveAction;
class RotateAction;

class Feet : public EnemyBase
{
    int hModel_;
    SphereCollider* pHandCollider_;
    
    //Action
    MoveAction* pMoveAction_;
    RotateAction* pRotateAction_;

public:
    Feet(GameObject* parent);
    ~Feet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    MoveAction* GetMoveAction() { return pMoveAction_; }
    RotateAction* GetRotateAction() { return pRotateAction_; }

};
