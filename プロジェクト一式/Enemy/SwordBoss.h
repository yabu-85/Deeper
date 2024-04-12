#pragma once
#include "EnemyBase.h"
#include "../AnimationController.h"

class PolyLine;
class AnimationController;
class AstarMoveAction;
class OrientedMoveAction;
class RotateAction;

enum class SWORDBOSS_ANIMATION {
    IDLE = 0,
    RUN,
    RORING,
    BACK_STEP,
    DAMAGE_SMALL,
    DAMAGE_MID,
    DEAD,
    ATTACK1,
    ATTACK2,
    ATTACK3,
    STONE_ATTACK,
    MAX,
};

const AnimeData SWORDBOSS_ANIMATION_DATA[(int)SWORDBOSS_ANIMATION::MAX]{
    {0, 120},     //Idle
    { 548, 590 }, //Run
    { 120, 175 }, //Roring
    { 500, 546 }, //Back
    { 175, 210 }, //DamageSmall
    { 210, 260 }, //DamageMid
    { 260, 495 }, //Dead
    { 595, 665 }, //Attack1
    { 665, 720 }, //Attack2
    { 675, 720 }, //Attack3
    { 725, 825 }, //Stone
};

class SwordBoss : public EnemyBase
{
    int hModel_;
    int hSwordModel_;
    int boneIndex_;
    int partIndex_;
    Transform swordTransform_;

    PolyLine* pPolyLine_;
    AnimationController* pAnimationController_;
    DamageController* pDamageController_;
    AstarMoveAction* pMoveAction_;
    OrientedMoveAction* pOrientedMoveAction_;
    RotateAction* pRotateAction_;

public:
    SwordBoss(GameObject* parent);
    ~SwordBoss() override;
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
};
