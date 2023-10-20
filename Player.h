#pragma once
#include "Engine/GameObject.h"

class Aim;
class StateManager;

class Player :
    public GameObject
{
    int hModel_;
    float moveSpeed_;           //�ړ��X�s�[�h
    XMFLOAT3 playerMovement_;   //���̈ړ���
    XMFLOAT3 moveVec_;          //�ړ��L�[�̒l���擾

    Aim* pAim_;
    StateManager* pStateManager_;

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void Move(float f = 1.0f);      //���ۂ̈ړ�
    void CalcMove();                //Input���l���������
    void CalcNoMove();              //Input�l�����ĂȂ��A�����Ƃ��̌v�Z�p

    bool IsMoveKeyPushed();                 //bool����
    bool IsMoveKeyPushed(XMFLOAT3& key);    //�����Ă���p�x���l�������l
    bool IsMove();                          //�ړ��ʂ������ł������Ă��true

    XMVECTOR GetMoveVec() { return XMLoadFloat3(&moveVec_); };

};

