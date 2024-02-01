#pragma once
#include "Character.h"

class Aim;
class StateManager;
class PlayerCommand;
class PlayerWeapon;

class Player : public Character
{
    enum MAIN_STATE {
        APPER = 0,
        HEAR,
        DEAD,
    }state_;
    void ApperUpdate();
    void HearUpdate();
    void DeadUpdate();

    int hModel_[2];
    float moveSpeed_;           //移動スピード
    float rotateRatio_;         //回転の比率
    XMFLOAT3 playerMovement_;   //Normalizeの移動量を保存する用

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
    void OnCollision(GameObject* pTarget) override;
    void OnAttackCollision(GameObject* pTarget) override;

    void Rotate();
    void Rotate(float ratio);
    void AimTargetRotate();
    void AimTargetRotate(float ratio);

    XMFLOAT3 GetInputMove();        //Inputの移動量取得
    void FrontMove(float f = 1.0f); //前への移動
    void Move(float f = 1.0f);      //移動
    void CalcMove();                //Inputを考慮したやつ
    void CalcNoMove();              //Input考慮してない、滑るやつとかの計算用

    XMVECTOR GetDirectionVec();     //向いている方向（正規化済み
    void ResetKeyMovement() { movement_ = XMFLOAT3(0.0f, 0.0f, 0.0f); }
    void ResetMovement() override;

    void InitAvo();

    int GetModelHandle() { return hModel_[0]; }
    Aim* GetAim() { return pAim_; }
    PlayerCommand* GetCommand() { return pCommand_; }
    PlayerWeapon* GetPlayerWeapon() { return pPlayerWeapon_; }

};

