#pragma once
#include "EnemyBase.h"
#include "../State/StateBase.h"

class SphereCollider;
class AstarMoveAction;
class OrientedMoveAction;
class RotateAction;
class VisionSearchAction;

class StoneGolem : public EnemyBase
{
    int hModel_;
    int boneIndex_[2];
    int partIndex_[2];

    DamageController* pDamageController_;
    AstarMoveAction* pMoveAction_;
    OrientedMoveAction* pOrientedMoveAction_;
    RotateAction* pRotateAction_;
    VisionSearchAction* pVisionSearchAction_;

public:
    StoneGolem(GameObject* parent);
    ~StoneGolem() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void OnAttackCollision(GameObject* pTarget) override;
    void DeadEnter() override;

    int GetModelHandle() { return hModel_; }
    DamageController* GetDamageController() { return pDamageController_; }
    AstarMoveAction* GetMoveAction() { return pMoveAction_; }
    OrientedMoveAction* GetOrientedMoveAction() { return pOrientedMoveAction_; }
    RotateAction* GetRotateAction() { return pRotateAction_; }
    VisionSearchAction* GetVisionSearchAction() { return pVisionSearchAction_; }

    void DamageInfoReset();
    void SetDamageInfoCombo1();
    void SetDamageInfoCombo2();
    void SetDamageInfoCombo3();

};
