#pragma once
#include "Engine/GameObject.h"

class Aim;

class Player :
    public GameObject
{
    int hModel_;
    bool isAvo_;                //������Ă��邩
    float moveSpeed_;           //�ړ��X�s�[�h
    XMFLOAT3 playerMovement_;   //���̈ړ���
    XMFLOAT3 moveVec_;          //�ړ��L�[�̒l���擾
    XMVECTOR vDirection_;       //�v���C���[�̌����Ă�������iVector�j
    Aim* pAim_;

    void CalcMove();            //�ړ��ʌv�Z

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    bool IsMoveKeyPushed();
    bool IsMove();

    XMVECTOR GetMoveVec() { return XMLoadFloat3(&moveVec_); };
    XMVECTOR GetDirection() { return vDirection_; };

};

