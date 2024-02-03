#pragma once
#include "BaseAction.h"

class RotateAction : public BaseAction
{
    float rotateRatio_;         //‰ñ“]—¦
    Character* pTarget_;

public:
    RotateAction(Character* obj);
    RotateAction(Character* obj, float ratio);
    virtual ~RotateAction() override {};
    virtual void Update() override;

    void SetRatio(float ratio) { rotateRatio_ = ratio; }
    void SetTarget(Character* target) { pTarget_ = target; }

};