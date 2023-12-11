#pragma once
#include "BaseAction.h"
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class Player;

class MoveAction : public BaseAction
{
protected:
    bool isInRange_;    //Target�ɂ��Ă�����true
    float moveSpeed_;   //�ړ��X�s�[�h
    float moveRange_;   //�ړ��I������͈̔�
    XMFLOAT3 targetPos_;//�^�[�Q�b�g�̏ꏊ

public:
    MoveAction(GameObject* obj);
    MoveAction(GameObject* obj, float speed, float range);
    virtual ~MoveAction() override {};
    virtual void Update() override;
    virtual void Initialize() override {};
    virtual void Terminate() override {};

    void SetMoveSpeed(float speed) { moveSpeed_ = speed; }
    void SetMoveRange(float range) { moveRange_ = range; }
    bool IsInRange() { return isInRange_; }

    virtual void SetTarget(XMFLOAT3 target) { targetPos_ = target; }    //Astar�̏ꍇ�������Ⴄ����virtual
};

class AstarMoveAction : public MoveAction {
    std::vector<XMFLOAT3> targetList_;

public:
    AstarMoveAction(GameObject* obj);
    AstarMoveAction(GameObject* obj, float speed, float range);
    ~AstarMoveAction();

    void Update() override;
    void SetTarget(XMFLOAT3 target) override;
    std::vector<XMFLOAT3> GetTarget() { return targetList_; }
    void StopMove() { targetList_.clear(); }

};