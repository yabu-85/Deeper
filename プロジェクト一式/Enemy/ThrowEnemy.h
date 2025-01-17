#pragma once
#include "EnemyBase.h"

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

    bool isHasItem_;        //今アイテムを持っているか

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

    void ThrowItem();
    void SetItem();

    int GetModelHandle() { return hModel_; }
    AstarMoveAction* GetMoveAction() { return pMoveAction_; }
    RotateAction* GetRotateAction() { return pRotateAction_; }
    VisionSearchAction* GetVisionSearchAction() { return pVisionSearchAction_; }

};