#pragma once
#include "BaseAction.h"

class Player;

class MoveAction : public BaseAction
{
    bool isInRange_;    //Targetについていたらtrue
    float moveSpeed_;   //移動スピード
    float moveRange_;   //移動終了判定の範囲
    Player* pPlayer_;

public:
    MoveAction(GameObject* obj);
    MoveAction(GameObject* obj, float speed, float range);
    virtual ~MoveAction() override {};
    virtual void Update() override;
    virtual void Initialize() override;
    virtual void Terminate() override;

    void SetMoveSpeed(float speed) { moveSpeed_ = speed; }
    void SetMoveRange(float range) { moveRange_ = range; }
    bool IsInRange() { return isInRange_; }
};