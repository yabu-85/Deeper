#pragma once
#include "BaseAction.h"

class Player;

class ActionRotate : public BaseAction
{
    float rotateRatio_;         //‰ñ“]—¦
    Player* pPlayer_;

public:
    ActionRotate(GameObject* obj);
    virtual ~ActionRotate() override {};
    virtual void Update() override;
    virtual void Initialize() override;
    virtual void Terminate() override;

    void SetRatio(float ratio) { rotateRatio_ = ratio; }

};