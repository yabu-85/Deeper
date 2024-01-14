#pragma once
#include "Character.h"

class Aim;
class StateManager;
class PlayerCommand;
class PlayerWeapon;

class Player : public Character
{
    int hModel_[2];
    float moveSpeed_;           //移動スピード
    float rotateRatio_;         //回転の比率
    XMFLOAT3 playerMovement_;   //今の移動量(Keyの情報だけ

    Aim* pAim_;
    StateManager* pStateManager_;
    PlayerCommand* pCommand_;
    PlayerWeapon* pPlayerWeapon_;

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void Rotate();
    void Rotate(float ratio);

    XMFLOAT3 GetInputMove();        //Inputの移動量取得
    void FrontMove(float f = 1.0f); //前への移動
    void Move(float f = 1.0f);      //実際の移動
    void CalcRotate();              //移動量は考慮しない回転のみの計算
    void CalcMove();                //Inputを考慮したやつ
    void CalcNoMove();              //Input考慮してない、滑るやつとかの計算用

    void ResetKeyMovement() { playerMovement_ = XMFLOAT3(0.0f, 0.0f, 0.0f); }
    XMFLOAT3 GetKeyMovement() { return playerMovement_; }
    XMVECTOR GetDirectionVec();     //向いている方向（正規化済み

    void InitAvo();

    int GetModelHandle() { return hModel_[0]; }
    Aim* GetAim() { return pAim_; }
    PlayerCommand* GetCommand() { return pCommand_; }
    PlayerWeapon* GetPlayerWeapon() { return pPlayerWeapon_; }

};

