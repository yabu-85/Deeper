#pragma once
#include "Engine/GameObject.h"

class Aim;

class Player :
    public GameObject
{
    int hModel_;
    float moveSpeed_;
    XMFLOAT3 playerMovement_;   //¡‚ÌˆÚ“®—Ê

    Aim* pAim_;

    void CalcMove(); //ˆÚ“®—ÊŒvZ

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

