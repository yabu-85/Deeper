#pragma once
#include "BaseAction.h"
#include <DirectXMath.h>

using namespace DirectX;

class Player;

class MoveAction : public BaseAction
{
    bool isInRange_;    //Target�ɂ��Ă�����true
    float moveSpeed_;   //�ړ��X�s�[�h
    float moveRange_;   //�ړ��I������͈̔�
    XMFLOAT3 targetPos_;//�^�[�Q�b�g�̏ꏊ

public:
    MoveAction(GameObject* obj);
    MoveAction(GameObject* obj, float speed, float range);
    virtual ~MoveAction() override {};
    virtual void Update() override;
    virtual void Initialize() override;
    virtual void Terminate() override;

    void SetMoveSpeed(float speed) { moveSpeed_ = speed; }
    void SetMoveRange(float range) { moveRange_ = range; }
    void SetTarget(XMFLOAT3 target) { targetPos_ = target; }
    bool IsInRange() { return isInRange_; }
};