#pragma once
#include "BaseAction.h"

class GameObject;

class RotateAction : public BaseAction
{
    float rotateRatio_;         //‰ñ“]—¦
    GameObject* pParent_;
    GameObject* pTarget_;

public:
    RotateAction(GameObject* obj);
    RotateAction(GameObject* obj, float ratio);
    virtual ~RotateAction() override {};
    virtual void Update() override;
    virtual void Initialize() override;
    virtual void Terminate() override;

    void SetRatio(float ratio) { rotateRatio_ = ratio; }
    void SetTarget(GameObject* target) { pTarget_ = target; }

};