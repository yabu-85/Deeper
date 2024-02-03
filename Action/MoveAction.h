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
    MoveAction(Character* obj, float speed, float range);
    virtual ~MoveAction() override {};
    virtual void Update() override;
    virtual void Initialize() override {};
    virtual void Terminate() override {};

    void SetMoveSpeed(float speed) { moveSpeed_ = speed; }
    void SetMoveRange(float range) { moveRange_ = range; }
    
    //�ړ����I������ATarget�̈ʒu�ɂ������ړ��I�����
    bool IsInRange() { return isInRange_; }

    //Astar�̏ꍇUpdate�Ă΂Ȃ��ƈړ����Ȃ��EIsOutEndTarget�Ɏg��
    virtual void SetTarget(XMFLOAT3 target) { targetPos_ = target; }
};

class AstarMoveAction : public MoveAction {
    std::vector<XMFLOAT3> targetList_;  //���̌o�H
    XMFLOAT3 lastTarget_;               //����TargetChara�̃|�W�V����

public:
    AstarMoveAction(Character* obj, float speed, float range);
    ~AstarMoveAction() override {};

    void Update() override;
    
    //���̈ړ��ڕW���X�g���擾
    std::vector<XMFLOAT3> GetTarget() { return targetList_; }
    
    //�ړ���߂�����
    void StopMove() { targetList_.clear(); }

    //�Ȃɂ��ꍡ�̖ڕW�̈ʒu�������Ȃ肷������true
    bool IsOutTarget(float range);
    
    //�V�����o�H�����߂�
    void UpdatePath(XMFLOAT3 target);

};