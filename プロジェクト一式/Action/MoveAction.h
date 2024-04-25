#pragma once
#include "BaseAction.h"
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

//pTarget��null�o�Ȃ��ꍇ�́ApTarget�̕����ֈړ�
//pTarget��null�Ȃ�targetPos�ɕ����Ɉړ�
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

    //���̓G���悯��悤��move���v�Z
    void CalcDodge(XMVECTOR& move);

    void SetMoveSpeed(float speed) { moveSpeed_ = speed; }
    void SetMoveRange(float range) { moveRange_ = range; }
    
    //�ړ����I������ATarget�̈ʒu�ɂ������ړ��I�����
    bool IsInRange() { return isInRange_; }

    //Astar�̏ꍇUpdate�Ă΂Ȃ��ƈړ����Ȃ�
    void SetTarget(XMFLOAT3 target) { targetPos_ = target; }
};

class AstarMoveAction : public MoveAction {
    std::vector<XMFLOAT3> targetList_;  //���̌o�H
    XMFLOAT3 lastTarget_;               //����TargetChara�̃|�W�V����
    int handle_;

public:
    AstarMoveAction(Character* obj, float speed, float range);
    void Update() override;

    //���̈ړ��ڕW���X�g���擾
    std::vector<XMFLOAT3> GetTarget() { return targetList_; }

    //�ړ���߂�����
    void StopMove() { targetList_.clear(); }

    //���̖ڕW�̈ʒu�������Ȃ肷������true
    bool IsOutTarget(float range);

    //�V�����o�H�����߂�
    void UpdatePath(XMFLOAT3 target);

    //�f�o�b�O�p�̌o�H�\��
    void Draw();
};

class OrientedMoveAction : public MoveAction {
    XMVECTOR direction_;    //�ړ��̌���
    XMVECTOR move_;         //�ړ���

public:
    OrientedMoveAction(Character* obj, float speed);
    ~OrientedMoveAction() override {};
    void Update() override;
    void SetDirection(XMVECTOR vec) { direction_ = vec; }

    //count�̐��ړ��������A�ǂɐڐG���邩�ǂ����F�ǂ̏ꍇtrue
    bool CheckWallCollision(int count);

    //�G�l�~�[�̈ʒu�ƃ^�[�Q�b�g�̈ʒu����������v�Z���ăZ�b�g
    void CalcOptimalDirection();

    //�S�����m��������Direction���擾�O�͂Ȃ��A������΍����قǊm���オ��
    void SelectProbabilityDirection(int f, int b, int r, int l);

    //Direction���t��
    void InverseDirection();

};