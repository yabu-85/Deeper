#pragma once
#include "EnemyBase.h"
#include "../State/StateBase.h"

class SphereCollider;
class AstarMoveAction;
class OrientedMoveAction;
class RotateAction;
class VisionSearchAction;

class MeleeFighter : public EnemyBase
{
    int hModel_;
    int boneIndex_[2];
    int partIndex_[2];
    SphereCollider* pHandCollider_[2];

    //Action
    AstarMoveAction* pMoveAction_;
    OrientedMoveAction* pOrientedMoveAction_;
    RotateAction* pRotateAction_;
    VisionSearchAction* pVisionSearchAction_;

public:
    MeleeFighter(GameObject* parent);
    ~MeleeFighter() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void ApplyDamage(int da) override;

    int GetModelHandle() { return hModel_; }
    SphereCollider* GetSphereCollider(int i) { return pHandCollider_[i]; }
    AstarMoveAction* GetMoveAction() { return pMoveAction_; }
    OrientedMoveAction* GetOrientedMoveAction() { return pOrientedMoveAction_; }
    RotateAction* GetRotateAction() { return pRotateAction_; }
    VisionSearchAction* GetVisionSearchAction() { return pVisionSearchAction_; }

};
