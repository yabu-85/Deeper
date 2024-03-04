#pragma once
#include "EnemyBase.h"
#include "../State/StateBase.h"

class AstarMoveAction;
class RotateAction;
class VisionSearchAction;

class ThrowEnemy : public EnemyBase
{
    int hModel_;
    int hItemModel_;
    int boneIndex_;
    int partIndex_;
    Transform itemTransform_;

    bool isHasItem_;        //���A�C�e���������Ă��邩

    //Action
    AstarMoveAction* pMoveAction_;
    RotateAction* pRotateAction_;
    VisionSearchAction* pVisionSearchAction_;

    void SmallKnockBack() override;
    void MediumKnockBack() override;
    void LargetKnockBack() override;

public:
    ThrowEnemy(GameObject* parent);
    ~ThrowEnemy() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void ApplyDamage(int da) override;
    void OnAttackCollision(GameObject* pTarget) override;

    void ThrowItem();
    void SetItem();

    int GetModelHandle() { return hModel_; }
    AstarMoveAction* GetMoveAction() { return pMoveAction_; }
    RotateAction* GetRotateAction() { return pRotateAction_; }
    VisionSearchAction* GetVisionSearchAction() { return pVisionSearchAction_; }

};