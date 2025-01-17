#pragma once
#include "BaseAction.h"

/// <summary>
/// 指定した方向へ回転させるクラス
/// </summary>
class RotateAction : public BaseAction
{
    float rotateRatio_;         //回転率
    Character* pTarget_;

public:
    RotateAction(Character* obj);
    RotateAction(Character* obj, float ratio);
    virtual ~RotateAction() override {};
    virtual void Update() override;

    void SetRatio(float ratio) { rotateRatio_ = ratio; }
    void SetTarget(Character* target) { pTarget_ = target; }

};