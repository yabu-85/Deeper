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

    Aim* pAim_;
    StateManager* pStateManager_;

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void Move(float f = 1.0f);      //実際の移動
    void CalcMove();                //Inputを考慮したやつ
    void CalcNoMove();              //Input考慮してない、滑るやつとかの計算用

    void InitAvo();

    bool IsMoveKeyPushed();                 //boolだけ
    bool IsMoveKeyPushed(XMFLOAT3& key);    //向いている角度を考慮した値
    bool IsMove();                          //移動量が少しでも入ってればtrue

    XMVECTOR GetMoveVec() { return XMLoadFloat3(&moveVec_); };  //移動方向（正規化済み
    XMVECTOR GetDirectionVec();                                 //向いている方向（正規化済み

    void ResetMovement() { playerMovement_ = XMFLOAT3(0.0f, 0.0f, 0.0f); };

};

