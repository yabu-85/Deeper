#pragma once
#include "EnemyBase.h"
#include "../State/StateBase.h"

class AstarMoveAction;
class RotateAction;
class VisionSearchAction;

class ThrowEnemy : public EnemyBase
{
    int hModel_;
    int boneIndex_;
    int partIndex_;
    
    //Action
    AstarMoveAction* pMoveAction_;
    RotateAction* pRotateAction_;
    VisionSearchAction* pVisionSearchAction_;

public:
    ThrowEnemy(GameObject* parent);
    ~ThrowEnemy() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void ApplyDamage(int da) override;
    void OnCollision(GameObject* pTarget) override;

    AstarMoveAction* GetMoveAction() { return pMoveAction_; }
    RotateAction* GetRotateAction() { return pRotateAction_; }
    VisionSearchAction* GetVisionSearchAction() { return pVisionSearchAction_; }

};