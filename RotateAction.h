#pragma once
#include "BaseAction.h"

class RotateAction : public BaseAction
{
    float rotateRatio_;         //��]��
    Character* pTarget_;

public:
    RotateAction(Character* obj);
    RotateAction(Character* obj, float ratio);
    virtual ~RotateAction() override {};
    virtual void Update() override;
    virtual void Initialize() override;
    virtual void Terminate() override;

    void SetRatio(float ratio) { rotateRatio_ = ratio; }
    void SetTarget(Character* target) { pTarget_ = target; }

};