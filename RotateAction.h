#pragma once
#include "BaseAction.h"

class Player;

class RotateAction : public BaseAction
{
    float rotateRatio_;         //‰ñ“]—¦
    Player* pPlayer_;

public:
    RotateAction(GameObject* obj);
    RotateAction(GameObject* obj, float ratio);
    virtual ~RotateAction() override {};
    virtual void Update() override;
    virtual void Initialize() override;
    virtual void Terminate() override;

    void SetRatio(float ratio) { rotateRatio_ = ratio; }

};