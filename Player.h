#pragma once
#include "Engine/GameObject.h"

class Aim;

class Player :
    public GameObject
{
    int hModel_;
    float moveSpeed_;
    XMFLOAT3 playerMovement_;   //今の移動量
    XMFLOAT3 moveVec_;          //移動キーの値を取得

    Aim* pAim_;

    void CalcMove(); //移動量計算

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //移動してたらTrue
    bool GetMoveVec(XMVECTOR& vec);

};

