#pragma once
#include "EnemyBase.h"

class PolyLine;
class AstarMoveAction;
class OrientedMoveAction;
class RotateAction;
class AnimationController;

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
    MAX,
};

const int SWORDBOSS_ANIMATION_DATA[(int)SWORDBOSS_ANIMATION::MAX][2] {
    {0, 120},     //Idle
    { 548, 590 }, //Run
    { 120, 175 }, //Roring
    { 500, 546 }, //Back
    { 175, 210 }, //DamageSmall
    { 210, 260 }, //DamageMid
    { 260, 495 }, //Dead
    { 600, 690 }, //Attack1ÅiêÿÇËè„Ç∞
    { 700, 775 }, //Attack2Åiâ°êÿÇË
};

class SwordBoss : public EnemyBase
{
    int hModel_;
    int polyCreatTime_;
    int hSwordModel_;
    int boneIndex_;
    int partIndex_;
    Transform swordTransform_;
    XMFLOAT3 preRotate_;

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
    void AttackEnd();
    void SetPolyCreatTime();

    int GetModelHandle() { return hModel_; }
    AnimationController* GetAnimationController() { return pAnimationController_; }
    DamageController* GetDamageController() { return pDamageController_; }
    AstarMoveAction* GetMoveAction() { return pMoveAction_; }
    OrientedMoveAction* GetOrientedMoveAction() { return pOrientedMoveAction_; }
    RotateAction* GetRotateAction() { return pRotateAction_; }
};
