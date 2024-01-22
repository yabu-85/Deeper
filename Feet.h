#pragma once
#include "EnemyBase.h"
#include "StateBase.h"

class SphereCollider;
class AstarMoveAction;
class RotateAction;
class VisionSearchAction;

class Feet : public EnemyBase
{
    int hModel_;
    SphereCollider* pHandCollider_;

    //Action
    AstarMoveAction* pMoveAction_;
    RotateAction* pRotateAction_;
    VisionSearchAction* pVisionSearchAction_;

public:
    Feet(GameObject* parent);
    ~Feet() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void ApplyDamage(int da) override;
    void OnCollision(GameObject* pTarget) override;

    int GetModelHandle() { return hModel_; }
    SphereCollider* GetSphereCollider() { return pHandCollider_; }
    AstarMoveAction* GetMoveAction() { return pMoveAction_; }
    RotateAction* GetRotateAction() { return pRotateAction_; }
    VisionSearchAction* GetVisionSearchAction() { return pVisionSearchAction_; }

};
