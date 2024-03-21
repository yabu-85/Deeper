#pragma once
#include "EnemyBase.h"
#include "../AnimationController.h"

class PolyLine;
class AnimationController;
class SphereCollider;
class AstarMoveAction;
class OrientedMoveAction;
class RotateAction;
class VisionSearchAction;

enum class MELEE_ANIMATION {
    IDLE = 0,
    WALK,
    RUN,
    PRE_RUN_ATTACK,
    RUN_ATTACK,
    MAX,
};

const AnimeData MELEE_ANIMATION_DATA[(int)MELEE_ANIMATION::MAX]{
    {0, 100},
    {105, 175},
    {180, 222},
    {240, 280},
    {280, 350},
};

class MeleeFighter : public EnemyBase
{
    int hModel_;
    int boneIndex_[2];
    int partIndex_[2];
    SphereCollider* pHandCollider_[2];

    PolyLine* pPolyLine_;
    AnimationController* pAnimationController_;
    DamageController* pDamageController_;
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
    void OnAttackCollision(GameObject* pTarget) override;

    void CalcPoly();
    void DamageInfoReset();
    void SetDamageInfoCombo1();

    int GetModelHandle() { return hModel_; }
    AnimationController* GetAnimationController() { return pAnimationController_; }
    DamageController* GetDamageController() { return pDamageController_; }
    AstarMoveAction* GetMoveAction() { return pMoveAction_; }
    OrientedMoveAction* GetOrientedMoveAction() { return pOrientedMoveAction_; }
    RotateAction* GetRotateAction() { return pRotateAction_; }
    VisionSearchAction* GetVisionSearchAction() { return pVisionSearchAction_; }
};
