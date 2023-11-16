#pragma once
#include "BaseAction.h"

class Player;

class ActionMove : public BaseAction
{
    bool isInRange_;    //Targetについていたらtrue
    float moveSpeed_;   //移動スピード
    float moveRange_;   //移動終了判定の範囲
    Player* pPlayer_;

public:
    ActionMove(GameObject* obj);
    virtual ~ActionMove() override {};
    virtual void Update() override;
    virtual void Initialize() override;
    virtual void Terminate() override;

    void SetMoveSpeed(float speed) { moveSpeed_ = speed; }
    void SetMoveRange(float range) { moveRange_ = range; }
    bool IsInRange() { return isInRange_; }
};