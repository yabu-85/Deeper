#pragma once
#include "../Engine/GameObject.h"

class Character : public GameObject
{
    int knockBackTime_;             //�m�b�N�o�b�N�̎���
    int knockBackTimeMax_;          //�m�b�N�o�b�N�̎��ԕۑ��p
    float knockBackPower_;          //�m�b�N�o�b�N�̋���
    XMFLOAT3 knockBackDirection_;   //�m�b�N�o�b�N���������

    virtual void SmallKnockBack() {};
    virtual void MediumKnockBack() {};
    virtual void LargetKnockBack() {};

protected:
    float bodyWeight_;  //�߂荞�ݎ��Ɏg���d��
    float bodyRange_;   //�߂荞�ݎ��Ɏg���͈�
    XMFLOAT3 movement_; //�ړ��ʂ�ۑ����邽�߂̂��

public:
    Character(GameObject* parent, std::string name);
    virtual ~Character();
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0;
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    //�L�����N�^�[���m�̏Փ˂̍ۂ߂荞�݂������
    void ReflectCharacter();

    //----------------------------------------------------------------

    enum KNOCK_TYPE {
        SMALL = 0,
        MEDIUM,
        LARGE,
        MAX,
    };

    //�m�b�N�o�b�N������AKnockBackPower�ƈ����Ŋ|�����l�Ńm�b�N�o�b�N������
    void KnockBack(float speed);

    //�m�b�N�o�b�N�̏����Z�b�g����
    void SetKnockBack(KNOCK_TYPE type, int time, float power, XMFLOAT3 pos);

    //�Q�b�g�֐�
    int GetKnockBackTime() { return knockBackTime_; }
    int GetKnockBackTimeMax() { return knockBackTimeMax_; }
    
    //----------------------------------------------------------------

    float GetBodyRange() { return bodyRange_; }
    float GetBodyWeight() { return bodyWeight_; }

    void SetMovement(XMFLOAT3 move) { movement_ = move; }                   //�v���C���[�����ǉ���������
    void SetMovement(XMVECTOR move) { XMStoreFloat3(&movement_, move); }    //�E�E
    XMFLOAT3 GetMovement() { return movement_; }
    XMVECTOR GetMovementVector() { return XMLoadFloat3(&movement_); }
    virtual void ResetMovement() { movement_ = { 0.0f, 0.0f, 0.0f }; };

};

