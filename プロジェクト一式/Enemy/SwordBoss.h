#pragma once
#include "EnemyBase.h"

class SelectAttack;
class DoublePolyLine;
class AstarMoveAction;
class OrientedMoveAction;
class RotateAction;
class AnimationController;

enum class SWORDBOSS_ANIMATION {
    RUN = 0,
    WALK,
    BACK_STEP,
    DEAD,
    Slash_Up,
    Slash_Right,
    Slash_Jump,
    Thrust,
    MAX,
};

const int SWORDBOSS_ANIMATION_DATA[(int)SWORDBOSS_ANIMATION::MAX][2] {
    { 288, 330 }, //Run
    { 340, 415 }, //Run
    { 240, 286 }, //Back
    { 0, 235 },   //Dead
    { 420, 510 }, //Attack1（切り上げ
    { 520, 595 }, //Attack2（右切り
    { 710, 800 }, //Attack3（ジャンプ
    { 600, 700 }, //Attack4（突き刺し
};

class SwordBoss : public EnemyBase
{
    int hModel_;
    int hSwordModel_;
    int boneIndex_;
    int partIndex_;
    Transform swordTransform_;
    XMFLOAT3 prePosition_;
    XMFLOAT3 preRotate_;

    SelectAttack* pSelectAttack_;
    DoublePolyLine* pDoublePolyLine_;
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

    void CreateVfxSword();
    void CalcPoly();
    void CalcCollider();
    void AttackEnd();

    void SetDamageInfo1();
    void SetDamageInfo2();
    void SetDamageInfo3();

    int GetModelHandle() { return hModel_; }
    SelectAttack* GetSelectAttack() { return pSelectAttack_; }
    AnimationController* GetAnimationController() { return pAnimationController_; }
    DamageController* GetDamageController() { return pDamageController_; }
    AstarMoveAction* GetMoveAction() { return pMoveAction_; }
    OrientedMoveAction* GetOrientedMoveAction() { return pOrientedMoveAction_; }
    RotateAction* GetRotateAction() { return pRotateAction_; }
};
