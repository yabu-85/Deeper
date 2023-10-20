#pragma once
#include "Engine/GameObject.h"

class Aim;
class StateManager;

class Player :
    public GameObject
{
    int hModel_;
    float moveSpeed_;           //移動スピード
    XMFLOAT3 playerMovement_;   //今の移動量
    XMFLOAT3 moveVec_;          //移動キーの値を取得
    XMVECTOR vDirection_;       //プレイヤーの向いている方向（Vector）
    
    Aim* pAim_;
    StateManager* pStateManager_;

    void CalcMove();            //移動量計算

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

