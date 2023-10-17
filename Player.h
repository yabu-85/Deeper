#pragma once
#include "Engine/GameObject.h"

class Aim;

class Player :
    public GameObject
{
    int hModel_;
    float moveSpeed_;
    XMFLOAT3 playerMovement_;   //���̈ړ���
    XMFLOAT3 moveVec_;          //�ړ��L�[�̒l���擾
    XMVECTOR vDirection_;       //�v���C���[�̌����Ă�������iVector�j

    Aim* pAim_;

    void CalcMove();            //�ړ��ʌv�Z
    void CalcDirection();       //�����Ă�������v�Z

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //�ړ����Ă���True
    bool GetMoveVec(XMVECTOR& vec);

    //��]��X�̒l�͏��ZERO
    XMVECTOR GetDirection() { return vDirection_; };

};

