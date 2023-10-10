#pragma once
#include "Engine/GameObject.h"

class Aim;

class Player :
    public GameObject
{
    int hModel_;
    float moveSpeed_;
    XMFLOAT3 playerMovement_;   //���̈ړ���

    Aim* pAim_;

    void CalcMove(); //�ړ��ʌv�Z

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

