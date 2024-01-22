#pragma once
#include "Engine/GameObject.h"

class Character : public GameObject
{
protected:
    float bodyWeight_;  //�߂荞�ݎ��Ɏg���d��
    float bodyRange_;   //�߂荞�ݎ��Ɏg���͈�

    XMFLOAT3 movement_; //�ړ��ʂ�ۑ����邽�߂̂��

public:
    Character(GameObject* parent);
    virtual ~Character() {};
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0 {};
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    void SetMovement(XMFLOAT3 move) { movement_ = move; }                   //�v���C���[�����ǉ���������
    void SetMovement(XMVECTOR move) { XMStoreFloat3(&movement_, move); }    //�E�E
    XMFLOAT3 GetMovement() { return movement_; }
    XMVECTOR GetMovementVector() { return XMLoadFloat3(&movement_); }
    virtual void ResetMovement() { movement_ = { 0.0f, 0.0f, 0.0f }; };

    //�L�����N�^�[���m�̏Փ˂̍ۂ߂荞�݂������
    void ReflectCharacter(Character* pCharacter);

};

